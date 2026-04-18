"""Generic smart pointer implementation for EcoOS/ACOM.

This module provides the `Ptr[T]` generic type for type-safe pointer operations.
"""

from typing import TYPE_CHECKING, Any, Generic, TypeVar

from eco_python2acom.types.core import TYPE_NAMES, Void, VoidPtr
from eco_python2acom.types.utils import addressof, pointer_type

T = TypeVar("T")


if TYPE_CHECKING:

    class Ptr(Generic[T]):
        """Generic smart pointer type.

        - For `Ptr[Void]`, the primary way to access the pointer is property `value`.
        - For other types, use property `obj` to dereference.

        Type Parameters:
            T: The type being pointed to.
        """

        _type_: type[T]

        def __init__(self, value: int | T | None = None) -> None:
            """Initialize pointer.

            Args:
                value: Optional value to point to. Can be:
                    - None: creates NULL pointer
                    - int: creates pointer with this address (for `Void`)
                    - Instance of T: creates pointer to that instance
                    - Value convertible to T: wraps and creates pointer
            """
            ...

        @property
        def obj(self) -> T:
            """Dereference pointer to access the pointed-to value.

            Returns:
                The dereferenced value of type T.
            """
            ...

        @obj.setter
        def obj(self, value: T) -> None:
            """Set the pointed-to value.

            Args:
                value: New value to store at pointer location.

            Raises:
                AttributeError: If T is `Void`.
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
            - Ptr[T]: Returns wrapper for pointers to T (with `.obj`)
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
            """
            if not isinstance(item, type):
                raise TypeError(f"Ptr element type must be a type, got '{type(item).__name__}'")

            # Check cache first
            if item in cls._cache:
                return cls._cache[item]

            # Special case: Ptr[Void]
            if item is Void:

                class SmartVoidPtr(VoidPtr):
                    """Smart void pointer wrapper (no `.obj` - void has no size)."""

                    def __repr__(self) -> str:
                        """String representation with address."""
                        if self.value is not None:
                            return f"<Ptr[Void] 0x{self.value:X}>"
                        return "<Ptr[Void] NULL>"

                    def __eq__(self, other: object) -> bool:
                        """Compare two void pointers by address."""
                        if not isinstance(other, SmartVoidPtr):
                            return NotImplemented
                        return self.value == other.value

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
                raise ValueError(f"Cannot create pointer to '{type_name}': {err}") from err

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
                        except Exception as err:
                            raise ValueError(
                                f"Cannot initialize Ptr[{type_name}] from '{type(value).__name__}'"
                            ) from err

                @property
                def obj(self) -> T:
                    """Dereference pointer to access the pointed-to value."""
                    return self.contents

                @obj.setter
                def obj(self, value: T) -> None:
                    """Set the pointed-to value."""
                    self.contents = value

                def __repr__(self) -> str:
                    """String representation with address."""
                    if not bool(self):
                        return f"<Ptr[{type_name}] NULL>"
                    return f"<Ptr[{type_name}] 0x{addressof(self.contents):X}>"

                def __eq__(self, other: object) -> bool:
                    """Compare two pointers by address."""
                    if not isinstance(other, SmartPtr):
                        return NotImplemented
                    self_null = not bool(self)
                    other_null = not bool(other)
                    if self_null or other_null:
                        return self_null and other_null
                    return addressof(self.contents) == addressof(other.contents)

            SmartPtr.__name__ = f"Ptr[{type_name}]"
            SmartPtr.__qualname__ = f"Ptr[{type_name}]"

            # Cache and return
            cls._cache[item] = SmartPtr
            return SmartPtr

    class Ptr(metaclass=_PtrMeta):
        """Generic smart pointer type.

        `Ptr[T]` is the Python equivalent of a C pointer `T*`. Subscripting
        creates a concrete pointer class.

        Type Parameters:
            T: The type being pointed to (EcoOS-compatible type).

        Attributes:
            _type_: The pointed-to type.

        - `Ptr[Void]` — void pointer; use `.value` to read/write the raw address.
        - `Ptr[T]` — typed pointer; use `.obj` to dereference and access the value.
        - `Ptr[Ptr[T]]` — double pointer; works recursively.

        Example:
            ```python
            from eco_python2acom.types.pointer import Ptr
            from eco_python2acom.types.core import Int32, Void

            # Typed pointer
            x = Int32(42)
            p = Ptr[Int32](x)
            print(p.obj.value)   # 42

            # Void pointer (raw address only)
            vp = Ptr[Void]()
            print(bool(vp))      # False — NULL

            # Double pointer (output parameter pattern)
            ppv = Ptr[Void]()
            bus.obj.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
            ```
        """

        pass


__all__ = ["Ptr"]
