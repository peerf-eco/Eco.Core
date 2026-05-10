"""Decorator for defining ACOM component factories implemented in Python.

A factory is a small companion class for every `@component`-decorated class.
It implements `IEcoComponentFactory` and is the single entry point for creating
instances of the component.
"""

from collections.abc import Callable
from typing import Any, Optional, TypeVar

from eco_python2acom.decorators.server.utils import _build_layout_class, _make_install_vtables
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof

C = TypeVar("C", bound=type)


# -----------------------------------------------------------------------------
# IEcoComponentFactory methods
# -----------------------------------------------------------------------------


def _make_alloc(cls: type) -> Callable[..., Int16]:
    """Build the `Alloc` method for a factory class.

    Mirrors `CEcoXxxFactory` method `Alloc`:
      1. Reject aggregation requests for non-`IEcoUnknown` IIDs.
      2. Construct the component.
      3. Run the explicit allocation phase: `__eco_new__(system, outer)`.
      4. Run the explicit initialisation phase: `__eco_init__(system)`.
      5. Hand the requested interface to the caller via `QueryInterface`.
      6. Release the temporary reference held by this factory.

    Args:
        cls: The `@component`-decorated class this factory creates.
    """

    def Alloc(
        self,
        system: Optional[Ptr[IEcoUnknown]],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Allocate a new component instance.

        Args:
            system: Pointer to system interface (can be NULL).
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out) or not bool(system):
            return Int16(EcoErrorCode.POINTER)

        if bool(outer) and iid.obj != IID_IEcoUnknown:
            return Int16(EcoErrorCode.NOAGGREGATION)

        instance = cls()
        result = instance.__eco_new__(system, outer)
        if result.value != 0:
            return result

        result = instance.__eco_init__(system)
        if result.value != 0:
            return result

        result = instance.QueryInterface(iid, out)
        if result.value != 0:
            return result

        instance.Release()
        return Int16(EcoErrorCode.SUCCESS)

    return Alloc


def _make_init() -> Callable[..., Int16]:
    """Build the `Init` method for a factory class."""

    def Init(self, system: Optional[Ptr[IEcoUnknown]], context: Ptr[Void]) -> Int16:
        """Initialize the factory with system context.

        Args:
            system: Pointer to system interface (can be NULL).
            context: Additional context (e.g., bus pointer).

        Returns:
            0 on success, error code otherwise.
        """
        return Int16(EcoErrorCode.SUCCESS)

    return Init


def _make_get_name() -> Callable[..., CString]:
    """Build the `get_Name` method for a factory class."""

    def get_Name(self) -> CString:
        """Get the component name.

        Returns:
            Pointer to null-terminated string.
        """
        return self.name

    return get_Name


def _make_get_version() -> Callable[..., CString]:
    """Build the `get_Version` method for a factory class."""

    def get_Version(self) -> CString:
        """Get the component version.

        Returns:
            Pointer to null-terminated string.
        """
        return self.version

    return get_Version


def _make_get_manufacturer() -> Callable[..., CString]:
    """Build the `get_Manufacturer` method for a factory class."""

    def get_Manufacturer(self) -> CString:
        """Get the component manufacturer.

        Returns:
            Pointer to null-terminated string.
        """
        return self.manufacturer

    return get_Manufacturer


# -----------------------------------------------------------------------------
# Decorator
# -----------------------------------------------------------------------------


def _make_factory_init(
    install_vtables: Callable[..., None], metadata: dict[str, Any]
) -> Callable[..., None]:
    """Build an `__init__` that wires vtables and seeds metadata fields.

    Args:
        install_vtables: Helper that points each `_vtbl_*` field at its singleton vtable.
        metadata: `name` / `version` / `manufacturer` defaults captured from the user class.
    """

    def __init__(self) -> None:
        """Wire vtables, seed metadata, set refcount to zero."""
        install_vtables(self)
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
        TypeError: If `component` is not decorated with `@component`, or the
            decorated class does not inherit from `IEcoComponentFactory`.
    """
    if not isinstance(component, type) or not getattr(component, "_eco_component_", False):
        raise TypeError(
            f"Class '{component.__name__}' is not a component and cannot be used in a factory"
        )

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

        # Build a hidden view carrying the `IEcoComponentFactory` methods.
        factory_view = type(
            f"{cls.__name__}_View",
            (IEcoComponentFactory,),
            {
                "Alloc": _make_alloc(component),
                "Init": _make_init(),
                "get_Name": _make_get_name(),
                "get_Version": _make_get_version(),
                "get_Manufacturer": _make_get_manufacturer(),
            },
        )
        factory_view = view(factory_view)
        setattr(cls, factory_view.__name__, factory_view)

        # Build the layout (vtable fields, refs, user fields)
        views = {IID_IEcoComponentFactory: factory_view}
        new_cls, vtables = _build_layout_class(cls, views)

        # Static singleton — make `__eco_del__` a no-op so inherited `Release` can call it safely
        new_cls.__eco_del__ = lambda self: None

        # Wrap `__init__` so vtables, metadata, and refcount are set before user code
        new_cls.__init__ = _make_factory_init(_make_install_vtables(views, vtables), metadata)  # type: ignore

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
