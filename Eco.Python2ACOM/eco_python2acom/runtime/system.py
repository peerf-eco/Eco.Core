"""EcoSystem - main entry point for ACOM component interaction.

This module implements the Python equivalent of `CEcoSystem1` from EcoOS.
It provides initialization of the runtime environment and access to
the interface bus for component registration and querying.

The bootstrap sequence mirrors `createCEcoSystem1` from C:
    1. Load `InterfaceBus` library -> create instance via factory `Alloc`
    2. Load `MemoryManager` library -> register factory with bus
    3. Configure `MemExt` -> set manager CID, enable pool
    4. Query component `MemoryManager` -> initialize heap
    5. Query interface `MemoryAllocator` from `MemoryManager`
    6. Load `FileSystemManagement` library -> register factory with bus (optional)
    7. Configure `FileExt` (optional)
    8. Scan user paths for component shared libraries -> register each
    9. Init bus factory -> finalize bus setup

Example:
    >>> with EcoSystem() as eco:
    ...     ppv = Ptr[Void]()
    ...     bus = eco.bus
    ...     bus.obj.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
"""

import logging
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
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.interface_bus import (
    IEcoInterfaceBus1,
    IEcoInterfaceBus1FileExt,
    IEcoInterfaceBus1MemExt,
)
from eco_python2acom.interfaces.memory_manager import IEcoMemoryAllocator1, IEcoMemoryManager1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.runtime.loader import EcoLib, EcoLibLoader
from eco_python2acom.runtime.logging import eco_logger
from eco_python2acom.runtime.utils import is_eco_lib, lib_filename_to_guid
from eco_python2acom.types.core import Void
from eco_python2acom.types.errors import EcoError
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast


class EcoSystem:
    """EcoOS System container -- Unikernel Bridge for Python.

    Bootstraps the ACOM runtime (`InterfaceBus`, `MemoryManager`,
    optionally `FileSystemManagement`), auto-registers user component
    libraries.

    Example:
        ```python
        with EcoSystem() as eco:
            ppv = Ptr[Void]()
            bus = eco.bus
            bus.obj.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
        ```

    Attributes:
        initialized: Whether the system has been initialized.
        bus: Pointer to the initialized `IEcoInterfaceBus1`.
        mem_manager: Pointer to the initialized `IEcoMemoryManager1`.
        mem_allocator: Pointer to the initialized `IEcoMemoryAllocator1`.
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
        logger: Optional[logging.Logger] = None,
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
            logger: Logger used to report bootstrap and release events.
                Defaults to the package-wide `system` child logger.
        """
        self._runtime_path = runtime_path or os.environ.get("ECO_FRAMEWORK_RT", None)
        self._user_lib_dir = Path(user_lib_dir) if user_lib_dir else Path.cwd()
        self._gid = gid or GID_IEcoSystem
        self.logger = logger if logger is not None else eco_logger.getChild(type(self).__name__)

        # Initialize internal references (set during init, released during cleanup)
        self._bus_factory: Ptr[IEcoComponentFactory] = Ptr[IEcoComponentFactory]()
        self._bus: Ptr[IEcoInterfaceBus1] = Ptr[IEcoInterfaceBus1]()
        self._mem_manager: Ptr[IEcoMemoryManager1] = Ptr[IEcoMemoryManager1]()
        self._mem_allocator: Ptr[IEcoMemoryAllocator1] = Ptr[IEcoMemoryAllocator1]()
        self._system_cids = frozenset(
            bytes(cid.data)
            for cid in (CID_EcoInterfaceBus1, CID_EcoMemoryManager1, CID_EcoFileSystemManagement1)
        )

        # Keep loaded libraries alive
        self._loader = EcoLibLoader(logger=logger)
        self._loaded_libs: list[EcoLib] = []

        # Initialize state
        self._heap_size = heap_size
        self._initialized = False

    # =========================================================================
    # Properties
    # =========================================================================

    @property
    def initialized(self) -> bool:
        """Whether the system has been initialized."""
        return self._initialized

    @property
    def bus(self) -> Ptr[IEcoInterfaceBus1]:
        """Pointer to the initialized interface bus."""
        if not self._initialized:
            raise RuntimeError("Access to 'bus' requires initialized EcoSystem")
        return self._bus

    @property
    def mem_manager(self) -> Ptr[IEcoMemoryManager1]:
        """Pointer to the initialized memory manager."""
        if not self._initialized:
            raise RuntimeError("Access to 'mem_manager' requires initialized EcoSystem")
        return self._mem_manager

    @property
    def mem_allocator(self) -> Ptr[IEcoMemoryAllocator1]:
        """Pointer to the initialized memory allocator."""
        if not self._initialized:
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
            self.logger.error("Runtime path not configured")
            raise FileNotFoundError("Runtime path not configured")

        self._full_runtime_path = Path(self._runtime_path) / self._gid.to_string(with_hyphens=False)
        if not self._full_runtime_path.exists():
            self.logger.error("Runtime path not found ---> '%s'", self._full_runtime_path)
            raise FileNotFoundError(f"Runtime path not found: '{self._full_runtime_path}'")

        self.logger.debug("Bootstrap starting | GID = <%s>", self._gid)

        # Step 1: Load InterfaceBus and create instance
        self._init_interface_bus(self._full_runtime_path)

        # Step 2: Register MemoryManager factory with bus
        self._register_memory_manager(self._full_runtime_path)

        # Step 3: Configure MemExt on the bus
        self._configure_mem_ext()

        # Step 4: Initialize MemoryManager heap
        self._init_memory_manager()

        # Step 5: Query MemoryAllocator interface
        self._init_memory_allocator()

        # Step 6: Register FileSystemManagement factory
        self._register_file_system(self._full_runtime_path)

        # Step 7: Configure FileExt on the bus
        self._configure_file_ext()

        # Step 8: Auto-register user component libraries
        self._scan_and_register_user_libs()

        # Step 9: Finalize bus factory
        if bool(self._bus_factory) and bool(self._bus):
            self.logger.debug("Step [9/9] ---> Finalizing 'InterfaceBus' factory")
            factory = self._bus_factory
            factory.obj.Init(None, cast(self._bus, Ptr[Void]))
            self.logger.debug("Step [9/9] ---> 'InterfaceBus' factory ---> OK")
        else:
            self.logger.debug("Step [9/9] ---> 'InterfaceBus' factory not available ---> Skipped")

        self._initialized = True
        self.logger.debug("Bootstrap complete ---> OK")

    # =========================================================================
    # Bootstrap helpers (private)
    # =========================================================================

    def _init_interface_bus(self, runtime_path: Path) -> None:
        """Load `InterfaceBus` component, create bus instance via factory."""
        self.logger.debug("Step [1/9] ---> Loading 'InterfaceBus'")
        loaded = self._loader.load_by_cid(CID_EcoInterfaceBus1, [runtime_path])
        self._loaded_libs.append(loaded)
        self._bus_factory = loaded.factory

        bus_ptr = Ptr[Void]()
        result = self._bus_factory.obj.Alloc(
            None, None, byref(IID_IEcoInterfaceBus1), byref(bus_ptr)
        )
        if result != 0 or not bool(bus_ptr):
            self.logger.error(
                "Step [1/9] ---> 'InterfaceBus' allocation failed | code = 0x%X", result
            )
            raise EcoError(result, "Failed to create `InterfaceBus` instance")

        self._bus = cast(bus_ptr, Ptr[IEcoInterfaceBus1])
        self.logger.debug("Step [1/9] ---> 'InterfaceBus' ---> OK")

    def _register_memory_manager(self, runtime_path: Path) -> None:
        """Load `MemoryManager` component and register its factory with the bus."""
        self.logger.debug("Step [2/9] ---> Registering 'MemoryManager'")
        loaded = self._loader.load_by_cid(CID_EcoMemoryManager1, [runtime_path])
        self._loaded_libs.append(loaded)

        result = self._bus.obj.RegisterComponent(
            byref(CID_EcoMemoryManager1), cast(loaded.factory, Ptr[IEcoUnknown])
        )
        if result != 0:
            self.logger.error(
                "Step [2/9] ---> 'MemoryManager' register failed | code = 0x%X", result
            )
            raise EcoError(result, "Failed to register `MemoryManager`")
        self.logger.debug("Step [2/9] ---> 'MemoryManager' ---> OK")

    def _configure_mem_ext(self) -> None:
        """Configure `InterfaceBus` memory extension."""
        self.logger.debug("Step [3/9] ---> Configuring 'MemExt'")
        mem_ext_ptr = Ptr[Void]()
        result = self._bus.obj.QueryInterface(
            byref(IID_IEcoInterfaceBus1MemExt), byref(mem_ext_ptr)
        )
        if result == 0 and bool(mem_ext_ptr):
            mem_ext = cast(mem_ext_ptr, Ptr[IEcoInterfaceBus1MemExt])
            mem_ext.obj.set_Manager(byref(CID_EcoMemoryManager1))
            mem_ext.obj.set_ExpandPool(True)
            mem_ext.obj.Release()
            self.logger.debug("Step [3/9] ---> 'MemExt' ---> OK")
        else:
            self.logger.debug("Step [3/9] ---> 'MemExt' not available ---> Skipped")

    def _init_memory_manager(self) -> None:
        """Query and initialize `MemoryManager` with heap."""
        self.logger.debug(
            "Step [4/9] ---> Initializing 'MemoryManager' | heap = %d bytes", self._heap_size
        )
        mgr_ptr = Ptr[Void]()
        result = self._bus.obj.QueryComponent(
            byref(CID_EcoMemoryManager1),
            None,
            byref(IID_IEcoMemoryManager1),
            byref(mgr_ptr),
        )
        if result != 0 or not bool(mgr_ptr):
            self.logger.error("Step [4/9] ---> 'MemoryManager' query failed | code = 0x%X", result)
            raise EcoError(result, "Failed to get `MemoryManager` interface")

        self._mem_manager = cast(mgr_ptr, Ptr[IEcoMemoryManager1])
        self._mem_manager.obj.Init(None, self._heap_size)
        self.logger.debug("Step [4/9] ---> 'MemoryManager' ---> OK")

    def _init_memory_allocator(self) -> None:
        """Query the `MemoryAllocator` interface from `MemoryManager`."""
        self.logger.debug("Step [5/9] ---> Initializing 'MemoryAllocator'")
        alloc_ptr = Ptr[Void]()
        result = self._mem_manager.obj.QueryInterface(
            byref(IID_IEcoMemoryAllocator1), byref(alloc_ptr)
        )
        if result != 0 or not bool(alloc_ptr):
            self.logger.error(
                "Step [5/9] ---> 'MemoryAllocator' query failed | code = 0x%X", result
            )
            raise EcoError(result, "Failed to get `MemoryAllocator` interface")

        self._mem_allocator = cast(alloc_ptr, Ptr[IEcoMemoryAllocator1])
        self.logger.debug("Step [5/9] ---> 'MemoryAllocator' ---> OK")

    def _register_file_system(self, runtime_path: Path) -> None:
        """Load `FileSystemManagement` component and register with bus."""
        self.logger.debug("Step [6/9] ---> Registering 'FileSystemManagement'")
        loaded = self._loader.load_by_cid(CID_EcoFileSystemManagement1, [runtime_path])
        self._loaded_libs.append(loaded)

        result = self._bus.obj.RegisterComponent(
            byref(CID_EcoFileSystemManagement1), cast(loaded.factory, Ptr[IEcoUnknown])
        )
        if result != 0:
            self.logger.error(
                "Step [6/9] ---> 'FileSystemManagement' register failed | code = 0x%X",
                result,
            )
            raise EcoError(result, "Failed to register `FileSystemManagement`")
        self.logger.debug("Step [6/9] ---> 'FileSystemManagement' ---> OK")

    def _configure_file_ext(self) -> None:
        """Configure `InterfaceBus` file extension."""
        self.logger.debug("Step [7/9] ---> Configuring 'FileExt'")
        file_ext_ptr = Ptr[Void]()
        result = self._bus.obj.QueryInterface(
            byref(IID_IEcoInterfaceBus1FileExt),
            byref(file_ext_ptr),
        )
        if result == 0 and bool(file_ext_ptr):
            file_ext = cast(file_ext_ptr, Ptr[IEcoInterfaceBus1FileExt])
            file_ext.obj.set_Manager(byref(CID_EcoFileSystemManagement1))
            file_ext.obj.Release()
            self.logger.debug("Step [7/9] ---> 'FileExt' ---> OK")
        else:
            self.logger.debug("Step [7/9] ---> 'FileExt' not available ---> Skipped")

    def _scan_and_register_user_libs(self) -> None:
        """Scan user paths for EcoOS libraries and register each with the bus.

        Skips libraries whose CID matches a known system component
        since those are already loaded.

        Raises:
            FileNotFoundError: If user library path not found.
            NotADirectoryError: If user library path is not a directory.
        """
        self.logger.debug("Step [8/9] ---> Scanning user libs in '%s'", self._user_lib_dir)

        if not self._user_lib_dir.exists():
            self.logger.error(
                "Step [8/9] ---> User lib path not found ---> '%s'", self._user_lib_dir
            )
            raise FileNotFoundError(f"User library path not found: '{self._user_lib_dir}'")

        if not self._user_lib_dir.is_dir():
            self.logger.error(
                "Step [8/9] ---> User lib path is not a directory ---> '%s'",
                self._user_lib_dir,
            )
            raise NotADirectoryError(
                f"User library path is not a directory: '{self._user_lib_dir}'"
            )

        registered = 0
        for lib_file in self._user_lib_dir.iterdir():
            if not lib_file.is_file() or not is_eco_lib(lib_file.name):
                continue

            # Derive CID from filename
            cid = lib_filename_to_guid(lib_file.name)
            if cid is None:
                continue

            # Skip system components (already registered)
            if bytes(cid.data) in self._system_cids:
                self.logger.debug(
                    "Step [8/9] ---> Skipping system component ---> '%s'", lib_file.name
                )
                continue

            loaded = self._loader.load(lib_file)
            self._loaded_libs.append(loaded)

            result = self._bus.obj.RegisterComponent(
                byref(cid),
                cast(loaded.factory, Ptr[IEcoUnknown]),
            )
            if result != 0:
                self.logger.error(
                    "Step [8/9] ---> Register failed ---> '%s' | code = 0x%X", lib_file.name, result
                )
                raise EcoError(result, f"Failed to register component: '{lib_file.name}'")
            registered += 1
            self.logger.debug("Step [8/9] ---> Registered ---> '%s'", lib_file.name)

        self.logger.debug("Step [8/9] ---> Scan complete | registered = %d", registered)

    # =========================================================================
    # Cleanup
    # =========================================================================

    def release(self) -> None:
        """Release all internal resources."""
        if not self._initialized:
            return

        self.logger.debug("Releasing resources")

        # Release in reverse initialization order
        for iface_ptr in (self._mem_allocator, self._mem_manager, self._bus, self._bus_factory):
            try:
                iface_ptr.obj.Release()
            except Exception as err:
                self.logger.debug("Ignoring error ---> %s", err)

        self._loaded_libs.clear()
        self._initialized = False
        self.logger.debug("OK")

    def __repr__(self) -> str:
        status = "initialized" if self._initialized else "not initialized"
        return f"<EcoSystem [{status}]>"
