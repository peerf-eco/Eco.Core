"""EcoSystem container - main entry point for ACOM component interaction.

This module implements the Python equivalent of CEcoSystem1 from EcoOS.
It provides initialization of the runtime environment and access to
the interface bus for component registration and querying.

Architecture Overview:
    EcoSystem acts as a "Unikernel Bridge" that:
    1. Loads runtime DLLs (InterfaceBus, MemoryManager) from ECO_FRAMEWORK_RT
    2. Initializes the EcoOS runtime environment
    3. Provides API for loading and using ACOM components

    The flow for using a component:
    1. Create EcoSystem and call init()
    2. Load component DLL with load_component(cid, dll_path)
    3. Query component interface with query_component(cid, iid, InterfaceClass)
    4. Call methods on the returned ComponentWrapper
    5. Release resources when done

Example:
    >>> from eco_python2acom.runtime.system import EcoSystem
    >>> from eco_python2acom.core.guid import UGUID
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>> from eco_python2acom.core.types import Int16, Int32
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX:
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
    >>>
    >>> CID = UGUID.from_string("4828F655-2E45-40E7-8121-EBD220DC360E")
    >>>
    >>> with EcoSystem() as eco:
    ...     eco.load_component(CID, "calculator.dll")
    ...     calc = eco.query_component(CID, IEcoCalculatorX._iid_, IEcoCalculatorX)
    ...     print(calc.Addition(10, 20))  # 30
    ...     calc.release()
"""

from __future__ import annotations

import ctypes
import os
import platform
from ctypes import POINTER, byref
from pathlib import Path
from typing import Any, Optional, Union

from eco_python2acom.core.errors import EcoError, EcoErrorCode, check_result
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import VoidPtr
from eco_python2acom.interfaces.base import (
    CID_EcoInterfaceBus1,
    CID_EcoMemoryManager1,
    GID_IEcoSystem_x64,
    GID_IEcoSystem_x86,
    IEcoComponentFactory,
    IEcoInterfaceBus1,
    IEcoInterfaceBus1MemExt,
    IEcoMemoryManager1,
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoMemoryManager1,
)

# =============================================================================
# Helper Functions
# =============================================================================


def get_system_gid() -> UGUID:
    """Get the system GID based on current architecture.

    Returns:
        UGUID for the current system architecture (x64 or x86).
    """
    is_64bit = platform.machine().endswith("64") or platform.architecture()[0] == "64bit"
    return GID_IEcoSystem_x64 if is_64bit else GID_IEcoSystem_x86


def guid_to_filename(guid: UGUID, extension: str = ".dll") -> str:
    """Convert UGUID to hex filename format used by EcoOS.

    Args:
        guid: The UGUID to convert.
        extension: File extension (default: .dll for Windows).

    Returns:
        Filename like "00000000000000000000000042757331.dll"
    """
    hex_str = "".join(f"{b:02X}" for b in guid.Data)
    return hex_str + extension


# =============================================================================
# EcoSystem Class
# =============================================================================


class EcoSystem:
    """EcoOS System container - Unikernel Bridge for Python.

    This class provides the main entry point for interacting with ACOM components.
    It initializes the EcoOS runtime environment including:
    - IEcoInterfaceBus1 for component registration and querying
    - IEcoMemoryManager1 for memory management

    The class can be used as a context manager for automatic resource cleanup.

    Attributes:
        is_initialized: Whether the system has been initialized.
        runtime_path: Path to ECO_FRAMEWORK_RT directory.

    Example:
        >>> with EcoSystem() as eco:
        ...     eco.load_component(cid, dll_path)
        ...     calc = eco.query_component(cid, iid, IEcoCalculatorX)
        ...     print(calc.Addition(10, 20))
    """

    def __init__(
        self,
        eco_framework_rt: Optional[str] = None,
        auto_init: bool = False,
    ) -> None:
        """Initialize EcoSystem container.

        Args:
            eco_framework_rt: Path to ECO_FRAMEWORK_RT directory.
                If None, uses ECO_FRAMEWORK_RT environment variable.
            auto_init: If True, automatically initialize during construction.
        """
        self._eco_framework_rt = eco_framework_rt or os.environ.get("ECO_FRAMEWORK_RT", "")
        self._is_initialized = False

        # Runtime components
        self._bus_dll: Optional[ctypes.WinDLL] = None
        self._bus_factory_ptr: VoidPtr = VoidPtr()
        self._bus_factory: Optional[IEcoComponentFactory] = None
        self._bus_ptr: VoidPtr = VoidPtr()
        self._bus: Optional[IEcoInterfaceBus1] = None

        self._mem_dll: Optional[ctypes.WinDLL] = None
        self._mem_factory_ptr: VoidPtr = VoidPtr()
        self._mem_mgr_ptr: VoidPtr = VoidPtr()
        self._mem_mgr: Optional[IEcoMemoryManager1] = None

        self._mem_ext_ptr: VoidPtr = VoidPtr()
        self._mem_ext: Optional[IEcoInterfaceBus1MemExt] = None

        # Loaded component DLLs cache
        self._loaded_dlls: dict[str, ctypes.WinDLL] = {}

        # System GID for current architecture
        self._system_gid = get_system_gid()

        if auto_init:
            self.init()

    @property
    def is_initialized(self) -> bool:
        """Check if the system is initialized."""
        return self._is_initialized

    @property
    def runtime_path(self) -> str:
        """Get the ECO_FRAMEWORK_RT path."""
        return self._eco_framework_rt or ""

    def __enter__(self) -> EcoSystem:
        """Enter context manager - initialize system."""
        if not self._is_initialized:
            self.init()
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Exit context manager - release resources."""
        self.release()

    def get_runtime_path(self) -> Path:
        """Get the runtime path for current architecture.

        Returns:
            Path to runtime directory containing DLLs.

        Raises:
            FileNotFoundError: If ECO_FRAMEWORK_RT is not set or doesn't exist.
        """
        if not self._eco_framework_rt:
            raise FileNotFoundError(
                "ECO_FRAMEWORK_RT environment variable is not set. "
                "Please set it to the path containing EcoOS runtime libraries."
            )

        base_path = Path(self._eco_framework_rt)
        if not base_path.exists():
            raise FileNotFoundError(f"ECO_FRAMEWORK_RT path does not exist: {base_path}")

        # Runtime DLLs are in subdirectory named by system GID
        runtime_path = base_path / guid_to_filename(self._system_gid, "")
        if not runtime_path.exists():
            # Try without subdirectory (flat structure)
            runtime_path = base_path

        return runtime_path

    def load_dll(self, cid: UGUID, search_path: Optional[Path] = None) -> ctypes.WinDLL:
        """Load a component DLL by its CID.

        Args:
            cid: Component ID (CID) of the component.
            search_path: Optional path to search for DLL.

        Returns:
            Loaded WinDLL instance.

        Raises:
            FileNotFoundError: If DLL cannot be found.
        """
        dll_name = guid_to_filename(cid)
        dll_key = dll_name.lower()

        # Check cache
        if dll_key in self._loaded_dlls:
            return self._loaded_dlls[dll_key]

        # Build search paths
        search_paths: list[Path] = []
        if search_path:
            search_paths.append(search_path)

        try:
            runtime_path = self.get_runtime_path()
            search_paths.append(runtime_path)
        except FileNotFoundError:
            pass

        # Try to load from search paths
        for path in search_paths:
            dll_path = path / dll_name
            if dll_path.exists():
                dll = ctypes.WinDLL(str(dll_path))
                self._loaded_dlls[dll_key] = dll
                return dll

        # Try system path
        try:
            dll = ctypes.WinDLL(dll_name)
            self._loaded_dlls[dll_key] = dll
            return dll
        except OSError as e:
            raise FileNotFoundError(
                f"Cannot find component DLL: {dll_name}. "
                f"Searched in: {[str(p) for p in search_paths]}. "
                f"Error: {e}"
            ) from e

    def get_factory_ptr(self, dll: ctypes.WinDLL) -> VoidPtr:
        """Get IEcoComponentFactory pointer from DLL.

        Args:
            dll: Loaded DLL instance.

        Returns:
            Pointer to component factory.

        Raises:
            EcoError: If factory function cannot be found.
        """
        try:
            get_factory = dll.GetIEcoComponentFactoryPtr
            get_factory.restype = VoidPtr
            get_factory.argtypes = []
            result: VoidPtr = get_factory()
            return result
        except AttributeError as err:
            raise EcoError(
                EcoErrorCode.COMPONENT_NOTFOUND,
                "DLL does not export GetIEcoComponentFactoryPtr",
            ) from err

    def init(self) -> None:
        """Initialize the EcoOS runtime environment.

        This method performs the following steps:
        1. Loads InterfaceBus DLL and creates IEcoInterfaceBus1 instance
        2. Loads and registers MemoryManager
        3. Configures memory extension for dynamic allocation
        4. Initializes the bus with system context

        Raises:
            FileNotFoundError: If runtime DLLs cannot be found.
            EcoError: If initialization fails.
        """
        if self._is_initialized:
            return

        # Step 1: Load InterfaceBus DLL
        self._bus_dll = self.load_dll(CID_EcoInterfaceBus1)
        self._bus_factory_ptr = self.get_factory_ptr(self._bus_dll)

        bus_factory = ctypes.cast(self._bus_factory_ptr, POINTER(IEcoComponentFactory))
        self._bus_factory = bus_factory.contents

        # Step 2: Create InterfaceBus instance
        result = self._bus_factory.pVTbl.contents.Alloc(
            self._bus_factory_ptr,
            None,  # pISystem = NULL (we create the system)
            None,  # pIUnknownOuter = NULL (no aggregation)
            byref(IID_IEcoInterfaceBus1),
            byref(self._bus_ptr),
        )

        if result != 0 or not self._bus_ptr.value:
            raise EcoError(result, "Failed to create InterfaceBus instance")

        self._bus = ctypes.cast(self._bus_ptr, POINTER(IEcoInterfaceBus1)).contents

        # Step 3: Load and register MemoryManager
        self._mem_dll = self.load_dll(CID_EcoMemoryManager1)
        self._mem_factory_ptr = self.get_factory_ptr(self._mem_dll)

        result = self._bus.pVTbl.contents.RegisterComponent(
            self._bus_ptr,
            byref(CID_EcoMemoryManager1),
            self._mem_factory_ptr,
        )
        if result != 0:
            raise EcoError(result, "Failed to register MemoryManager")

        # Step 4: Get and configure memory extension
        result = self._bus.pVTbl.contents.QueryInterface(
            self._bus_ptr,
            byref(IID_IEcoInterfaceBus1MemExt),
            byref(self._mem_ext_ptr),
        )

        if result == 0 and self._mem_ext_ptr.value:
            self._mem_ext = ctypes.cast(
                self._mem_ext_ptr, POINTER(IEcoInterfaceBus1MemExt)
            ).contents

            # Set memory manager and enable pool expansion
            self._mem_ext.pVTbl.contents.set_Manager(
                self._mem_ext_ptr, byref(CID_EcoMemoryManager1)
            )
            self._mem_ext.pVTbl.contents.set_ExpandPool(self._mem_ext_ptr, 1)

        # Step 5: Get MemoryManager and initialize
        result = self._bus.pVTbl.contents.QueryComponent(
            self._bus_ptr,
            byref(CID_EcoMemoryManager1),
            None,
            byref(IID_IEcoMemoryManager1),
            byref(self._mem_mgr_ptr),
        )

        if result == 0 and self._mem_mgr_ptr.value:
            self._mem_mgr = ctypes.cast(self._mem_mgr_ptr, POINTER(IEcoMemoryManager1)).contents
            # Initialize with 1.5GB heap (0x60000000 bytes)
            self._mem_mgr.pVTbl.contents.Init(self._mem_mgr_ptr, None, 0x60000000)

        # Step 6: Initialize Bus with factory
        self._bus_factory.pVTbl.contents.Init(
            self._bus_factory_ptr,
            None,
            self._bus_ptr,
        )

        self._is_initialized = True

    def release(self) -> None:
        """Release all resources and cleanup.

        This method releases all ACOM interfaces and clears DLL cache.
        Safe to call multiple times.
        """
        if not self._is_initialized:
            return

        # Release in reverse order of acquisition
        if self._mem_mgr and self._mem_mgr_ptr.value:
            self._mem_mgr.pVTbl.contents.Release(self._mem_mgr_ptr)
            self._mem_mgr = None
            self._mem_mgr_ptr = VoidPtr()

        if self._mem_ext and self._mem_ext_ptr.value:
            self._mem_ext.pVTbl.contents.Release(self._mem_ext_ptr)
            self._mem_ext = None
            self._mem_ext_ptr = VoidPtr()

        if self._bus and self._bus_ptr.value:
            self._bus.pVTbl.contents.Release(self._bus_ptr)
            self._bus = None
            self._bus_ptr = VoidPtr()

        self._loaded_dlls.clear()
        self._is_initialized = False

    def register_component(
        self,
        cid: Union[UGUID, str],
        factory_ptr: VoidPtr,
    ) -> None:
        """Register a component with the interface bus.

        Args:
            cid: Component ID (CID) - UGUID or string like "4828F655-2E45-40E7-..."
            factory_ptr: Pointer to component factory (from GetIEcoComponentFactoryPtr).

        Raises:
            EcoError: If registration fails.
            RuntimeError: If system is not initialized.
        """
        if not self._is_initialized or not self._bus:
            raise RuntimeError("EcoSystem is not initialized. Call init() first.")

        if isinstance(cid, str):
            cid = UGUID.from_string(cid)

        result = self._bus.pVTbl.contents.RegisterComponent(
            self._bus_ptr,
            byref(cid),
            factory_ptr,
        )
        check_result(result, "RegisterComponent")

    def load_component(
        self,
        cid: Union[UGUID, str],
        dll_path: Union[str, Path],
    ) -> VoidPtr:
        """Load a component DLL and register it with the interface bus.

        This is the main method for loading user components.

        Args:
            cid: Component ID (CID) for registration.
            dll_path: Path to the component DLL file.

        Returns:
            Pointer to the component factory.

        Raises:
            FileNotFoundError: If DLL cannot be found.
            EcoError: If registration fails.
            RuntimeError: If system is not initialized.

        Example:
            >>> cid = UGUID.from_string("4828F655-2E45-40E7-8121-EBD220DC360E")
            >>> eco.load_component(cid, "path/to/component.dll")
        """
        if not self._is_initialized:
            raise RuntimeError("EcoSystem is not initialized. Call init() first.")

        dll_path = Path(dll_path)

        # Resolve relative path
        if not dll_path.is_absolute():
            for search_path in [Path.cwd(), self.get_runtime_path()]:
                full_path = search_path / dll_path
                if full_path.exists():
                    dll_path = full_path
                    break

        if not dll_path.exists():
            raise FileNotFoundError(f"Component DLL not found: {dll_path}")

        # Load DLL
        dll = ctypes.WinDLL(str(dll_path))
        self._loaded_dlls[dll_path.name.lower()] = dll

        # Get factory
        factory_ptr = self.get_factory_ptr(dll)

        # Register
        self.register_component(cid, factory_ptr)

        return factory_ptr

    def query_component(
        self,
        cid: Union[UGUID, str],
        iid: Union[UGUID, str],
        interface_class: Optional[type[Any]] = None,
    ) -> Any:
        """Create a component instance and return interface pointer.

        This method:
        1. Creates a NEW instance of the component identified by CID
        2. Returns a POINTER to the interface identified by IID

        The returned interface pointer can be used to:
        - Call methods of that interface
        - Query other interfaces via QueryInterface (query_interface method)

        Note:
            Each call creates a NEW component instance. The component may
            implement multiple interfaces (e.g., IEcoCalculatorX and IEcoCalculatorY).
            Use query_interface() to get other interfaces from the same instance.

        Args:
            cid: Component ID (CID) - identifies WHICH component to create.
            iid: Interface ID (IID) - identifies WHICH interface to return.
            interface_class: Optional Python class decorated with @interface.
                If provided, returns a ComponentWrapper for convenient method calls.

        Returns:
            If interface_class is provided: ComponentWrapper wrapping the interface.
            Otherwise: Raw VoidPtr pointer to the interface.

        Raises:
            EcoError: If component creation or interface query fails.
            RuntimeError: If system is not initialized.

        Example:
            >>> # Create component and get IEcoCalculatorX interface
            >>> calc_x = eco.query_component(cid, IEcoCalculatorX._iid_, IEcoCalculatorX)
            >>> print(calc_x.Addition(10, 20))  # 30
            >>>
            >>> # Get another interface from the SAME component instance
            >>> calc_y = calc_x.query_interface(IEcoCalculatorY)
            >>> print(calc_y.Multiplication(6, 7))  # 42
        """
        if not self._is_initialized or not self._bus:
            raise RuntimeError("EcoSystem is not initialized. Call init() first.")

        if isinstance(cid, str):
            cid = UGUID.from_string(cid)
        if isinstance(iid, str):
            iid = UGUID.from_string(iid)

        # Output: pointer to the requested interface
        interface_ptr = VoidPtr()
        result = self._bus.pVTbl.contents.QueryComponent(
            self._bus_ptr,
            byref(cid),
            None,  # pIUnknownOuter (no aggregation)
            byref(iid),
            byref(interface_ptr),
        )

        check_result(result, "QueryComponent")

        if interface_class is not None:
            from eco_python2acom.client.component import ComponentWrapper

            return ComponentWrapper(interface_ptr, interface_class)

        return interface_ptr

    def get_bus_ptr(self) -> VoidPtr:
        """Get the raw InterfaceBus pointer.

        Returns:
            Pointer to IEcoInterfaceBus1 interface.

        Raises:
            RuntimeError: If system is not initialized.
        """
        if not self._is_initialized:
            raise RuntimeError("EcoSystem is not initialized. Call init() first.")
        return self._bus_ptr

    def __repr__(self) -> str:
        """Return string representation."""
        status = "initialized" if self._is_initialized else "not initialized"
        return f"EcoSystem(status={status}, runtime_path={self._eco_framework_rt!r})"
