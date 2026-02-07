"""EcoSystem - main entry point for ACOM component interaction.

This module implements the Python equivalent of CEcoSystem1 from EcoOS.
It provides initialization of the runtime environment and access to
the interface bus for component registration and querying.

The bootstrap sequence mirrors ``createCEcoSystem1`` from C:
    1. Load InterfaceBus DLL -> create instance via factory Alloc
    2. Load MemoryManager DLL -> register factory with bus
    3. Configure MemExt (set manager CID, enable pool)
    4. QueryComponent MemoryManager -> Init(0, heap_size)
    5. Load FileSystem DLL -> register factory with bus (optional)
    6. Configure FileExt (optional)
    7. Scan user_dll_path for component DLLs -> register each
    8. Init bus factory (factory.Init(None, bus_ptr))

After initialization the user accesses the ``bus`` property and
works with components via ``bus.QueryComponent()``.

Example:
    >>> with EcoSystem() as eco:
    ...     ppv = VoidPtr()
    ...     eco.bus.QueryComponent(
    ...         ByRef(CID_Calculator), None,
    ...         ByRef(IID_IEcoCalculatorX), ByRef(ppv),
    ...     )
    ...     calc = IEcoCalculatorX(ppv)
    ...     calc.Addition(10, 20)
    ...     calc.Release()
"""

from __future__ import annotations

import os
from pathlib import Path
from typing import Any, Optional, Union

from eco_python2acom.core.errors import EcoError
from eco_python2acom.core.types import Bool, ByRef, CastPtr, UInt32, VoidPtr
from eco_python2acom.interfaces.base import IEcoComponentFactory, IEcoUnknownPtr
from eco_python2acom.interfaces.guids.cid import (
    CID_EcoFileSystemManagement1,
    CID_EcoInterfaceBus1,
    CID_EcoMemoryManager1,
)
from eco_python2acom.interfaces.guids.iid import (
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1FileExt,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoMemoryAllocator1,
    IID_IEcoMemoryManager1,
)
from eco_python2acom.interfaces.system.interface_bus import (
    IEcoInterfaceBus1,
    IEcoInterfaceBus1FileExt,
    IEcoInterfaceBus1MemExt,
)
from eco_python2acom.interfaces.system.memory_manager import (
    IEcoMemoryAllocator1,
    IEcoMemoryManager1,
)
from eco_python2acom.runtime.helpers import filename_to_guid, get_runtime_dll_path, is_eco_dll
from eco_python2acom.runtime.loader import DllLoader, LoadedDll

# Default heap size for MemoryManager (~1.5 GB)
_DEFAULT_HEAP_SIZE = 0x60000000

# System CIDs that are loaded from ECO_FRAMEWORK_RT (not user components)
_SYSTEM_CIDS = frozenset(
    bytes(cid.Data)
    for cid in (CID_EcoInterfaceBus1, CID_EcoMemoryManager1, CID_EcoFileSystemManagement1)
)


class EcoSystem:
    """EcoOS System container -- Unikernel Bridge for Python.

    Bootstraps the ACOM runtime (InterfaceBus, MemoryManager,
    optionally FileSystemManagement), auto-registers user component
    DLLs found in ``user_dll_path``, and exposes the ``bus`` for
    the user to query components.

    Usage as a context manager::

        with EcoSystem() as eco:
            ppv = VoidPtr()
            eco.bus.QueryComponent(ByRef(cid), None, ByRef(iid), ByRef(ppv))
            calc = IEcoCalculatorX(ppv)
            calc.Addition(10, 20)
            calc.Release()

    Attributes:
        bus: The initialized ``IEcoInterfaceBus1`` instance.
    """

    def __init__(
        self,
        runtime_path: Optional[str] = None,
        user_dll_path: Optional[Union[str, Path]] = None,
        heap_size: int = _DEFAULT_HEAP_SIZE,
    ) -> None:
        """Initialize EcoSystem container.

        Args:
            runtime_path: Path to ``ECO_FRAMEWORK_RT`` directory.
                If None, reads the ``ECO_FRAMEWORK_RT`` environment variable.
            user_dll_path: Directory to scan for user component DLLs.
                If None, uses the current working directory.
            heap_size: Heap size for the memory manager in bytes.
                Defaults to ~1.5 GB (0x60000000).
        """
        self._runtime_path = runtime_path or os.environ.get("ECO_FRAMEWORK_RT", None)
        self._user_dll_path = Path(user_dll_path) if user_dll_path else Path.cwd()
        self._heap_size = UInt32(heap_size)
        self._loader = DllLoader()
        self._is_initialized = False

        # Internal references (set during init, released during cleanup)
        self._bus_factory: Optional[IEcoComponentFactory] = None
        self._bus: Optional[IEcoInterfaceBus1] = None
        self._mem_mgr: Optional[IEcoMemoryManager1] = None
        self._mem_alloc: Optional[IEcoMemoryAllocator1] = None

        # Keep loaded DLLs alive so ctypes doesn't unload them
        self._loaded_dlls: list[LoadedDll] = []

    # =========================================================================
    # Properties
    # =========================================================================

    @property
    def is_initialized(self) -> bool:
        """Whether the system has been initialized."""
        return self._is_initialized

    @property
    def bus(self) -> IEcoInterfaceBus1:
        """The initialized interface bus.

        Use this to query/create components::

            ppv = VoidPtr()
            eco.bus.QueryComponent(ByRef(cid), None, ByRef(iid), ByRef(ppv))

        Raises:
            RuntimeError: If system not initialized.
        """
        if not self._is_initialized or self._bus is None:
            raise RuntimeError("EcoSystem is not initialized. Call init() first.")
        return self._bus

    @property
    def loader(self) -> DllLoader:
        """The DLL loader instance."""
        return self._loader

    # =========================================================================
    # Context Manager
    # =========================================================================

    def __enter__(self) -> EcoSystem:
        """Enter context manager -- initialize system."""
        if not self._is_initialized:
            self.init()
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Exit context manager -- release resources."""
        self.release()

    # =========================================================================
    # Initialization (mirrors createCEcoSystem1)
    # =========================================================================

    def init(self) -> None:
        """Initialize the EcoOS runtime environment.

        Follows the same bootstrap sequence as ``createCEcoSystem1``:

        1. InterfaceBus -- load, create instance
        2. MemoryManager -- register, configure MemExt, init heap
        3. FileSystem -- register, configure FileExt (optional)
        4. User DLLs -- scan ``user_dll_path``, register each
        5. Finalize bus factory

        Raises:
            FileNotFoundError: If runtime DLLs cannot be found.
            EcoError: If any initialization step fails.
        """
        if self._is_initialized:
            return

        if self._runtime_path is None:
            raise FileNotFoundError(
                "Runtime path not configured. "
                "Set ECO_FRAMEWORK_RT environment variable or pass runtime_path."
            )

        rt_path = get_runtime_dll_path(self._runtime_path)

        # Step 1: Load InterfaceBus and create instance
        self._init_interface_bus(rt_path)

        # Step 2: Register MemoryManager factory with bus
        self._register_memory_manager(rt_path)

        # Step 3: Configure MemExt on the bus
        self._configure_mem_ext()

        # Step 4: Initialize MemoryManager heap
        self._init_memory_manager()

        # Step 5: Register FileSystemManagement factory (optional)
        self._register_file_system(rt_path)

        # Step 6: Configure FileExt on the bus (optional)
        self._configure_file_ext()

        # Step 7: Auto-register user component DLLs
        self._scan_and_register_user_dlls()

        # Step 8: Finalize bus factory
        if self._bus_factory is not None and self._bus is not None:
            self._bus_factory.Init(None, self._bus._ptr)

        self._is_initialized = True

    # =========================================================================
    # Bootstrap helpers (private)
    # =========================================================================

    def _init_interface_bus(self, rt_path: Path) -> None:
        """Load InterfaceBus DLL, create bus instance via factory."""
        loaded = self._loader.load_by_cid(CID_EcoInterfaceBus1, [rt_path])
        self._loaded_dlls.append(loaded)
        self._bus_factory = loaded.factory

        bus_ptr = VoidPtr()
        result = self._bus_factory.Alloc(None, None, ByRef(IID_IEcoInterfaceBus1), ByRef(bus_ptr))
        if result != 0 or not bus_ptr.value:
            raise EcoError(result, "Failed to create InterfaceBus instance")

        self._bus = IEcoInterfaceBus1(bus_ptr)

    def _register_memory_manager(self, rt_path: Path) -> None:
        """Load MemoryManager DLL and register its factory with the bus."""
        loaded = self._loader.load_by_cid(CID_EcoMemoryManager1, [rt_path])
        self._loaded_dlls.append(loaded)

        result = self._bus.RegisterComponent(
            ByRef(CID_EcoMemoryManager1),
            CastPtr(loaded.factory._ptr, IEcoUnknownPtr),
        )
        if result != 0:
            raise EcoError(result, "Failed to register MemoryManager")

    def _configure_mem_ext(self) -> None:
        """Configure InterfaceBus memory extension."""
        mem_ext_ptr = VoidPtr()
        result = self._bus.QueryInterface(ByRef(IID_IEcoInterfaceBus1MemExt), ByRef(mem_ext_ptr))
        if result == 0 and mem_ext_ptr.value:
            mem_ext = IEcoInterfaceBus1MemExt(mem_ext_ptr)
            mem_ext.set_Manager(ByRef(CID_EcoMemoryManager1))
            mem_ext.set_ExpandPool(Bool(True))
            mem_ext.Release()

    def _init_memory_manager(self) -> None:
        """Query and initialize MemoryManager with heap."""
        mgr_ptr = VoidPtr()
        result = self._bus.QueryComponent(
            ByRef(CID_EcoMemoryManager1),
            None,
            ByRef(IID_IEcoMemoryManager1),
            ByRef(mgr_ptr),
        )
        if result != 0 or not mgr_ptr.value:
            raise EcoError(result, "Failed to get MemoryManager interface")

        self._mem_mgr = IEcoMemoryManager1(mgr_ptr)
        self._mem_mgr.Init(None, self._heap_size)

        # Also get IEcoMemoryAllocator1 for convenience
        alloc_ptr = VoidPtr()
        result = self._bus.QueryComponent(
            ByRef(CID_EcoMemoryManager1),
            None,
            ByRef(IID_IEcoMemoryAllocator1),
            ByRef(alloc_ptr),
        )
        if result != 0 or not alloc_ptr.value:
            raise EcoError(result, "Failed to get MemoryAllocator interface")

        self._mem_alloc = IEcoMemoryAllocator1(alloc_ptr)

    def _register_file_system(self, rt_path: Path) -> None:
        """Load FileSystemManagement DLL and register with bus (optional)."""
        try:
            loaded = self._loader.load_by_cid(CID_EcoFileSystemManagement1, [rt_path])
        except FileNotFoundError:
            return  # FileSystem is optional

        self._loaded_dlls.append(loaded)

        result = self._bus.RegisterComponent(
            ByRef(CID_EcoFileSystemManagement1),
            CastPtr(loaded.factory._ptr, IEcoUnknownPtr),
        )
        if result != 0:
            raise EcoError(result, "Failed to register FileSystemManagement")

    def _configure_file_ext(self) -> None:
        """Configure InterfaceBus file extension (optional)."""
        file_ext_ptr = VoidPtr()
        result = self._bus.QueryInterface(ByRef(IID_IEcoInterfaceBus1FileExt), ByRef(file_ext_ptr))
        if result == 0 and file_ext_ptr.value:
            file_ext = IEcoInterfaceBus1FileExt(file_ext_ptr)
            file_ext.set_Manager(ByRef(CID_EcoFileSystemManagement1))
            file_ext.Release()

    def _scan_and_register_user_dlls(self) -> None:
        """Scan user_dll_path for EcoOS DLLs and register each with the bus.

        Skips DLLs whose CID matches a known system component (Bus,
        MemoryManager, FileSystem) since those are already loaded.
        """
        if not self._user_dll_path.is_dir():
            return

        for dll_file in self._user_dll_path.iterdir():
            if not dll_file.is_file() or not is_eco_dll(dll_file.name):
                continue

            # Derive CID from filename
            cid = filename_to_guid(dll_file.name)
            if cid is None:
                continue

            # Skip system components (already registered)
            if bytes(cid.Data) in _SYSTEM_CIDS:
                continue

            try:
                loaded = self._loader.load(dll_file)
                self._loaded_dlls.append(loaded)

                self._bus.RegisterComponent(
                    ByRef(cid),
                    CastPtr(loaded.factory._ptr, IEcoUnknownPtr),
                )
            except Exception:
                # Non-critical: skip DLLs that fail to load/register
                continue

    # =========================================================================
    # Cleanup
    # =========================================================================

    def release(self) -> None:
        """Release all internal resources.

        Releases in reverse order (MemAllocator -> MemManager -> Bus),
        matching the C-side ``deleteCEcoSystem1`` logic.
        """
        if not self._is_initialized:
            return

        # Release in reverse initialization order
        for iface in (self._mem_alloc, self._mem_mgr, self._bus):
            if iface is not None:
                try:
                    iface.Release()
                except Exception:
                    pass

        self._bus = None
        self._bus_factory = None
        self._mem_mgr = None
        self._mem_alloc = None
        self._loaded_dlls.clear()
        self._is_initialized = False

    def __repr__(self) -> str:
        status = "initialized" if self._is_initialized else "not initialized"
        return f"<EcoSystem [{status}]>"
