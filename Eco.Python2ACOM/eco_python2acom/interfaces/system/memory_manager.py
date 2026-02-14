"""Memory Manager interfaces for EcoOS.

The Memory Manager provides heap memory management for EcoOS components.

Interfaces:
    IEcoMemoryManager1: Low-level manager for heap initialization.
    IEcoMemoryAllocator1: User-facing allocator for memory operations.

Reference:
    Based on IEcoMemoryManager1.h and IEcoMemoryAllocator1.h
    from Eco.MemoryManager1/SharedFiles.
"""

from __future__ import annotations

from ctypes import _Pointer
from typing import TYPE_CHECKING, Optional

from eco_python2acom.core.types import Char, EcoStructure, Int16, Ptr, UInt32, UInt32Ptr, VoidPtr
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.interfaces.guids.iid import IID_IEcoMemoryAllocator1, IID_IEcoMemoryManager1


class EcoMemoryManager1Block(EcoStructure):
    _fields_ = [
        ("lowAddr", UInt32),
        ("highAddr", UInt32),
        ("size", UInt32),
    ]


class EcoMemoryManager1Status(EcoStructure):
    _fields_ = [
        ("lowAddr", UInt32),
        ("highAddr", UInt32),
        ("totalSize", UInt32),
        ("freeSize", UInt32),
        ("usedBlocks", UInt32),
    ]


if TYPE_CHECKING:
    EcoMemoryManager1BlockPtr = _Pointer[EcoMemoryManager1Block]
    EcoMemoryManager1StatusPtr = _Pointer[EcoMemoryManager1Status]
else:
    EcoMemoryManager1BlockPtr = Ptr(EcoMemoryManager1Block)
    EcoMemoryManager1StatusPtr = Ptr(EcoMemoryManager1Status)


# =============================================================================
# IEcoMemoryManager1
# =============================================================================


@interface(iid=IID_IEcoMemoryManager1)
class IEcoMemoryManager1(IEcoUnknown):
    """Memory manager interface for heap initialization.

    Used internally by EcoSystem to set up the memory heap
    before allocations can be made.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Init(self, startAddress: Optional[VoidPtr], size: UInt32) -> Int16:
        """Initialize the memory manager with heap.

        Args:
            startAddress: Start of heap memory (NULL for auto).
            size: Size of heap in bytes.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Status(self, status: EcoMemoryManager1StatusPtr) -> Int16:
        """Get memory status information.

        Args:
            status: Pointer to status structure.

        Returns:
            0 on success, non-zero on failure.
        """
        ...

    @method
    def get_UsedBlocks(self, blocks: EcoMemoryManager1BlockPtr, size_in_blocks: UInt32Ptr) -> Int16:
        """Get information about a used memory block.

        Args:
            block: Pointer to a memory block structure.

        Returns:
            0 if the block is used, non-zero if not.
        """
        ...


# =============================================================================
# IEcoMemoryAllocator1
# =============================================================================


@interface(iid=IID_IEcoMemoryAllocator1)
class IEcoMemoryAllocator1(IEcoUnknown):
    """Memory allocator interface for heap operations.

    Provides standard memory operations: alloc, free, realloc,
    copy, fill, compare, and size retrieval.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Alloc(self, size: UInt32) -> VoidPtr:
        """Allocate memory block.

        Args:
            size: Size in bytes to allocate.

        Returns:
            Pointer to allocated memory, or NULL on failure.
        """
        ...

    @method
    def Free(self, ptr: VoidPtr) -> None:
        """Free memory block.

        Args:
            ptr: Pointer to memory to free.
        """
        ...

    @method
    def Realloc(self, ptr: VoidPtr, size: UInt32) -> VoidPtr:
        """Reallocate memory block.

        Args:
            ptr: Pointer to existing memory.
            size: New size in bytes.

        Returns:
            Pointer to reallocated memory.
        """
        ...

    @method
    def Copy(self, dest: VoidPtr, src: VoidPtr, size: UInt32) -> VoidPtr:
        """Copy memory from source to destination.

        Args:
            dest: Destination pointer.
            src: Source pointer.
            size: Number of bytes to copy.

        Returns:
            Destination pointer.
        """
        ...

    @method
    def Fill(self, dest: VoidPtr, value: Char, size: UInt32) -> VoidPtr:
        """Fill memory with a byte value.

        Args:
            dest: Destination pointer.
            value: Byte value to fill with.
            size: Number of bytes to fill.

        Returns:
            Destination pointer.
        """
        ...

    @method
    def Compare(self, ptr1: VoidPtr, ptr2: VoidPtr, size: UInt32) -> Int16:
        """Compare two memory blocks.

        Args:
            ptr1: First memory block.
            ptr2: Second memory block.
            size: Number of bytes to compare.

        Returns:
            0 if equal, negative if ptr1 < ptr2, positive if ptr1 > ptr2.
        """
        ...

    @method
    def RetrieveSize(self, ptr: VoidPtr) -> UInt32:
        """Get size of allocated block.

        Args:
            ptr: Pointer to allocated memory.

        Returns:
            Size of the block in bytes.
        """
        ...


if TYPE_CHECKING:
    IEcoMemoryManager1Ptr = _Pointer[IEcoMemoryManager1]
    IEcoMemoryAllocator1Ptr = _Pointer[IEcoMemoryAllocator1]
else:
    IEcoMemoryManager1Ptr = IEcoMemoryManager1._interface_ptr_
    IEcoMemoryAllocator1Ptr = IEcoMemoryAllocator1._interface_ptr_
