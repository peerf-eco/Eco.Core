"""Base ACOM interface definitions.

This module provides the foundational `IEcoUnknown` interface that all
ACOM interfaces inherit from, defined using our declarative approach.

The `IEcoUnknown` interface provides three fundamental methods:
    - QueryInterface: Get a different interface from a component.
    - AddRef: Increment reference count.
    - Release: Decrement reference count (free when zero).
"""

from __future__ import annotations

from typing import Optional, Union

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown
from eco_python2acom.types.core import CString, Int16, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import cast

# =============================================================================
# IEcoUnknown - Base interface for all ACOM components
# =============================================================================


@interface(iid=IID_IEcoUnknown)
class IEcoUnknown:
    """Base interface for all ACOM components.

    Every ACOM interface inherits from `IEcoUnknown` and must implement
    these three methods. The `@interface` decorator automatically adds
    them to the VTbl in the correct order.

    Note:
        When defining a new interface, inherit from `IEcoUnknown`.
        The decorator will automatically include these methods.
    """

    def __init__(self, ptr: Union[Ptr[Void], int]) -> None:
        """Initialize interface wrapper from a pointer.

        Args:
            ptr: Pointer to interface object.

        Raises:
            ValueError: If ptr is NULL or pVTbl is NULL.
        """
        if isinstance(ptr, int):
            ptr = Ptr[Void](ptr)
        if not ptr or not ptr.value:
            raise ValueError(f"{self.__class__.__name__}: NULL pointer")

        self.ptr = ptr

        pVTbl = cast(ptr, Ptr[Ptr[Void]]).contents
        if not pVTbl or not pVTbl.value:
            raise ValueError(f"{self.__class__.__name__}: NULL pVTbl")

        self.vtbl = cast(pVTbl, Ptr[type(self)]).contents  # type: ignore

    def __repr__(self) -> str:
        """Return string representation of interface instance."""
        addr = self.ptr.value if self.ptr else 0
        return f"<{self.__class__.__name__} at 0x{addr:X}>"

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for another interface on this component.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        ...

    def Release(self) -> UInt32:
        """Decrement the reference count.

        When the count reaches zero, the component is freed.

        Returns:
            The new reference count.
        """
        ...


# =============================================================================
# IEcoComponentFactory - Factory for creating component instances
# =============================================================================


@interface(iid=IID_IEcoComponentFactory)
class IEcoComponentFactory(IEcoUnknown):
    """Factory interface for creating ACOM component instances.

    Each component exports `GetIEcoComponentFactoryPtr()` that returns
    a pointer to this interface.
    """

    def Alloc(
        self,
        system: Optional[Ptr[Void]],
        outer: Optional[Ptr[Void]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Allocate a new component instance.

        Args:
            system: Pointer to system interface (can be NULL).
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Init(self, system: Optional[Ptr[Void]], context: Ptr[Void]) -> Int16:
        """Initialize the factory with system context.

        Args:
            system: Pointer to system interface (can be NULL).
            context: Additional context (e.g., bus pointer).

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Name(self) -> CString:
        """Get the component name.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    def get_Version(self) -> CString:
        """Get the component version.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    def get_Manufacturer(self) -> CString:
        """Get the component manufacturer.

        Returns:
            Pointer to null-terminated string.
        """
        ...
