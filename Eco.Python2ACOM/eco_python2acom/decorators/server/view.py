"""Decorator for grouping the implementation of one interface inside a component.

Use this decorator on classes nested inside an `@component` to declare the
methods of a single ACOM interface.
"""

import inspect
from typing import TypeVar

C = TypeVar("C", bound=type)


def _resolve_iface(cls: type) -> type:
    """Find the single `@interface` base of a view class.

    Args:
        cls: The class being decorated.

    Returns:
        The interface this view groups methods for.

    Raises:
        TypeError: If the class declares zero or more than one `@interface` base.
    """
    ifaces = [base for base in cls.__bases__ if getattr(base, "_eco_interface_", False)]
    if len(ifaces) != 1:
        raise TypeError(
            f"View '{cls.__name__}' must inherit from exactly one interface, but inherits from '{len(ifaces)}'"
        )
    return next(iter(ifaces))


def _validate_implementation(cls: type, iface: type) -> None:
    """Verify the view declares every business method of its interface.

    Args:
        cls: The view class.
        iface: The interface the view implements.

    Raises:
        NotImplementedError: If a business method is missing or not callable.
    """
    ieco_unknown = {"QueryInterface", "AddRef", "Release"}

    for field_name in getattr(iface, "_eco_method_params_", {}):
        method_name = field_name.removeprefix("_func_")
        if method_name in ieco_unknown:
            continue
        value = cls.__dict__.get(method_name)
        if value is None or not inspect.isfunction(value):
            raise NotImplementedError(f"View '{cls.__name__}' does not implement '{method_name}'")


def view(cls: C) -> C:
    """Decorator that marks a class as a view inside an `@component`.

    Args:
        cls: The class being decorated. Must inherit from exactly one `@interface`
            and implement every business method of that interface.

    Returns:
        The decorated class.

    Raises:
        TypeError: If the class does not inherit from exactly one `@interface`.
        NotImplementedError: If a business method of the interface is missing.
    """
    iface = _resolve_iface(cls)
    _validate_implementation(cls, iface)

    cls._eco_view_ = True
    cls._eco_iface_ = iface
    return cls


__all__ = ["view"]
