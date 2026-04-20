"""Base ACOM interface definition.

This module provides the foundational `IEcoUnknown` interface that all
other ACOM interfaces extend, defined using our declarative approach.

The `IEcoUnknown` interface provides three fundamental methods:
    - `QueryInterface`: Get a different interface from a component.
    - `AddRef`: Increment reference count.
    - `Release`: Decrement reference count (free when zero).
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoUnknown)
class IEcoUnknown:
    """Base interface for all ACOM components.

    Every ACOM interface extends `IEcoUnknown` and must implement these
    three methods.
    """

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
