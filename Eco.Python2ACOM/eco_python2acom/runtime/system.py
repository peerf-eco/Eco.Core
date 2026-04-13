"""EcoSystem - main entry point for ACOM component interaction.

This module implements the Python equivalent of `CEcoSystem1` from EcoOS.
It provides initialization of the runtime environment and access to
the interface bus for component registration and querying.

The bootstrap sequence mirrors `createCEcoSystem1` from C:
    1. Load `InterfaceBus` library -> create instance via factory `Alloc`
    2. Load `MemoryManager` library -> register factory with bus
    3. Configure `MemExt` -> set manager CID, enable pool
    4. Query component `MemoryManager` -> initialize heap
    5. Load `FileSystemManagement` library -> register factory with bus (optional)
    6. Configure `FileExt` (optional)
    7. Scan user paths for component shared libraries -> register each
    8. Init bus factory -> finalize bus setup

Example:
    >>> with EcoSystem() as eco:
    ...     ppv = Ptr[Void]()
    ...     eco.bus.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
    ...     calc = IEcoCalculatorX(ppv)
    ...     calc.Addition(10, 20)
    ...     calc.Release()
"""

import os
from pathlib import Path
from typing import Any, ClassVar, Optional, Self, Union

from eco_python2acom.guids.cid import (
    CID_EcoFileSystemManagement1,
    CID_EcoInterfaceBus1,
    CID_EcoMemoryManager1,
)
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.guids.iid import (
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1FileExt,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoMemoryAllocator1,
    IID_IEcoMemoryManager1,
)
from eco_python2acom.interfaces.base import IEcoComponentFactory, IEcoUnknown
from eco_python2acom.interfaces.interface_bus import (
    IEcoInterfaceBus1,
    IEcoInterfaceBus1FileExt,
    IEcoInterfaceBus1MemExt,
)
from eco_python2acom.interfaces.memory_manager import IEcoMemoryAllocator1, IEcoMemoryManager1
from eco_python2acom.runtime.loader import EcoLib, EcoLibLoader
from eco_python2acom.runtime.utils import is_eco_lib, lib_filename_to_guid
from eco_python2acom.types.core import Bool, UInt32, Void
from eco_python2acom.types.errors import EcoError
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast


class EcoSystem:
    """EcoOS System container -- Unikernel Bridge for Python.

    Bootstraps the ACOM runtime (`InterfaceBus`, `MemoryManager`,
    optionally `FileSystemManagement`), auto-registers user component
    libraries.

    Usage as a context manager:

        with EcoSystem() as eco:
            ppv = Ptr[Void]()
            eco.bus.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
            calc = IEcoCalculatorX(ppv)
            calc.Addition(10, 20)
            calc.Release()

    Attributes:
        initialized: Whether the system has been initialized.
        bus: The initialized `IEcoInterfaceBus1` instance.
        mem_manager: The initialized `IEcoMemoryManager1` instance.
        mem_allocator: The initialized `IEcoMemoryAllocator1` instance.
        gid: The generation ID (GID) of the system.
        runtime_path: The path to the runtime libraries.
    """

    # Default heap size for MemoryManager (~1.5 GB)
    DEFAULT_HEAP_SIZE: ClassVar[int] = 0x60000000

    def __init__(
        self,
        runtime_path: Optional[str] = None,
        user_lib_dir: Optional[Union[str, Path]] = None,
        heap_size: int = DEFAULT_HEAP_SIZE,
        gid: Optional[UGUID] = None,
    ) -> None:
        """Initialize `EcoSystem` container.

        Args:
            runtime_path: Path to `ECO_FRAMEWORK_RT` directory.
                If None, reads the `ECO_FRAMEWORK_RT` environment variable.
            user_lib_dir: Directory to scan for user component libraries.
                If None, uses the current working directory.
            heap_size: Heap size for the memory manager in bytes.
                Defaults to ~1.5 GB (0x60000000).
            gid: Generation ID (GID) of the system.
                If None, uses the default GID for the current architecture.
        """
        self._runtime_path = runtime_path or os.environ.get("ECO_FRAMEWORK_RT", None)
        self._user_lib_dir = Path(user_lib_dir) if user_lib_dir else Path.cwd()
        self._gid = gid or GID_IEcoSystem

        # Initialize internal references (set during init, released during cleanup)
        self._bus_factory: Optional[IEcoComponentFactory] = None
        self._bus: Optional[IEcoInterfaceBus1] = None
        self._mem_manager: Optional[IEcoMemoryManager1] = None
        self._mem_allocator: Optional[IEcoMemoryAllocator1] = None
        self._system_cids = frozenset(
            bytes(cid.data)
            for cid in (CID_EcoInterfaceBus1, CID_EcoMemoryManager1, CID_EcoFileSystemManagement1)
        )

        # Keep loaded libraries alive
        self._loader = EcoLibLoader()
        self._loaded_libs: list[EcoLib] = []

        # Initialize state
        self._heap_size = UInt32(heap_size)
        self._initialized = False

    # =========================================================================
    # Properties
    # =========================================================================

    @property
    def initialized(self) -> bool:
        """Whether the system has been initialized."""
        return self._initialized

    @property
    def bus(self) -> IEcoInterfaceBus1:
        """The initialized interface bus."""
        if not self._initialized or self._bus is None:
            raise RuntimeError("Access to 'bus' requires initialized EcoSystem")
        return self._bus

    @property
    def mem_manager(self) -> IEcoMemoryManager1:
        """The initialized memory manager."""
        if not self._initialized or self._mem_manager is None:
            raise RuntimeError("Access to 'mem_manager' requires initialized EcoSystem")
        return self._mem_manager

    @property
    def mem_allocator(self) -> IEcoMemoryAllocator1:
        """The initialized memory allocator."""
        if not self._initialized or self._mem_allocator is None:
            raise RuntimeError("Access to 'mem_allocator' requires initialized EcoSystem")
        return self._mem_allocator

    @property
    def gid(self) -> UGUID:
        """The generation ID (GID) of the system."""
        return self._gid

    @property
    def runtime_path(self) -> Optional[str]:
        """The path to the runtime libraries."""
        return self._runtime_path

    # =========================================================================
    # Context Manager
    # =========================================================================

    def __enter__(self) -> Self:
        """Enter context manager -- initialize system."""
        if not self._initialized:
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

        Raises:
            FileNotFoundError: If runtime libraries cannot be found.
            EcoError: If any initialization step fails.
        """
        if self._initialized:
            return

        if self._runtime_path is None:
            raise FileNotFoundError("Runtime path not configured")

        self._full_runtime_path = Path(self._runtime_path) / self._gid.to_string(with_hyphens=False)
        if not self._full_runtime_path.exists():
            raise FileNotFoundError(f"Runtime path not found: '{self._full_runtime_path}'")

        # Step 1: Load InterfaceBus and create instance
        self._init_interface_bus(self._full_runtime_path)

        # Step 2: Register MemoryManager factory with bus
        self._register_memory_manager(self._full_runtime_path)

        # Step 3: Configure MemExt on the bus
        self._configure_mem_ext()

        # Step 4: Initialize MemoryManager heap
        self._init_memory_manager()

        # Step 5: Register FileSystemManagement factory
        self._register_file_system(self._full_runtime_path)

        # Step 6: Configure FileExt on the bus
        self._configure_file_ext()

        # Step 7: Auto-register user component libraries
        self._scan_and_register_user_libs()

        # Step 8: Finalize bus factory
        if self._bus_factory is not None and self._bus is not None:
            self._bus_factory.Init(None, self._bus.ptr)

        self._initialized = True

    # =========================================================================
    # Bootstrap helpers (private)
    # =========================================================================

    def _init_interface_bus(self, runtime_path: Path) -> None:
        """Load `InterfaceBus` component, create bus instance via factory."""
        loaded = self._loader.load_by_cid(CID_EcoInterfaceBus1, [runtime_path])
        self._loaded_libs.append(loaded)
        self._bus_factory = loaded.factory

        bus_ptr = Ptr[Void]()
        result = self._bus_factory.Alloc(None, None, byref(IID_IEcoInterfaceBus1), byref(bus_ptr))
        if result.value != 0 or not bus_ptr.value:
            raise EcoError(result, "Failed to create `InterfaceBus` instance")

        self._bus = IEcoInterfaceBus1(bus_ptr)

    def _register_memory_manager(self, runtime_path: Path) -> None:
        """Load `MemoryManager` component and register its factory with the bus."""
        loaded = self._loader.load_by_cid(CID_EcoMemoryManager1, [runtime_path])
        self._loaded_libs.append(loaded)

        result = self._bus.RegisterComponent(
            byref(CID_EcoMemoryManager1),
            cast(loaded.factory.ptr, Ptr[IEcoUnknown]),
        )
        if result.value != 0:
            raise EcoError(result, "Failed to register `MemoryManager`")

    def _configure_mem_ext(self) -> None:
        """Configure `InterfaceBus` memory extension."""
        mem_ext_ptr = Ptr[Void]()
        result = self._bus.QueryInterface(byref(IID_IEcoInterfaceBus1MemExt), byref(mem_ext_ptr))
        if result.value == 0 and mem_ext_ptr.value:
            mem_ext = IEcoInterfaceBus1MemExt(mem_ext_ptr)
            mem_ext.set_Manager(byref(CID_EcoMemoryManager1))
            mem_ext.set_ExpandPool(Bool(True))
            mem_ext.Release()

    def _init_memory_manager(self) -> None:
        """Query and initialize `MemoryManager` with heap."""
        mgr_ptr = Ptr[Void]()
        result = self._bus.QueryComponent(
            byref(CID_EcoMemoryManager1),
            None,
            byref(IID_IEcoMemoryManager1),
            byref(mgr_ptr),
        )
        if result.value != 0 or not mgr_ptr.value:
            raise EcoError(result, "Failed to get `MemoryManager` interface")

        self._mem_manager = IEcoMemoryManager1(mgr_ptr)
        self._mem_manager.Init(None, self._heap_size)

        # Also get IEcoMemoryAllocator1 for convenience
        alloc_ptr = Ptr[Void]()
        result = self._bus.QueryComponent(
            byref(CID_EcoMemoryManager1),
            None,
            byref(IID_IEcoMemoryAllocator1),
            byref(alloc_ptr),
        )
        if result.value != 0 or not alloc_ptr.value:
            raise EcoError(result, "Failed to get `MemoryAllocator` interface")

        self._mem_allocator = IEcoMemoryAllocator1(alloc_ptr)

    def _register_file_system(self, runtime_path: Path) -> None:
        """Load `FileSystemManagement` component and register with bus."""
        loaded = self._loader.load_by_cid(CID_EcoFileSystemManagement1, [runtime_path])
        self._loaded_libs.append(loaded)

        result = self._bus.RegisterComponent(
            byref(CID_EcoFileSystemManagement1),
            cast(loaded.factory.ptr, Ptr[IEcoUnknown]),
        )
        if result.value != 0:
            raise EcoError(result, "Failed to register `FileSystemManagement`")

    def _configure_file_ext(self) -> None:
        """Configure `InterfaceBus` file extension."""
        file_ext_ptr = Ptr[Void]()
        result = self._bus.QueryInterface(byref(IID_IEcoInterfaceBus1FileExt), byref(file_ext_ptr))
        if result.value == 0 and file_ext_ptr.value:
            file_ext = IEcoInterfaceBus1FileExt(file_ext_ptr)
            file_ext.set_Manager(byref(CID_EcoFileSystemManagement1))
            file_ext.Release()

    def _scan_and_register_user_libs(self) -> None:
        """Scan user paths for EcoOS libraries and register each with the bus.

        Skips libraries whose CID matches a known system component
        since those are already loaded.

        Raises:
            FileNotFoundError: If user library path not found.
            NotADirectoryError: If user library path is not a directory.
        """
        if not self._user_lib_dir.exists():
            raise FileNotFoundError(f"User library path not found: '{self._user_lib_dir}'")

        if not self._user_lib_dir.is_dir():
            raise NotADirectoryError(
                f"User library path is not a directory: '{self._user_lib_dir}'"
            )

        for lib_file in self._user_lib_dir.iterdir():
            if not lib_file.is_file() or not is_eco_lib(lib_file.name):
                continue

            # Derive CID from filename
            cid = lib_filename_to_guid(lib_file.name)
            if cid is None:
                continue

            # Skip system components (already registered)
            if bytes(cid.data) in self._system_cids:
                continue

            loaded = self._loader.load(lib_file)
            self._loaded_libs.append(loaded)

            result = self._bus.RegisterComponent(
                byref(cid),
                cast(loaded.factory.ptr, Ptr[IEcoUnknown]),
            )
            if result.value != 0:
                raise EcoError(result, f"Failed to register component: '{lib_file.name}'")

    # =========================================================================
    # Cleanup
    # =========================================================================

    def release(self) -> None:
        """Release all internal resources."""
        if not self._initialized:
            return

        # Release in reverse initialization order
        for iface in (
            self._mem_allocator,
            self._mem_manager,
            self._bus,
            self._bus_factory,
        ):
            if iface is not None:
                try:
                    iface.Release()
                except Exception:
                    pass

        self._loaded_libs.clear()
        self._initialized = False
        self._bus = None
        self._bus_factory = None
        self._mem_allocator = None
        self._mem_manager = None

    def __repr__(self) -> str:
        status = "initialized" if self._initialized else "not initialized"
        return f"<EcoSystem [{status}]>"
