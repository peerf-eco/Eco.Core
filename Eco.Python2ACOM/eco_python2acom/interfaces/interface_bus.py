"""Interface bus interfaces for EcoOS.

The `InterfaceBus` is the central registry for components in EcoOS.

Interfaces:
    `IEcoInterfaceBus1`: Core interface bus for component management.
    `IEcoInterfaceBus1MemExt`: Memory extension for heap configuration.
    `IEcoInterfaceBus1FileExt`: File extension for auto-loading.

Reference:
    Based on `IEcoInterfaceBus1.h`, `IEcoInterfaceBus1MemExt.h`,
    `IEcoInterfaceBus1FileExt.h` from `Eco.InterfaceBus1/SharedFiles`.
"""

from typing import Optional

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import (
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1FileExt,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoInterfaceBus1NetExt,
)
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Bool, CString, Int16, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

# =============================================================================
# IEcoInterfaceBus1
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1)
class IEcoInterfaceBus1(IEcoUnknown):
    """Core interface bus for component registration and querying.

    The `InterfaceBus` maintains a registry of component factories
    and provides methods to create component instances.
    """

    def Init(self) -> Int16:
        """Initialize the bus (no heap).

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def InitWith(self, heap: Ptr[Void], size: UInt32) -> Int16:
        """Initialize the bus with custom heap.

        Args:
            heap: Pointer to heap memory.
            size: Size of heap in bytes.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def RegisterComponent(self, cid: Ptr[UGUID], factory: Ptr[IEcoUnknown]) -> Int16:
        """Register a component factory by CID.

        Args:
            cid: Component ID pointer.
            factory: Pointer to `IEcoComponentFactory`.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def UnRegisterComponent(self, cid: Ptr[UGUID]) -> Int16:
        """Unregister a component by CID.

        Args:
            cid: Component ID pointer.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def QueryComponent(
        self,
        cid: Ptr[UGUID],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Create component instance and get interface.

        Args:
            cid: Component ID to instantiate.
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoInterfaceBus1MemExt
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1MemExt)
class IEcoInterfaceBus1MemExt(IEcoUnknown):
    """Memory extension interface for `InterfaceBus`.

    Used to configure which memory manager the bus uses
    for internal allocations and pool management.
    """

    def set_Manager(self, cid: Ptr[UGUID]) -> Int16:
        """Set the memory manager CID.

        Args:
            cid: Memory manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Manager(self) -> Ptr[UGUID]:
        """Get the current memory manager CID.

        Returns:
            Pointer to memory manager CID.
        """
        ...

    def set_ExpandPool(self, enable: Bool) -> Int16:
        """Enable/disable pool expansion.

        Args:
            enable: True to enable pool expansion.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoInterfaceBus1FileExt
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1FileExt)
class IEcoInterfaceBus1FileExt(IEcoUnknown):
    """File extension interface for `InterfaceBus`.

    Enables automatic component loading from library files
    based on a configurable search path.
    """

    def set_Manager(self, cid: Ptr[UGUID]) -> Int16:
        """Set the file system manager CID.

        Args:
            cid: File system manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Manager(self) -> Ptr[UGUID]:
        """Get the current file system manager CID.

        Returns:
            Pointer to file manager CID.
        """
        ...

    def set_SearchPath(self, path: CString) -> Int16:
        """Set path for component search.

        Args:
            path: Search path string.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_SearchPath(self) -> CString:
        """Get current search path.

        Returns:
            Pointer to search path string.
        """
        ...

    def RegisterComponent(self, cid: Ptr[UGUID], filename: CString) -> Int16:
        """Register component from file by CID.

        Args:
            cid: Component ID.
            filename: Path to component file.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def QueryComponent(
        self,
        filename: CString,
        cid: Ptr[UGUID],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Query component from file directly.

        Args:
            filename: Path to component file.
            cid: Component ID.
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoInterfaceBus1FileExt
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1NetExt)
class IEcoInterfaceBus1NetExt(IEcoUnknown):
    """Network extension interface for `InterfaceBus`.

    Enables remote component registration and querying
    over a network connection.
    """

    def set_Manager(self, cid: Ptr[UGUID]) -> Int16:
        """Set the network manager CID.

        Args:
            cid: Network manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Manager(self) -> Ptr[UGUID]:
        """Get the current network manager CID.

        Returns:
            Pointer to network manager CID.
        """
        ...

    def QueryComponent(
        self,
        network_name: CString,
        cid: Ptr[UGUID],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Query remote component over network.

        Args:
            network_name: Network name of remote bus.
            cid: Component ID.
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...
