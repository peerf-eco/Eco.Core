"""Decorator for defining ACOM components implemented in Python.

Use this decorator on classes nested with one or more `@view` declarations
to compose a component with an associated CID and full `IEcoUnknown` triple.
"""

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.decorators.server.utils import (
    _build_layout_class,
    _collect_views,
    _make_install_vtables,
)
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

C = TypeVar("C", bound=type)


def _make_component_init(cls: type, install_vtables: Callable[..., None]) -> Callable[..., Int16]:
    """Build the `__eco_new__` that wires vtables and seeds the refcount.

    Args:
        cls: The user-decorated class — its `__eco_new__` is wrapped.
        install_vtables: Helper that points each `_vtbl_*` field at its singleton vtable.
    """
    original = cls.__dict__["__eco_new__"]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Install vtables, set the initial refcount, then run user allocation."""
        install_vtables(self)
        self.refs = 1
        return original(self, system, outer)

    return __eco_new__


def component(cid: Union[str, bytes, UGUID], preamble: int = 0x01) -> Callable[[C], C]:
    """Decorator for defining ACOM components implemented in Python.

    Converts a class with nested `@view` declarations into an EcoOS component
    class, validates the lifecycle hooks, and wires the `IEcoUnknown` triple.

    Args:
        cid: Component identifier as a string, bytes, or pre-built `UGUID`.
        preamble: Preamble byte for UGUID when `cid` is a string.

    Returns:
        A decorator that turns the class into an EcoOS component class.

    Raises:
        TypeError: If the class declares no `@view`s, or two views target the same interface.
        NotImplementedError: If `__eco_new__`, `__eco_init__`, or `__eco_del__` is missing.
    """

    def decorator(cls: C) -> C:
        """Convert `cls` into a Python ACOM component."""
        if isinstance(cid, UGUID):
            guid = cid
        else:
            guid = UGUID(cid, preamble=preamble)

        views = _collect_views(cls)
        if not views:
            raise TypeError(f"Component '{cls.__name__}' must declare at least one '@view'")

        for hook in ("__eco_new__", "__eco_init__", "__eco_del__"):
            if hook not in cls.__dict__ or not inspect.isfunction(cls.__dict__[hook]):
                raise NotImplementedError(
                    f"Component '{cls.__name__}' must define '{hook}' as a method"
                )

        # Build the layout (vtable fields, refs, user fields)
        new_cls, vtables = _build_layout_class(cls, views)

        # Wrap user `__eco_new__` so vtables and refcount are set before user code
        new_cls.__eco_new__ = _make_component_init(cls, _make_install_vtables(views, vtables))

        new_cls._cid_ = guid
        new_cls._eco_component_ = True
        return new_cls  # type: ignore

    return decorator


__all__ = ["component"]
