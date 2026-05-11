"""Decorator for defining ACOM components implemented in Python.

Use this decorator on classes nested with one or more `@view` declarations
to compose a component with an associated CID and full `IEcoUnknown` triple.

Every `@component` class must define the three lifecycle hooks the factory
drives during `Alloc`:
  - `__eco_new__(self, system, outer) -> Int16`: allocation phase.
  - `__eco_init__(self, system) -> Int16`: initialisation phase.
  - `__eco_del__(self) -> Void`: cleanup phase.
"""

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.decorators.server.ndu import NonDelegatingUnknown
from eco_python2acom.decorators.server.utils import _eco_server_class, _make_vtbls_installer
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof, cast, offsetof

C = TypeVar("C", bound=type)


def _make_component_new(
    cls: type, vtbls_installer: Callable[..., None], aggregatable: bool
) -> Callable[..., Int16]:
    """Build the `__eco_new__` that wires vtables, refcount and (when
    aggregatable) the `outer` self-loop.

    Args:
        cls: The user-decorated class — its `__eco_new__` is wrapped.
        vtbls_installer: Helper that points each `_vtbl_*` field at its singleton vtable.
        aggregatable: Whether the component can participate in aggregation as the inner part.
    """
    original = cls.__dict__["__eco_new__"]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Install vtables, set the initial refcount, then run user allocation."""
        vtbls_installer(self)
        self.refs = 1
        if aggregatable:
            if bool(outer):
                self.outer = outer
            else:
                # Self-loop: route delegating IEcoUnknown calls back into our NDU.
                self.outer = cast(
                    addressof(self) + offsetof(self, NonDelegatingUnknown), Ptr[IEcoUnknown]
                )
        return original(self, system, outer)

    return __eco_new__


def component(
    cid: Union[str, bytes, UGUID], preamble: int = 0x01, aggregatable: bool = False
) -> Callable[[C], C]:
    """Decorator for defining ACOM components implemented in Python.

    Converts a class with nested `@view` declarations into an EcoOS component
    class, validates the lifecycle hooks, and wires the `IEcoUnknown` triple.

    Args:
        cid: Component identifier as a string, bytes, or pre-built `UGUID`.
        preamble: Preamble byte for UGUID when `cid` is a string.
        aggregatable: Whether the component can participate in aggregation as the inner part.

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

        for hook in ("__eco_new__", "__eco_init__", "__eco_del__"):
            if hook not in cls.__dict__ or not inspect.isfunction(cls.__dict__[hook]):
                raise NotImplementedError(
                    f"Component '{cls.__name__}' must define '{hook}' as a method"
                )

        new_cls, vtbls = _eco_server_class(cls, aggregatable=aggregatable)
        vtbls_installer = _make_vtbls_installer(new_cls._eco_views_, vtbls)

        # Wrap user `__eco_new__` so vtables and refcount are set before user code
        new_cls.__eco_new__ = _make_component_new(cls, vtbls_installer, aggregatable)

        new_cls._cid_ = guid
        new_cls._eco_component_ = True
        new_cls._eco_aggregatable_ = aggregatable
        return new_cls  # type: ignore

    return decorator


__all__ = ["component"]
