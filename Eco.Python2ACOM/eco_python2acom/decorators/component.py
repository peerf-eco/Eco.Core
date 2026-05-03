"""Decorator for defining ACOM components implemented in Python.

Use this decorator on a class that inherits from one or more `@interface`
classes and provides Python implementations of their methods.
"""

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.types.guid import UGUID

C = TypeVar("C", bound=type)


def _collect_interfaces(cls: type) -> list[type]:
    """Walk the MRO and collect all `@interface`-decorated bases.

    Args:
        cls: The component class being decorated.

    Returns:
        Interfaces that the component declares it implements in MRO order.
    """
    interfaces = []
    for base in cls.__mro__:
        if base is cls or base is object:
            continue
        if getattr(base, "_eco_interface_", False):
            interfaces.append(base)
    return interfaces


def _validate_implementation(cls: type, interfaces: list[type]) -> list[type]:
    """Verify the component implements every method of its interfaces.

    Args:
        cls: The component class being decorated.
        interfaces: Interfaces collected from the MRO.

    Returns:
        The list of interfaces if validation is successful.

    Raises:
        TypeError: If two interfaces declare a method with the same name, excluding the `IEcoUnknown` triple.
        NotImplementedError: If a method is missing or not callable on the component.
    """
    ieco_unknown = {"QueryInterface", "AddRef", "Release"}
    seen: dict[str, type] = {}

    for iface in interfaces:
        for field_name in getattr(iface, "_eco_method_params_", {}):
            method_name = field_name.removeprefix("_func_")
            if (
                method_name in seen
                and seen[method_name] is not iface
                and method_name not in ieco_unknown
            ):
                raise TypeError(
                    f"Method '{method_name}' is declared by both "
                    f"'{seen[method_name].__name__}' and '{iface.__name__}'"
                )
            value = getattr(cls, method_name, None)
            if value is None or not inspect.isfunction(value):
                raise NotImplementedError(
                    f"Component '{cls.__name__}' does not implement '{method_name}'"
                )
            seen[method_name] = iface

    return interfaces


def component(cid: Union[str, bytes, UGUID], preamble: int = 0x01) -> Callable[[C], C]:
    """Decorator for defining ACOM components implemented in Python.

    Converts a class into an ACOM component class by verifying it implements
    every method of every interface it inherits from, including the `IEcoUnknown` triple.

    Args:
        cid: Component identifier as a string, bytes, or pre-built `UGUID`.
        preamble: Preamble byte for UGUID when `cid` is a string.

    Returns:
        A decorator that turns the class into an EcoOS component class.

    Raises:
        TypeError: If the class does not inherit from any `@interface`, or
            declares conflicting method names across interfaces.
        NotImplementedError: If class omits an implementation for any interface method.
    """

    def decorator(cls: C) -> C:
        """Converts the decorated class into an ACOM component class."""

        if isinstance(cid, UGUID):
            guid = cid
        else:
            guid = UGUID(cid, preamble=preamble)

        interfaces = _validate_implementation(cls, _collect_interfaces(cls))
        if not interfaces:
            raise TypeError(
                f"Component '{cls.__name__}' must inherit from at least one '@interface' class"
            )

        cls._eco_component_ = True
        cls._cid_ = guid
        return cls

    return decorator


__all__ = ["component"]
