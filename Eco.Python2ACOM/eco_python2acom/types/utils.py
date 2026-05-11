"""Utilities for EcoOS/ACOM types structures."""

import ctypes
from typing import Any, TypeVar

from eco_python2acom.types.core import CPointer

U = TypeVar("U")


# -----------------------------------------------------------------------------
# Function helpers
# -----------------------------------------------------------------------------


def pointer_type(base_type: type[U]) -> type[CPointer]:
    """Create a pointer type for an EcoOS data type.

    Args:
        base_type: The EcoOS data type.

    Returns:
        A pointer type (class) for the given type.
    """
    return ctypes.POINTER(base_type)


def pointer(obj: U) -> CPointer:
    """Create a pointer to an EcoOS object.

    Args:
        obj: The EcoOS object.

    Returns:
        An EcoOS pointer to the object.
    """
    return ctypes.pointer(obj)


def cast(obj: Any, target_type: type[U]) -> U:
    """Cast an EcoOS pointer to a different type.

    Args:
        obj: The pointer or object to cast.
        target_type: The target EcoOS type.

    Returns:
        The object cast to the new type.
    """
    return ctypes.cast(obj, target_type)


def byref(obj: Any) -> Any:
    """Pass an EcoOS object by reference.

    Args:
        obj: The EcoOS object.

    Returns:
        A lightweight reference to the object.
    """
    return ctypes.byref(obj)


def addressof(obj: Any) -> int:
    """Get the memory address of an EcoOS object.

    Args:
        obj: The EcoOS object.

    Returns:
        Integer memory address.
    """
    return ctypes.addressof(obj)


def sizeof(obj_or_type: Any) -> int:
    """Get the size in bytes of a EcoOS type or object.

    Args:
        obj_or_type: A EcoOS type or instance.

    Returns:
        Size in bytes.
    """
    return ctypes.sizeof(obj_or_type)


def offsetof(obj_or_type: Any, iface: type) -> int:
    """Get the byte offset of an interface's vtable field within a server layout.

    Resolves the `_vtbl_<iface>` field on either a class or one of its instances.

    Args:
        obj_or_type: A `@component`/`@factory` class or one of its instances.
        iface: The interface whose vtable field is being looked up.

    Returns:
        Byte offset of `_vtbl_<iface.__name__>` within the structure.
    """
    cls = obj_or_type if isinstance(obj_or_type, type) else type(obj_or_type)
    return getattr(cls, f"_vtbl_{iface.__name__}").offset


__all__ = ["pointer_type", "pointer", "cast", "byref", "addressof", "sizeof", "offsetof"]
