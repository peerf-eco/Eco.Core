"""Linked list interface for EcoOS.

The `IEcoList1` interface exposes a generic linked list of opaque pointers
(`void*`). It is widely used inside Eco components as a building block for
storing collections of items by their native address.

Reference:
    Based on `IEcoList1.h` from `Eco.List1/SharedFiles`.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoList1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import UInt32, Void
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoList1)
class IEcoList1(IEcoUnknown):
    """Linked list of opaque pointers (`void*`)."""

    def Count(self) -> UInt32:
        """Return the current number of elements stored in the list.

        Returns:
            Element count.
        """
        ...

    def Item(self, index: UInt32) -> Ptr[Void]:
        """Return the element at `index` without removing it.

        Args:
            index: Zero-based index in `[0, Count())`.

        Returns:
            Stored pointer at that position, or `NULL` if the index is invalid.
        """
        ...

    def Add(self, value: Ptr[Void]) -> UInt32:
        """Append a value to the end of the list.

        Args:
            value: Opaque pointer to store.

        Returns:
            New element count after the append.
        """
        ...

    def IndexOf(self, value: Ptr[Void]) -> UInt32:
        """Return the index of the first occurrence of `value`.

        Args:
            value: Pointer to locate.

        Returns:
            Zero-based index, or `-1` when the value is not present.
        """
        ...

    def InsertAt(self, index: UInt32, value: Ptr[Void]) -> Void:
        """Insert `value` at the given position, shifting later elements right.

        Args:
            index: Zero-based insertion position.
            value: Pointer to store.
        """
        ...

    def Remove(self, value: Ptr[Void]) -> Void:
        """Remove the first occurrence of `value` from the list.

        Args:
            value: Pointer to remove.
        """
        ...

    def RemoveAt(self, index: UInt32) -> Void:
        """Remove the element stored at `index`.

        Args:
            index: Zero-based index of the element to remove.
        """
        ...

    def Clear(self) -> Void:
        """Drop every element from the list, leaving it empty."""
        ...
