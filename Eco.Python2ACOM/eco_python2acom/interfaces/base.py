"""Base ACOM interface definitions.

This module provides the foundational IEcoUnknown interface that all
ACOM interfaces inherit from, defined using our declarative approach.

After decoration, classes are directly instantiable from VoidPtr:
    >>> obj = IEcoUnknown(ptr)
    >>> obj.Release()

The IEcoUnknown interface provides three fundamental methods:
    - QueryInterface: Get a different interface from a component.
    - AddRef: Increment reference count.
    - Release: Decrement reference count (free when zero).
"""

from __future__ import annotations

from ctypes import _Pointer
from typing import TYPE_CHECKING, Optional

from eco_python2acom.core.guid import UGUIDPtr
from eco_python2acom.core.types import CharPtr, EcoResult, UInt32, VoidPtr, VoidPtrPtr
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.interfaces.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown

# =============================================================================
# IEcoUnknown - Base interface for all ACOM components
# =============================================================================


@interface(iid=IID_IEcoUnknown)
class IEcoUnknown:
    """Base interface for all ACOM components.

    Every ACOM interface inherits from IEcoUnknown and must implement
    these three methods. The @interface decorator automatically adds
    them to the VTbl in the correct order.

    Instantiate from a raw pointer:
        >>> unknown = IEcoUnknown(ptr)
        >>> unknown.AddRef()
        >>> unknown.Release()

    Note:
        When defining a new interface, inherit from IEcoUnknown.
        The decorator will automatically include these methods.
    """

    def __init__(self, ptr: VoidPtr) -> None:
        """Initialize from a raw interface pointer.

        Args:
            ptr: VoidPtr to the ACOM interface.

        Raises:
            ValueError: If ptr is NULL.
        """
        ...

    @property
    def ptr(self) -> VoidPtr:
        """Raw interface pointer."""
        return self._ptr  # type: ignore

    @method
    def QueryInterface(self, riid: UGUIDPtr, ppv: VoidPtrPtr) -> EcoResult:
        """Query for another interface on this component.

        Args:
            riid: Pointer to the requested interface ID.
            ppv: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        ...

    @method
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

    Each component DLL exports GetIEcoComponentFactoryPtr() that returns
    a pointer to this interface.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Alloc(
        self,
        pISystem: Optional[VoidPtr],
        pIUnknownOuter: Optional[VoidPtr],
        riid: UGUIDPtr,
        ppv: VoidPtrPtr,
    ) -> EcoResult:
        """Allocate a new component instance.

        Args:
            pISystem: Pointer to system interface (can be NULL).
            pIUnknownOuter: Outer unknown for aggregation (can be NULL).
            riid: Requested interface ID.
            ppv: Output pointer for the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def Init(self, pISystem: Optional[VoidPtr], pv: VoidPtr) -> EcoResult:
        """Initialize the factory with system context.

        Args:
            pISystem: Pointer to system interface (can be NULL).
            pv: Additional context (e.g., bus pointer).

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Name(self) -> CharPtr:
        """Get the component name.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    @method
    def get_Version(self) -> CharPtr:
        """Get the component version.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    @method
    def get_Manufacturer(self) -> CharPtr:
        """Get the component manufacturer.

        Returns:
            Pointer to null-terminated string.
        """
        ...


if TYPE_CHECKING:
    IEcoUnknownPtr = _Pointer[IEcoUnknown]
    IEcoComponentFactoryPtr = _Pointer[IEcoComponentFactory]
else:
    IEcoUnknownPtr = IEcoUnknown._interface_ptr_
    IEcoComponentFactoryPtr = IEcoComponentFactory._interface_ptr_
