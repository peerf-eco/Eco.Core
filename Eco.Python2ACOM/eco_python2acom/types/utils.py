"""Utilities for EcoOS/ACOM types structures."""

import ctypes
from typing import Any, TypeVar

U = TypeVar("U")


# -----------------------------------------------------------------------------
# Function helpers
# -----------------------------------------------------------------------------


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


def memmove(dst: Any, src: Any, size: int) -> None:
    """Copy `size` bytes from `src` to `dst`.

    Args:
        dst: Destination address or destination object.
        src: Source address or source object.
        size: Number of bytes to copy.
    """
    ctypes.memmove(dst, src, size)


def memset(dst: Any, byte: int, size: int) -> None:
    """Fill `size` bytes at `dst` with `byte`.

    Args:
        dst: Destination address or destination object.
        byte: Byte value to write (0–255).
        size: Number of bytes to fill.
    """
    ctypes.memset(dst, byte, size)


__all__ = [
    "cast",
    "byref",
    "addressof",
    "sizeof",
    "offsetof",
    "memmove",
    "memset",
]
