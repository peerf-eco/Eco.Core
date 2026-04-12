"""Generic smart pointer implementation for EcoOS/ACOM.

This module provides the Ptr[T] generic type for type-safe pointer operations.
"""

from __future__ import annotations

from typing import TYPE_CHECKING, Any, Generic, TypeVar

from eco_python2acom.types.core import TYPE_NAMES, Void, VoidPtr
from eco_python2acom.types.utils import addressof, pointer_type

__all__ = ["Ptr"]

T = TypeVar("T")


if TYPE_CHECKING:

    class Ptr(Generic[T]):
        """Generic smart pointer type.

        - For `Ptr[Void]`, the primary way to access the pointer is property `value`.
        - For other types, use property `contents` instead.

        Type Parameters:
            T: The type being pointed to.
        """

        _type_: type[T]

        def __init__(self, value: int | T | None = None) -> None:
            """Initialize pointer.

            Args:
                value: Optional value to point to. Can be:
                    - None: creates NULL pointer
                    - int: creates pointer with this address (for Void)
                    - Instance of T: creates pointer to that instance
                    - Value convertible to T: wraps and creates pointer
            """
            ...

        @property
        def contents(self) -> T:
            """Dereference pointer to access the pointed-to value.

            Returns:
                The dereferenced value of type T.
            """
            ...

        @contents.setter
        def contents(self, value: T) -> None:
            """Set the pointed-to value.

            Args:
                value: New value to store at pointer location.

            Raises:
                AttributeError: If T is Void.
            """
            ...

        @property
        def value(self) -> int | None:
            """Get raw pointer address.

            Returns:
                Integer address or None for NULL.
            """
            ...

        @value.setter
        def value(self, addr: int | None) -> None:
            """Set raw pointer address.

            Args:
                addr: New address or None for NULL.
            """
            ...

        def __bool__(self) -> bool:
            """Check if pointer is non-NULL.

            Returns:
                True if pointer is non-NULL, False otherwise.
            """
            ...

        def __repr__(self) -> str:
            """Return string representation.

            Returns:
                String showing pointer type and address or NULL.
            """
            ...

else:

    class _PtrMeta(type):
        """Metaclass that enables Ptr[T] subscript syntax.

        Handles special cases:
            - Ptr[Void]: Returns wrapper for void pointers (with `.value`)
            - Ptr[T]: Returns wrapper for pointers to T (with `.contents`)
            - Ptr[Ptr[T]]: Recursive pointers work correctly

        Attributes:
            _cache: Dictionary mapping types to their pointer classes.
        """

        _cache: dict[type, type] = {}

        def __getitem__(cls, item: type[T]) -> type:
            """Create or retrieve a pointer type for the given type.

            Args:
                item: The type to create a pointer for.

            Returns:
                A pointer class for the specified type.

            Raises:
                TypeError: If item cannot be made into a pointer.
            """
            # Check cache first
            if item in cls._cache:
                return cls._cache[item]

            # Special case: Ptr[Void] wraps c_void_p (no .contents)
            if item is Void:

                class SmartVoidPtr(VoidPtr):
                    """Smart void pointer wrapper (no `.contents` - void has no size)."""

                    def __repr__(self) -> str:
                        """String representation with address."""
                        if self.value is not None:
                            return f"<Ptr[Void] 0x{self.value:X}>"
                        return "<Ptr[Void] NULL>"

                    def __eq__(self, other: object) -> bool:
                        """Compare two pointers for equality.

                        Comparison is done on the value of the pointers.
                        """
                        if not isinstance(other, SmartVoidPtr):
                            return NotImplemented
                        return self.value == other.value

                    def __hash__(self) -> int:
                        """Hash the pointer."""
                        return hash(self.value)

                SmartVoidPtr.__name__ = "Ptr[Void]"
                SmartVoidPtr.__qualname__ = "Ptr[Void]"

                cls._cache[item] = SmartVoidPtr
                return SmartVoidPtr

            # Get display name
            type_name = TYPE_NAMES.get(item, getattr(item, "__name__", str(item)))

            # Create pointer type
            try:
                ptr_type = pointer_type(item)
            except TypeError as err:
                raise TypeError(f"Cannot create pointer to {item}: {err}") from err

            # Create smart pointer wrapper class
            class SmartPtr(ptr_type):
                """Runtime smart pointer implementation."""

                _type_ = item

                def __init__(self, value: Any | None = None) -> None:
                    """Initialize pointer from value."""
                    if value is None:
                        super().__init__()
                    elif isinstance(value, item):
                        super().__init__(value)
                    else:
                        try:
                            super().__init__(item(value))
                        except Exception:
                            super().__init__()

                def __repr__(self) -> str:
                    """String representation with address."""
                    try:
                        addr = addressof(self.contents)
                        return f"<Ptr[{type_name}] 0x{addr:X}>"
                    except Exception:
                        return f"<Ptr[{type_name}] NULL>"

                def __eq__(self, other: object) -> bool:
                    """Compare two pointers for equality.

                    Comparison is done on the value of the pointers.
                    """
                    if not isinstance(other, SmartPtr):
                        return NotImplemented
                    return addressof(self.contents) == addressof(other.contents)

                def __hash__(self) -> int:
                    """Hash the pointer."""
                    return hash(addressof(self.contents))

            SmartPtr.__name__ = f"Ptr[{type_name}]"
            SmartPtr.__qualname__ = f"Ptr[{type_name}]"

            # Cache and return
            cls._cache[item] = SmartPtr
            return SmartPtr

    class Ptr(metaclass=_PtrMeta):
        """Generic smart pointer type.

        This is the runtime class that uses _PtrMeta to enable Ptr[T] syntax.
        """

        pass
