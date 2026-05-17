# mypy: ignore-errors
"""EcoSystem — root ACOM component that boots the EcoOS runtime.

Python equivalent of `CEcoSystem1` from `Eco.System1`. Loads core system libraries
(`InterfaceBus`, `MemoryManager`, `FileSystemManagement`), wires their extensions on
the bus, scans a directory for user libraries, and finalizes the bus factory.
Exposes a family of system interfaces via views.

Classes:
    `EcoSystem`: Root system component, lifecycle owner and view host.
"""

import os
import sys
from pathlib import Path
from typing import Any, ClassVar, Optional, Self, Union

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
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
    IID_IEcoMemoryManager1,
)
from eco_python2acom.interfaces.interface_bus import (
    IEcoInterfaceBus1,
    IEcoInterfaceBus1FileExt,
    IEcoInterfaceBus1MemExt,
)
from eco_python2acom.interfaces.memory_manager import IEcoMemoryManager1
from eco_python2acom.interfaces.system import (
    IEcoCommandArguments1,
    IEcoSystem1,
    IEcoSystemInformation1,
)
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.runtime.loader import EcoLib, EcoLibLoader
from eco_python2acom.runtime.platform import get_computer_name, get_hw_profile_id
from eco_python2acom.runtime.utils import is_eco_lib, lib_filename_to_guid
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import Bool, CString, Int16, UInt32, Void
from eco_python2acom.types.errors import EcoError, EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import byref, cast


@component()
class EcoSystem:
    """Root ACOM component for the EcoOS runtime.

    Owns the interface bus together with its memory and file extensions,
    keeps the loaded system and user libraries alive, and exposes a family
    of system interfaces (`IEcoSystem1`, `IEcoSystemInformation1`,
    `IEcoCommandArguments1`, `IEcoInterfaceBus1`, `IEcoInterfaceBus1MemExt`,
    `IEcoInterfaceBus1FileExt`) through dedicated views.

    Attributes:
        argc: Number of command-line arguments.
        argv: Pointer to the command-line argument vector.
        data: Arbitrary user data pointer.
        name: Cached machine name.
        id: Cached hardware profile GUID.
        path: Cached executable directory prefix.
        bus: Pointer to the loaded `IEcoInterfaceBus1` instance.
        mem_manager: Pointer to the `IEcoMemoryManager1` interface.
        mem_ext: Pointer to the `IEcoInterfaceBus1MemExt` interface, may be NULL.
        file_ext: Pointer to the `IEcoInterfaceBus1FileExt` interface, may be NULL.
    """

    # Command-line arguments
    argc: Int16
    argv: Ptr[CString]

    # User-settable opaque data slot exposed through `IEcoSystem1`
    data: Ptr[Void]

    # Lazy caches for `IEcoSystemInformation1` and `IEcoCommandArguments1`
    name: CString
    id: CString
    path: CString

    # Core system interfaces resolved during bootstrap
    bus: Ptr[IEcoInterfaceBus1]
    mem_manager: Ptr[IEcoMemoryManager1]
    mem_ext: Ptr[IEcoInterfaceBus1MemExt]
    file_ext: Ptr[IEcoInterfaceBus1FileExt]

    # Default heap size for `MemoryManager` (~1.5 GB)
    DEFAULT_HEAP_SIZE: ClassVar[int] = 0x60000000

    # System component CIDs are auto-loaded by `EcoSystem`
    SYSTEM_CIDS: ClassVar[frozenset[bytes]] = frozenset(
        bytes(cid.data)
        for cid in (
            CID_EcoInterfaceBus1,
            CID_EcoMemoryManager1,
            CID_EcoFileSystemManagement1,
        )
    )

    # =========================================================================
    # Views
    # =========================================================================

    @view
    class SystemView(IEcoSystem1):
        """`IEcoSystem1` view — opaque user data slot."""

        def set_Data(self, data: Ptr[Void]) -> Void:
            """Store the user data pointer."""
            self.data = data

        def get_Data(self) -> Ptr[Void]:
            """Return the previously stored user data pointer."""
            return self.data

    @view
    class SystemInformationView(IEcoSystemInformation1):
        """`IEcoSystemInformation1` view — local machine identity."""

        def get_Name(self) -> CString:
            """Return the local machine name, cached after the first call."""
            if not self.name:
                self.name = get_computer_name()
            return self.name

        def get_Id(self) -> CString:
            """Return the hardware profile GUID, cached after the first call."""
            if not self.id:
                self.id = get_hw_profile_id()
            return self.id

    @view
    class CommandArgumentsView(IEcoCommandArguments1):
        """`IEcoCommandArguments1` view — exposes command line arguments."""

        def get_Count(self) -> Int16:
            """Return the number of command-line arguments."""
            return self.argc

        def get_Args(self) -> Ptr[CString]:
            """Return the command-line argument vector."""
            return self.argv

        def get_Path(self) -> CString:
            """Return the directory prefix of the first argument, cached on first call."""
            if not bool(self.path) and self.argc > 0:
                head, _, _ = self.argv[0].rpartition(b"\\")
                self.path = head
            return self.path

        def get_Arg(self, index: Int16) -> CString:
            """Return a single command-line argument by index."""
            if 0 <= index < self.argc:
                return self.argv[index]
            return b""

    @view
    class InterfaceBusView(IEcoInterfaceBus1):
        """`IEcoInterfaceBus1` view — forwards every call to the inner bus."""

        def Init(self) -> Int16:
            """Forward to the inner bus."""
            return self.bus.obj.Init()

        def InitWith(self, heap: Ptr[Void], size: UInt32) -> Int16:
            """Forward to the inner bus."""
            return self.bus.obj.InitWith(heap, size)

        def RegisterComponent(self, cid: Ptr[UGUID], factory: Ptr[IEcoUnknown]) -> Int16:
            """Forward to the inner bus."""
            return self.bus.obj.RegisterComponent(cid, factory)

        def UnRegisterComponent(self, cid: Ptr[UGUID]) -> Int16:
            """Forward to the inner bus."""
            return self.bus.obj.UnRegisterComponent(cid)

        def QueryComponent(
            self,
            cid: Ptr[UGUID],
            outer: Optional[Ptr[IEcoUnknown]],
            iid: Ptr[UGUID],
            out: Ptr[Ptr[Void]],
        ) -> Int16:
            """Forward to the inner bus."""
            return self.bus.obj.QueryComponent(cid, outer, iid, out)

    @view
    class MemoryExtensionView(IEcoInterfaceBus1MemExt):
        """`IEcoInterfaceBus1MemExt` view — forwards to the bus memory extension."""

        def set_Manager(self, cid: Ptr[UGUID]) -> Int16:
            """Forward to the inner memory extension."""
            if not bool(self.mem_ext):
                return EcoErrorCode.POINTER
            return self.mem_ext.obj.set_Manager(cid)

        def get_Manager(self) -> Ptr[UGUID]:
            """Forward to the inner memory extension."""
            if not bool(self.mem_ext):
                return Ptr[UGUID]()
            return self.mem_ext.obj.get_Manager()

        def set_ExpandPool(self, enable: Bool) -> Int16:
            """Forward to the inner memory extension."""
            if not bool(self.mem_ext):
                return EcoErrorCode.POINTER
            return self.mem_ext.obj.set_ExpandPool(enable)

    @view
    class FileExtensionView(IEcoInterfaceBus1FileExt):
        """`IEcoInterfaceBus1FileExt` view — forwards to the bus file extension."""

        def set_Manager(self, cid: Ptr[UGUID]) -> Int16:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return EcoErrorCode.POINTER
            return self.file_ext.obj.set_Manager(cid)

        def get_Manager(self) -> Ptr[UGUID]:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return Ptr[UGUID]()
            return self.file_ext.obj.get_Manager()

        def set_SearchPath(self, path: CString) -> Int16:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return EcoErrorCode.POINTER
            return self.file_ext.obj.set_SearchPath(path)

        def get_SearchPath(self) -> CString:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return b""
            return self.file_ext.obj.get_SearchPath()

        def RegisterComponent(self, cid: Ptr[UGUID], filename: CString) -> Int16:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return EcoErrorCode.POINTER
            return self.file_ext.obj.RegisterComponent(cid, filename)

        def QueryComponent(
            self,
            filename: CString,
            cid: Ptr[UGUID],
            outer: Optional[Ptr[IEcoUnknown]],
            iid: Ptr[UGUID],
            out: Ptr[Ptr[Void]],
        ) -> Int16:
            """Forward to the inner file extension."""
            if not bool(self.file_ext):
                return EcoErrorCode.POINTER
            return self.file_ext.obj.QueryComponent(filename, cid, outer, iid, out)

    # =========================================================================
    # Python constructor
    # =========================================================================

    def __init__(
        self,
        argc: Optional[int] = None,
        argv: Optional[list[bytes]] = None,
        lib_dir: Optional[Union[str, Path]] = None,
    ) -> None:
        """Build the component and run the bootstrap sequence.

        Args:
            argc: Number of command-line arguments, defaults to `len(argv)`.
            argv: Command-line argument vector, defaults to `sys.argv` encoded as bytes.
            lib_dir: Directory scanned for user component libraries, defaults to `CWD`.

        Raises:
            FileNotFoundError: If `ECO_FRAMEWORK_RT` is missing or `lib_dir` is invalid.
            EcoError: If any bootstrap step fails.
        """
        if argv is None:
            argv = [arg.encode() for arg in sys.argv]
        if argc is None:
            argc = len(argv)
        self._lib_dir = Path(lib_dir) if lib_dir else Path.cwd()
        self._loader = EcoLibLoader()
        self._loaded_libs: list[EcoLib] = []
        result = self.__eco_new__(argc, argv, self._lib_dir)
        if result != 0:
            raise EcoError(result, "Failed to bootstrap EcoSystem")

    # =========================================================================
    # Context manager
    # =========================================================================

    def __enter__(self) -> Self:
        """Return the already-bootstrapped instance."""
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Release every owned resource on scope exit."""
        self.__eco_del__()

    # =========================================================================
    # Lifecycle
    # =========================================================================

    def __eco_new__(self, argc: int, argv: list[bytes], lib_dir: Path) -> Int16:
        """Run the 8-step bootstrap.

        Args:
            argc: Number of command-line arguments.
            argv: Command-line argument vector.
            lib_dir: Directory scanned for user component libraries.

        Returns:
            0 on success, error code otherwise.

        Raises:
            FileNotFoundError: If `ECO_FRAMEWORK_RT` is missing or `lib_dir` is invalid.
            EcoError: If any registration or interface query fails.
        """
        # Reset all interface pointers and instance data
        self.bus = Ptr[IEcoInterfaceBus1]()
        self.mem_manager = Ptr[IEcoMemoryManager1]()
        self.mem_ext = Ptr[IEcoInterfaceBus1MemExt]()
        self.file_ext = Ptr[IEcoInterfaceBus1FileExt]()
        self.data = Ptr[Void]()
        self.argc = argc
        self.argv = Array[CString, len(argv)](*argv)

        # Resolve the runtime path from the environment
        runtime_path = os.environ.get("ECO_FRAMEWORK_RT")
        if runtime_path is None:
            self.logger.error("Runtime path not configured ---> 'ECO_FRAMEWORK_RT' missing")
            raise FileNotFoundError("'ECO_FRAMEWORK_RT' environment variable not set")

        full_runtime_path = Path(runtime_path) / GID_IEcoSystem.to_string(with_hyphens=False)
        if not full_runtime_path.exists():
            self.logger.error(f"Runtime path not found ---> '{full_runtime_path}'")
            raise FileNotFoundError(f"Runtime path not found: '{full_runtime_path}'")

        self.logger.debug(f"Bootstrap starting... | GID = <{GID_IEcoSystem}>")

        # ---------------------------------------------------------------------
        # Step 1/8 — Load `InterfaceBus` and create the bus instance via factory
        # ---------------------------------------------------------------------
        self.logger.debug("Step [1/8] ---> Loading 'InterfaceBus'")
        loaded = self._loader.load_by_cid(CID_EcoInterfaceBus1, [full_runtime_path])
        self._loaded_libs.append(loaded)
        bus_factory = loaded.factory

        bus_ptr = Ptr[Void]()
        result = bus_factory.obj.Alloc(None, None, byref(IID_IEcoInterfaceBus1), byref(bus_ptr))
        if result != 0 or not bool(bus_ptr):
            self.logger.error(f"Step [1/8] ---> Alloc failed | code = {result}")
            raise EcoError(result, "Failed to create `InterfaceBus` instance")

        self.bus = cast(bus_ptr, Ptr[IEcoInterfaceBus1])
        self.logger.debug("Step [1/8] ---> 'InterfaceBus' ---> OK")

        # ---------------------------------------------------------------------
        # Step 2/8 — Load `MemoryManager` and register its factory with the bus
        # ---------------------------------------------------------------------
        self.logger.debug("Step [2/8] ---> Registering 'MemoryManager'")
        loaded = self._loader.load_by_cid(CID_EcoMemoryManager1, [full_runtime_path])
        self._loaded_libs.append(loaded)

        result = self.bus.obj.RegisterComponent(
            byref(CID_EcoMemoryManager1), cast(loaded.factory, Ptr[IEcoUnknown])
        )
        if result != 0:
            self.logger.error(f"Step [2/8] ---> Register failed | code = {result}")
            raise EcoError(result, "Failed to register `MemoryManager`")
        self.logger.debug("Step [2/8] ---> 'MemoryManager' ---> OK")

        # ---------------------------------------------------------------------
        # Step 3/8 — Configure bus memory extension if available
        # ---------------------------------------------------------------------
        self.logger.debug("Step [3/8] ---> Configuring 'MemExt'")
        mem_ext_ptr = Ptr[Void]()
        result = self.bus.obj.QueryInterface(byref(IID_IEcoInterfaceBus1MemExt), byref(mem_ext_ptr))
        if result == 0 and bool(mem_ext_ptr):
            self.mem_ext = cast(mem_ext_ptr, Ptr[IEcoInterfaceBus1MemExt])
            self.mem_ext.obj.set_Manager(byref(CID_EcoMemoryManager1))
            self.mem_ext.obj.set_ExpandPool(True)
            self.logger.debug("Step [3/8] ---> 'MemExt' ---> OK")
        else:
            self.logger.debug("Step [3/8] ---> 'MemExt' not available ---> Skipped")

        # ---------------------------------------------------------------------
        # Step 4/8 — Query `MemoryManager` and initialize the heap
        # ---------------------------------------------------------------------
        self.logger.debug(
            f"Step [4/8] ---> Initializing 'MemoryManager' | heap = {self.DEFAULT_HEAP_SIZE} bytes"
        )
        mgr_ptr = Ptr[Void]()
        result = self.bus.obj.QueryComponent(
            byref(CID_EcoMemoryManager1),
            None,
            byref(IID_IEcoMemoryManager1),
            byref(mgr_ptr),
        )
        if result != 0 or not bool(mgr_ptr):
            self.logger.error(f"Step [4/8] ---> Query failed | code = {result}")
            raise EcoError(result, "Failed to get `MemoryManager` interface")

        self.mem_manager = cast(mgr_ptr, Ptr[IEcoMemoryManager1])
        self.mem_manager.obj.Init(None, self.DEFAULT_HEAP_SIZE)
        self.logger.debug("Step [4/8] ---> 'MemoryManager' ---> OK")

        # ---------------------------------------------------------------------
        # Step 5/8 — Load `FileSystemManagement` and register with the bus
        # ---------------------------------------------------------------------
        self.logger.debug("Step [5/8] ---> Registering 'FileSystemManagement'")
        loaded = self._loader.load_by_cid(CID_EcoFileSystemManagement1, [full_runtime_path])
        self._loaded_libs.append(loaded)

        result = self.bus.obj.RegisterComponent(
            byref(CID_EcoFileSystemManagement1), cast(loaded.factory, Ptr[IEcoUnknown])
        )
        if result != 0:
            self.logger.error(f"Step [5/8] ---> Register failed | code = {result}")
            raise EcoError(result, "Failed to register `FileSystemManagement`")
        self.logger.debug("Step [5/8] ---> 'FileSystemManagement' ---> OK")

        # ---------------------------------------------------------------------
        # Step 6/8 — Configure bus file extension if available
        # ---------------------------------------------------------------------
        self.logger.debug("Step [6/8] ---> Configuring 'FileExt'")
        file_ext_ptr = Ptr[Void]()
        result = self.bus.obj.QueryInterface(
            byref(IID_IEcoInterfaceBus1FileExt), byref(file_ext_ptr)
        )
        if result == 0 and bool(file_ext_ptr):
            self.file_ext = cast(file_ext_ptr, Ptr[IEcoInterfaceBus1FileExt])
            self.file_ext.obj.set_Manager(byref(CID_EcoFileSystemManagement1))
            self.logger.debug("Step [6/8] ---> 'FileExt' ---> OK")
        else:
            self.logger.debug("Step [6/8] ---> 'FileExt' not available ---> Skipped")

        # ---------------------------------------------------------------------
        # Step 7/8 — Auto-register user component libraries from `lib_dir`
        # ---------------------------------------------------------------------
        self.logger.debug(f"Step [7/8] ---> Scanning user libs in '{lib_dir}'")
        if not lib_dir.exists():
            self.logger.error(f"Step [7/8] ---> User lib path not found ---> '{lib_dir}'")
            raise FileNotFoundError(f"User library path not found: '{lib_dir}'")
        if not lib_dir.is_dir():
            self.logger.error(f"Step [7/8] ---> User lib path is not a directory ---> '{lib_dir}'")
            raise NotADirectoryError(f"User library path is not a directory: '{lib_dir}'")

        registered = 0
        for lib_file in lib_dir.iterdir():
            if not lib_file.is_file() or not is_eco_lib(lib_file.name):
                continue
            cid = lib_filename_to_guid(lib_file.name)
            if cid is None:
                continue
            if bytes(cid.data) in self.SYSTEM_CIDS:
                self.logger.debug(
                    f"Step [7/8] ---> Skipping system component ---> '{lib_file.name}'"
                )
                continue

            loaded = self._loader.load(lib_file)
            self._loaded_libs.append(loaded)
            result = self.bus.obj.RegisterComponent(
                byref(cid), cast(loaded.factory, Ptr[IEcoUnknown])
            )
            if result != 0:
                self.logger.error(
                    f"Step [7/8] ---> Register failed ---> '{lib_file.name}' | code = {result}"
                )
                raise EcoError(result, f"Failed to register component: '{lib_file.name}'")
            registered += 1
            self.logger.debug(f"Step [7/8] ---> Registered ---> '{lib_file.name}'")
        self.logger.debug(f"Step [7/8] ---> Scan complete | registered = {registered}")

        # ---------------------------------------------------------------------
        # Step 8/8 — Finalize the bus factory via `Init`
        # ---------------------------------------------------------------------
        if bool(self.bus):
            self.logger.debug("Step [8/8] ---> Finalizing 'InterfaceBus' factory")
            bus_factory.obj.Init(pointer(self, IEcoUnknown), cast(self.bus, Ptr[Void]))
            self.logger.debug("Step [8/8] ---> 'InterfaceBus' factory ---> OK")
        else:
            self.logger.debug("Step [8/8] ---> 'InterfaceBus' factory not available ---> Skipped")

        self.logger.debug("Bootstrap complete ---> OK")
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Release every owned interface and drop pinned libraries.

        Resources are released in reverse acquisition order. Pinned libraries are
        dropped last so their handles outlive any final teardown call.
        """
        self.logger.debug("Releasing resources...")
        for iface_ptr in (self.file_ext, self.mem_ext, self.mem_manager, self.bus):
            try:
                if bool(iface_ptr):
                    iface_ptr.obj.Release()
            except Exception as err:
                self.logger.debug(f"Ignoring error ---> {err}")
        self._loaded_libs.clear()
        self.logger.debug("Released ---> OK")


__all__ = ["EcoSystem"]
