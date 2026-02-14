"""InterfaceBus interfaces for EcoOS.

The InterfaceBus is the central registry for components in EcoOS.
All interfaces inherit from IEcoUnknown.

Interfaces:
    IEcoInterfaceBus1: Core interface bus for component management.
    IEcoInterfaceBus1MemExt: Memory extension for heap configuration.
    IEcoInterfaceBus1FileExt: File extension for DLL auto-loading.

Reference:
    Based on IEcoInterfaceBus1.h, IEcoInterfaceBus1MemExt.h,
    IEcoInterfaceBus1FileExt.h from Eco.InterfaceBus1/SharedFiles.
"""

from __future__ import annotations

from ctypes import _Pointer
from typing import TYPE_CHECKING, Optional

from eco_python2acom.core.guid import UGUIDPtr
from eco_python2acom.core.types import Bool, CharPtr, Int16, UInt32, VoidPtr, VoidPtrPtr
from eco_python2acom.interfaces.base import IEcoUnknown, IEcoUnknownPtr
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.interfaces.guids.iid import (
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1FileExt,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoInterfaceBus1NetExt,
)

# =============================================================================
# IEcoInterfaceBus1
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1)
class IEcoInterfaceBus1(IEcoUnknown):
    """Core interface bus for component registration and querying.

    The InterfaceBus maintains a registry of component factories
    and provides methods to create component instances.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Init(self) -> Int16:
        """Initialize the bus (no heap).

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def InitWith(self, pHeap: VoidPtr, size: UInt32) -> Int16:
        """Initialize the bus with custom heap.

        Args:
            pHeap: Pointer to heap memory.
            size: Size of heap in bytes.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def RegisterComponent(self, cid: UGUIDPtr, pIFactory: IEcoUnknownPtr) -> Int16:
        """Register a component factory by CID.

        Args:
            cid: Component ID pointer.
            pIFactory: Pointer to IEcoComponentFactory.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def UnRegisterComponent(self, cid: UGUIDPtr) -> Int16:
        """Unregister a component by CID.

        Args:
            cid: Component ID pointer.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def QueryComponent(
        self,
        cid: UGUIDPtr,
        pIUnknownOuter: Optional[IEcoUnknownPtr],
        iid: UGUIDPtr,
        ppv: VoidPtrPtr,
    ) -> Int16:
        """Create component instance and get interface.

        Args:
            cid: Component ID to instantiate.
            pIUnknownOuter: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            ppv: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoInterfaceBus1MemExt
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1MemExt)
class IEcoInterfaceBus1MemExt(IEcoUnknown):
    """Memory extension interface for InterfaceBus.

    Used to configure which memory manager the bus uses
    for internal allocations and pool management.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def set_Manager(self, cid: UGUIDPtr) -> Int16:
        """Set the memory manager CID.

        Args:
            cid: Memory manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Manager(self) -> UGUIDPtr:
        """Get the current memory manager CID.

        Returns:
            Pointer to memory manager CID.
        """
        ...

    @method
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
    """File extension interface for InterfaceBus.

    Enables automatic component loading from DLL files
    based on a configurable search path.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def set_Manager(self, cid: UGUIDPtr) -> Int16:
        """Set the file system manager CID.

        Args:
            cid: File system manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Manager(self) -> UGUIDPtr:
        """Get the current file system manager CID.

        Returns:
            Pointer to file manager CID.
        """
        ...

    @method
    def set_SearchPath(self, path: CharPtr) -> Int16:
        """Set path for component DLL search.

        Args:
            path: Search path string.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_SearchPath(self) -> CharPtr:
        """Get current search path.

        Returns:
            Pointer to search path string.
        """
        ...

    @method
    def RegisterComponent(self, cid: UGUIDPtr, filename: CharPtr) -> Int16:
        """Register component from file by CID.

        Args:
            cid: Component ID.
            filename: DLL filename.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def QueryComponent(
        self,
        filename: CharPtr,
        cid: UGUIDPtr,
        pIUnknownOuter: Optional[IEcoUnknownPtr],
        iid: UGUIDPtr,
        ppv: VoidPtrPtr,
    ) -> Int16:
        """Query component from file directly.

        Args:
            filename: DLL filename.
            cid: Component ID.
            pIUnknownOuter: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            ppv: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoInterfaceBus1FileExt
# =============================================================================


@interface(iid=IID_IEcoInterfaceBus1NetExt)
class IEcoInterfaceBus1NetExt(IEcoUnknown):
    """Network extension interface for InterfaceBus.

    Enables remote component registration and querying
    over a network connection.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def set_Manager(self, cid: UGUIDPtr) -> Int16:
        """Set the network manager CID.

        Args:
            cid: Network manager component ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Manager(self) -> UGUIDPtr:
        """Get the current network manager CID.

        Returns:
            Pointer to network manager CID.
        """
        ...

    @method
    def QueryRemoteComponent(
        self,
        network_name: CharPtr,
        cid: UGUIDPtr,
        pIUnknownOuter: Optional[IEcoUnknownPtr],
        iid: UGUIDPtr,
        ppv: VoidPtrPtr,
    ) -> Int16:
        """Query remote component over network.

        Args:
            network_name: Network name of remote bus.
            cid: Component ID.
            pIUnknownOuter: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            ppv: Output pointer for interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...


if TYPE_CHECKING:
    IEcoInterfaceBus1Ptr = _Pointer[IEcoInterfaceBus1]
    IEcoInterfaceBus1MemExtPtr = _Pointer[IEcoInterfaceBus1MemExt]
    IEcoInterfaceBus1FileExtPtr = _Pointer[IEcoInterfaceBus1FileExt]
    IEcoInterfaceBus1NetExtPtr = _Pointer[IEcoInterfaceBus1NetExt]
else:
    IEcoInterfaceBus1Ptr = IEcoInterfaceBus1._interface_ptr_
    IEcoInterfaceBus1MemExtPtr = IEcoInterfaceBus1MemExt._interface_ptr_
    IEcoInterfaceBus1FileExtPtr = IEcoInterfaceBus1FileExt._interface_ptr_
    IEcoInterfaceBus1NetExtPtr = IEcoInterfaceBus1NetExt._interface_ptr_
