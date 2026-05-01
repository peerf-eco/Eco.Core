"""Decorator for defining ACOM components implemented in Python.

Use this decorator on a class that inherits from one or more `@interface`
classes and provides Python implementations of their methods.

There are three required lifecycle hooks that the component class must implement:

- `__eco_new__` — allocation phase;
- `__eco_init__` — initialisation phase;
- `__eco_del__` — cleanup phase;
"""

import inspect
from collections.abc import Callable
from dataclasses import dataclass
from typing import TypeVar, Union

from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof

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
    """Verify the component implements every business method of its interfaces.

    Args:
        cls: The component class being decorated.
        interfaces: Interfaces collected from the MRO.

    Returns:
        The list of interfaces if validation is successful.

    Raises:
        TypeError: If two interfaces declare a non-built-in method with the same name.
        NotImplementedError: If a method is missing or not callable on the component.
    """
    builtin = {"QueryInterface", "AddRef", "Release"}
    seen: dict[str, type] = {}

    for iface in interfaces:
        for field_name in getattr(iface, "_eco_method_params_", {}):
            method_name = field_name.removeprefix("_func_")
            if method_name in builtin:
                continue
            if method_name in seen and seen[method_name] is not iface:
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


# -----------------------------------------------------------------------------
# IEcoUnknown methods
# -----------------------------------------------------------------------------


def _make_query_interface(supported: set[UGUID]) -> Callable[..., Int16]:
    """Build a `QueryInterface` for a component class.

    Args:
        supported: A set of UGUIDs representing the IIDs that this component supports.
    """

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for an interface supported by this component.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out):
            return Int16(EcoErrorCode.POINTER)
        if iid.obj in supported:
            out.obj.value = addressof(self)
            self.AddRef()
            return Int16(EcoErrorCode.SUCCESS)
        out.obj.value = 0
        return Int16(EcoErrorCode.NOINTERFACE)

    return QueryInterface


def _make_add_ref() -> Callable[..., UInt32]:
    """Build an `AddRef` for a component class."""

    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        self.refs += 1
        return UInt32(self.refs)

    return AddRef


def _make_release() -> Callable[..., UInt32]:
    """Build a `Release` for a component class."""

    def Release(self) -> UInt32:
        """Decrement the reference count.

        When the count reaches zero, the component is freed.

        Returns:
            The new reference count.
        """
        self.refs -= 1
        if self.refs == 0:
            self.__eco_del__()
        return UInt32(self.refs)

    return Release


# -----------------------------------------------------------------------------
# Decorator
# -----------------------------------------------------------------------------


def component(cid: Union[str, bytes, UGUID], preamble: int = 0x01) -> Callable[[C], C]:
    """Decorator for defining ACOM components implemented in Python.

    Converts a class into an ACOM component class by verifying it implements
    the required lifecycle hooks and interface methods, then injecting `QueryInterface`,
    `AddRef`, and `Release` implementations that manage the component's reference
    counting and interface querying based on the interfaces it implements.

    Args:
        cid: Component identifier as a string, bytes, or pre-built `UGUID`.
        preamble: Preamble byte for UGUID when `cid` is a string.

    Returns:
        A decorator that turns the class into an EcoOS component class.

    Raises:
        TypeError: If the class does not inherit from any `@interface`,
            declares conflicting method names across interfaces.
        NotImplementedError: If class omits an implementation for any interface
            method, or omits any of the required lifecycle hooks.
    """

    def decorator(cls: C) -> C:
        """Converts the decorated class into an ACOM component class.

        Applies CID metadata, collects interfaces and methods, validates the implementation,
        and injects `QueryInterface`, `AddRef`, and `Release` methods.
        """

        if isinstance(cid, UGUID):
            guid = cid
        else:
            guid = UGUID(cid, preamble=preamble)

        interfaces = _validate_implementation(cls, _collect_interfaces(cls))
        if not interfaces:
            raise TypeError(
                f"Component '{cls.__name__}' must inherit from at least one '@interface' class"
            )

        for hook in ("__eco_new__", "__eco_init__", "__eco_del__"):
            if hook not in cls.__dict__ or not inspect.isfunction(cls.__dict__[hook]):
                raise NotImplementedError(
                    f"Component '{cls.__name__}' must define '{hook}' as a method"
                )

        supported: set[UGUID] = {iface._iid_ for iface in interfaces} | {IID_IEcoUnknown}
        cls.QueryInterface = _make_query_interface(supported)
        cls.AddRef = _make_add_ref()
        cls.Release = _make_release()

        cls._eco_component_ = True
        cls._cid_ = guid
        cls = dataclass(init=False)(cls)
        return cls

    return decorator


__all__ = ["component"]
