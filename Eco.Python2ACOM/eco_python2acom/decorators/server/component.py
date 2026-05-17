"""Decorator for defining ACOM components implemented in Python.

Use this decorator on classes nested with one or more `@view` declarations
to compose a component with an associated CID and full `IEcoUnknown` triple.

A full `@component` class must define all three lifecycle hooks the factory
drives during `Alloc`:
  - `__eco_new__(self, system, outer) -> Int16`: allocation phase.
  - `__eco_init__(self, system) -> Int16`: initialisation phase.
  - `__eco_del__(self) -> Void`: cleanup phase.

An internal helper components must define only `__eco_new__` and `__eco_del__`.
Their signatures are not enforced.
"""

import inspect
from collections.abc import Callable
from typing import Optional, TypeVar, Union

from eco_python2acom.decorators.server.utils import _eco_server_class, _make_vtbls_installer
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer

C = TypeVar("C", bound=type)


def _make_component_new(
    cls: type, vtbls_installer: Callable[..., None], aggregatable: bool, internal: bool
) -> Callable[..., Int16]:
    """Build the `__eco_new__` that wires vtables, refcount and (when
    aggregatable) the `outer` self-loop.

    Args:
        cls: The user-decorated class — its `__eco_new__` is wrapped.
        vtbls_installer: Helper that points each `_vtbl_*` field at its singleton vtable.
        aggregatable: Whether the component can participate in aggregation as the inner part.
        internal: True for helper components called by hand with arbitrary arguments.
    """
    original = cls.__dict__["__eco_new__"]

    if internal:

        def __eco_new__(self, *args, **kwargs) -> Int16:
            """Install vtables and refcount, then forward to user allocation as-is."""
            vtbls_installer(self)
            self.refs = 1
            return original(self, *args, **kwargs)

    else:

        def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:  # type: ignore
            """Install vtables, set the initial refcount, then run user allocation."""
            vtbls_installer(self)
            self.refs = 1
            if aggregatable:
                if bool(outer):
                    self.outer = outer
                else:
                    # Self-loop: route delegating IEcoUnknown calls back into our NDU
                    self.outer = pointer(self, IEcoUnknown, shift=True)
            return original(self, system, outer)

    return __eco_new__


def component(
    cid: Optional[Union[str, bytes, UGUID]] = None,
    preamble: int = 0x01,
    aggregatable: bool = False,
) -> Callable[[C], C]:
    """Decorator for defining ACOM components implemented in Python.

    Converts a class with nested `@view` declarations into an EcoOS component
    class, validates the lifecycle hooks, and wires the `IEcoUnknown` triple.

    A `cid` is required only for components that are exposed through `@factory`.
    Pass `None` for internal helper components instantiated by hand inside
    another component's lifecycle — sinks, connection points, enumerators, etc.

    Args:
        cid: Component identifier as a string, bytes, or pre-built `UGUID`.
            When `None`, the component is treated as an internal helper.
        preamble: Preamble byte for UGUID when `cid` is a string.
        aggregatable: Whether the component can participate in aggregation as the inner part.

    Returns:
        A decorator that turns the class into an EcoOS component class.

    Raises:
        TypeError: If the class declares no `@view`s, or two views target the same interface.
        NotImplementedError: If a required lifecycle hook is missing.
    """

    def decorator(cls: C) -> C:
        """Convert `cls` into a Python ACOM component."""
        if cid is None:
            guid = None
        elif isinstance(cid, UGUID):
            guid = cid
        else:
            guid = UGUID(cid, preamble=preamble)

        hooks = ("__eco_new__", "__eco_del__")
        if guid is not None:
            hooks += ("__eco_init__",)
        for hook in hooks:
            if hook not in cls.__dict__ or not inspect.isfunction(cls.__dict__[hook]):
                raise NotImplementedError(
                    f"Component '{cls.__name__}' must define '{hook}' as a method"
                )

        new_cls, vtbls = _eco_server_class(cls, aggregatable=aggregatable)
        vtbls_installer = _make_vtbls_installer(new_cls._eco_views_, vtbls)

        # Wrap user `__eco_new__` so vtables and refcount are set before user code
        new_cls.__eco_new__ = _make_component_new(
            cls, vtbls_installer, aggregatable, internal=guid is None
        )

        new_cls._cid_ = guid
        new_cls._eco_component_ = True
        new_cls._eco_aggregatable_ = aggregatable
        return new_cls  # type: ignore

    return decorator


__all__ = ["component"]
