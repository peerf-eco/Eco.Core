"""Decorator for defining ACOM component factories implemented in Python.

A factory is a small companion class for every `@component`-decorated class.
It implements `IEcoComponentFactory` and is the single entry point for creating
instances of the component.
"""

from collections.abc import Callable
from typing import Any, TypeVar

from eco_python2acom.decorators.server.utils import _eco_server_class, _make_vtbls_installer
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.types.utils import addressof

C = TypeVar("C", bound=type)


def _make_factory_init(
    vtbls_installer: Callable[..., None], metadata: dict[str, Any]
) -> Callable[..., None]:
    """Build an `__init__` that wires vtables and seeds metadata fields.

    Args:
        vtbls_installer: Helper that points each `_vtbl_*` field at its singleton vtable.
        metadata: `name` / `version` / `manufacturer` defaults captured from the user class.
    """

    def __init__(self) -> None:
        """Wire vtables, seed metadata, set refcount to zero."""
        vtbls_installer(self)
        for field_name, value in metadata.items():
            try:
                setattr(self, field_name, value)
            except TypeError as err:
                raise TypeError(f"Cannot assign default {value!r} to field '{field_name}'") from err
        self.refs = 0

    return __init__


def factory(component: type) -> Callable[[C], C]:
    """Decorator for defining ACOM component factories implemented in Python.

    Converts a class into an `IEcoComponentFactory` implementation that creates
    instances of the specified component class. The factory is a static
    singleton — it has no lifecycle hooks and is never torn down.

    Args:
        component: The `@component`-decorated class this factory creates.

    Returns:
        A decorator that turns the class into an EcoOS factory class.

    Raises:
        TypeError: If `component` is not decorated with `@component`, has no
            CID assigned, or the decorated class does not inherit from
            `IEcoComponentFactory`.
    """
    if not isinstance(component, type) or not getattr(component, "_eco_component_", False):
        raise TypeError(
            f"Class '{component.__name__}' is not a component and cannot be used in a factory"
        )

    if getattr(component, "_cid_", None) is None:
        raise TypeError(f"Component '{component.__name__}' has no CID and cannot have a factory")

    def decorator(cls: C) -> C:
        """Convert `cls` into an `IEcoComponentFactory` implementation."""
        if not issubclass(cls, IEcoComponentFactory):
            raise TypeError(f"Factory '{cls.__name__}' must inherit from 'IEcoComponentFactory'")

        # Capture metadata defaults from the user class before its rebuild
        metadata = {
            name: cls.__dict__[name]
            for name in ("name", "version", "manufacturer")
            if name in cls.__dict__
        }

        new_cls, vtbls = _eco_server_class(cls, product=component)
        vtbls_installer = _make_vtbls_installer(new_cls._eco_views_, vtbls)

        # Static singleton — make `__eco_del__` a no-op so inherited `Release` can call it safely
        new_cls.__eco_del__ = lambda self: None

        # Wrap `__init__` so vtables, metadata, and refcount are set before user code
        new_cls.__init__ = _make_factory_init(vtbls_installer, metadata)  # type: ignore

        new_cls._eco_factory_ = True
        new_cls._eco_component_cls_ = component
        return new_cls  # type: ignore

    return decorator


def export(cls: type) -> tuple[Any, Callable[[], int]]:
    """Build the module-level singleton and `get_component_factory` for a factory class.

    Args:
        cls: A class produced by `@factory(...)`.

    Returns:
        A tuple of `(instance, get_component_factory)` pair.

    Raises:
        TypeError: If `cls` was not decorated with `@factory`.
    """
    if not isinstance(cls, type) or not getattr(cls, "_eco_factory_", False):
        raise TypeError(f"Class '{cls.__name__}' is not a factory")

    instance = cls()

    def get_component_factory() -> int:
        """Return the native address of the module-level factory singleton."""
        return addressof(instance)

    return instance, get_component_factory


__all__ = ["factory", "export"]
