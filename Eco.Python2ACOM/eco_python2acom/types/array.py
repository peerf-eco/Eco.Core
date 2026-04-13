"""Generic fixed-size array implementation for EcoOS/ACOM.

This module provides the Array[T, N] generic type for type-safe fixed-size
array operations.
"""

from __future__ import annotations

from collections.abc import Iterable, Iterator
from typing import TYPE_CHECKING, Generic, TypeVar, overload

from eco_python2acom.types.core import TYPE_NAMES, CLayout
from eco_python2acom.types.utils import addressof

T = TypeVar("T")
N = TypeVar("N", bound=int)


if TYPE_CHECKING:

    class Array(Generic[T, N]):
        """Generic fixed-size array with type safety.

        Type Parameters:
            T: The element type.
            N: The array size.

        Attributes:
            _element_type_: The type of array elements.
            _size_: The number of elements in the array.
        """

        _element_type_: type[T]
        _size_: N

        def __init__(self, *values: T) -> None:
            """Initialize array with optional values.

            Args:
                *values: Initial values for array elements. If fewer values
                    are provided than the array size, remaining elements are
                    zero-initialized.

            Examples:
                >>> arr = Array[Int32, 5](1, 2, 3, 4, 5)
            """
            ...

        @overload
        def __getitem__(self, index: int) -> T:
            ...

        @overload
        def __getitem__(self, index: slice) -> list[T]:
            ...

        def __getitem__(self, index: int | slice) -> T | list[T]:
            """Get element(s) by index or slice.

            Args:
                index: Index or slice.

            Returns:
                A single element or a list of elements.
            """
            ...

        @overload
        def __setitem__(self, index: int, value: T) -> None:
            ...

        @overload
        def __setitem__(self, index: slice, value: Iterable[T]) -> None:
            ...

        def __setitem__(self, index: int | slice, value: T | Iterable[T]) -> None:
            """Set element(s) by index or slice.

            Args:
                index: Index or slice.
                value: Value or iterable of values.
            """
            ...

        def __len__(self) -> int:
            """Get array length.

            Returns:
                The number of elements in the array.
            """
            ...

        def __iter__(self) -> Iterator[T]:
            """Iterate over array elements.

            Returns:
                An iterator over elements.
            """
            ...

        def __bytes__(self) -> bytes:
            """Convert array to bytes.

            Returns:
                Byte representation of the array.
            """
            ...

        def __repr__(self) -> str:
            """Return string representation.

            Returns:
                String showing array type, size, and address.
            """
            ...

else:

    class _ArrayMeta(type):
        """Metaclass that enables Array[T, N] subscript syntax.

        This metaclass intercepts `__getitem__` calls on the `Array` class to
        dynamically create typed array classes. It maintains a cache to avoid
        recreating the same array type multiple times.

        The subscript must be a tuple of (type, size), where size is a
        non-negative integer.

        Attributes:
            _cache: Dictionary mapping (type, size) tuples to array classes.
        """

        _cache: dict[tuple[type, int], type] = {}

        def __getitem__(cls, params: tuple[type, int]) -> type:
            """Create or retrieve an array type for the given type and size.

            Args:
                params: A tuple of (element_type, size) where:
                    - element_type: The type of array elements
                    - size: The number of elements (non-negative integer)

            Returns:
                An array class for the specified type and size.

            Raises:
                TypeError: If params is not a 2-tuple or size is invalid.

            Examples:
                >>> IntArray5 = Array[Int32, 5]
                >>> arr = IntArray5(1, 2, 3, 4, 5)
            """
            # Validate parameters
            if not isinstance(params, tuple) or len(params) != 2:
                raise TypeError(f"Array requires 2 parameters: Array[Type, Size], got {params}")

            element_type, size = params

            if not isinstance(size, int) or size < 0:
                raise TypeError(f"Array size must be a non-negative integer, got {size}")

            if not isinstance(element_type, type):
                raise TypeError(f"Array element type must be a type, got {element_type}")

            if issubclass(element_type, CLayout) and element_type not in CLayout.__args__:
                from eco_python2acom.decorators.utils import finalize

                finalize(element_type)

            # Check cache
            key = (element_type, size)
            if key in cls._cache:
                return cls._cache[key]

            # Create array type
            try:
                array_type = element_type * size
            except TypeError as err:
                raise TypeError(f"Cannot create array of {element_type}: {err}") from err

            # Get display name
            type_name = TYPE_NAMES.get(
                element_type, getattr(element_type, "__name__", str(element_type))
            )

            # Create smart array wrapper class
            class SmartArray(array_type):
                """Runtime array implementation."""

                _element_type_ = element_type
                _size_ = size

                def __repr__(self) -> str:
                    """String representation with type and address."""
                    try:
                        addr = addressof(self)
                        return f"<Array[{type_name}, {size}] 0x{addr:X}>"
                    except Exception:
                        return f"<Array[{type_name}, {size}]>"

                def __eq__(self, other: object) -> bool:
                    """Compare two arrays for equality.

                    Comparison is done on the size and contents of the arrays.
                    """
                    if not isinstance(other, SmartArray):
                        return NotImplemented
                    if self._size_ != other._size_:
                        return False
                    for i in range(self._size_):
                        if self[i] != other[i]:
                            return False
                    return True

                def __iter__(self):
                    """Iterate over array elements."""
                    for i in range(self._size_):
                        yield self[i]

                def __bytes__(self) -> bytes:
                    """Convert array to bytes."""
                    return bytes(int(self[i]) for i in range(self._size_))

                def __hash__(self) -> int:
                    """Hash the array."""
                    raise NotImplementedError("Hashing is not supported for arrays")

            SmartArray.__name__ = f"Array[{type_name}, {size}]"
            SmartArray.__qualname__ = f"Array[{type_name}, {size}]"

            # Cache and return
            cls._cache[key] = SmartArray
            return SmartArray

    class Array(metaclass=_ArrayMeta):
        """Generic fixed-size array type.

        This is the runtime class that uses `_ArrayMeta` to enable
        Array[T, N] syntax.

        Note:
            Arrays are fixed-size and allocated on creation. Use Ptr[Array[T, N]]
            for dynamic array pointers.
        """

        pass


__all__ = ["Array"]
