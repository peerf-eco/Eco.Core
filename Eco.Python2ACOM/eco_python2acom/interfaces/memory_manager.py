"""Memory manager interfaces for EcoOS.

The `MemoryManager` provides heap memory management for EcoOS components.

Interfaces:
    IEcoMemoryManager1: Low-level manager for heap initialization.
    IEcoMemoryAllocator1: User-facing allocator for memory operations.

Reference:
    Based on `IEcoMemoryManager1.h` and `IEcoMemoryAllocator1.h`
    from `Eco.MemoryManager1/SharedFiles`.
"""

from __future__ import annotations

from typing import Optional

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model
from eco_python2acom.guids.iid import IID_IEcoMemoryAllocator1, IID_IEcoMemoryManager1
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.core import Char, Int16, UInt32, Void
from eco_python2acom.types.pointer import Ptr


@model
class EcoMemoryManager1Block:
    """Descriptor of a single memory block in the manager's heap.

    Attributes:
        low_addr: Low (start) address of the block.
        high_addr: High (end) address of the block.
        size: Size of the block in bytes.
    """

    low_addr: UInt32
    high_addr: UInt32
    size: UInt32


@model
class EcoMemoryManager1Status:
    """Current status of the memory manager heap.

    Attributes:
        low_addr: Low (start) address of the heap.
        high_addr: High (end) address of the heap.
        total_size: Total heap size in bytes.
        free_size: Free (unused) size in bytes.
        used_blocks: Number of allocated blocks.
    """

    low_addr: UInt32
    high_addr: UInt32
    total_size: UInt32
    free_size: UInt32
    used_blocks: UInt32


# =============================================================================
# IEcoMemoryManager1
# =============================================================================


@interface(iid=IID_IEcoMemoryManager1)
class IEcoMemoryManager1(IEcoUnknown):
    """Memory manager interface for heap initialization.

    Used internally by `EcoSystem` to set up the memory heap
    before allocations can be made.
    """

    def Init(self, start: Optional[Ptr[Void]], size: UInt32) -> Int16:
        """Initialize the memory manager with heap.

        Args:
            start: Start of heap memory (NULL for auto).
            size: Size of heap in bytes.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Status(self, status: Ptr[EcoMemoryManager1Status]) -> Int16:
        """Get memory status information.

        Args:
            status: Pointer to status structure.

        Returns:
            0 on success, non-zero on failure.
        """
        ...

    def get_UsedBlocks(
        self, blocks: Ptr[EcoMemoryManager1Block], size_in_blocks: Ptr[UInt32]
    ) -> Int16:
        """Get descriptors of used memory blocks.

        Args:
            blocks: Output array of EcoMemoryManager1Block structures.
            size_in_blocks: Input size of the array; output number of blocks written.

        Returns:
            0 on success, error code otherwise.
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
    """

    def Alloc(self, size: UInt32) -> Ptr[Void]:
        """Allocate memory block.

        Args:
            size: Size in bytes to allocate.

        Returns:
            Pointer to allocated memory, or NULL on failure.
        """
        ...

    def Free(self, ptr: Ptr[Void]) -> None:
        """Free memory block.

        Args:
            ptr: Pointer to memory to free.
        """
        ...

    def Realloc(self, ptr: Ptr[Void], size: UInt32) -> Ptr[Void]:
        """Reallocate memory block.

        Args:
            ptr: Pointer to existing memory.
            size: New size in bytes.

        Returns:
            Pointer to reallocated memory.
        """
        ...

    def Copy(self, dst: Ptr[Void], src: Ptr[Void], size: UInt32) -> Ptr[Void]:
        """Copy memory from source to destination.

        Args:
            dst: Destination pointer.
            src: Source pointer.
            size: Number of bytes to copy.

        Returns:
            Destination pointer.
        """
        ...

    def Fill(self, dst: Ptr[Void], value: Char, size: UInt32) -> Ptr[Void]:
        """Fill memory with a byte value.

        Args:
            dst: Destination pointer.
            value: Byte value to fill with.
            size: Number of bytes to fill.

        Returns:
            Destination pointer.
        """
        ...

    def Compare(self, first: Ptr[Void], second: Ptr[Void], size: UInt32) -> Int16:
        """Compare two memory blocks.

        Args:
            first: First memory block.
            second: Second memory block.
            size: Number of bytes to compare.

        Returns:
            0 if equal, negative if first < second, positive if first > second.
        """
        ...

    def RetrieveSize(self, ptr: Ptr[Void]) -> UInt32:
        """Get size of allocated block.

        Args:
            ptr: Pointer to allocated memory.

        Returns:
            Size of the block in bytes.
        """
        ...
