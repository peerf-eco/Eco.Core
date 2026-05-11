"""Shared building blocks for `@view`, `@component` and `@factory` decorators.

Hosts the layout, trampoline, and view-gathering machinery used by every
server-side decorator.
"""

from collections.abc import Callable
from typing import Any

from eco_python2acom.decorators.server.methods import (
    DelegatingIEcoUnknownMethods,
    IEcoComponentFactoryMethods,
    IEcoUnknownMethods,
)
from eco_python2acom.decorators.server.ndu import NonDelegatingUnknownView
from eco_python2acom.decorators.server.view import view
from eco_python2acom.decorators.utils import _build_namespace, _resolve_fields
from eco_python2acom.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CSimpleData, CStructure, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import cast

# -----------------------------------------------------------------------------
# View collection
# -----------------------------------------------------------------------------


def _collect_views(cls: type, aggregatable: bool = False) -> dict[UGUID, type]:
    """Resolve the `{IID: view}` mapping for a component class.

    Args:
        cls: The component class being decorated.
        aggregatable: When `True`, append the NDU view under `IID_IEcoUnknown`.

    Returns:
        A mapping from interface IID to view namespace.

    Raises:
        TypeError: If the class declares no user `@view`s.
        ValueError: If two collected views target the same interface.
    """
    out: dict[UGUID, type] = {}

    for value in cls.__dict__.values():
        if isinstance(value, type) and getattr(value, "_eco_view_", False):
            iface = value._eco_iface_
            if iface._iid_ in out:
                raise ValueError(
                    f"Component '{cls.__name__}' has two views for interface '{iface.__name__}'"
                )
            out[iface._iid_] = value

    if not out:
        raise TypeError(f"Component '{cls.__name__}' must declare at least one '@view'")

    if aggregatable:
        out[IID_IEcoUnknown] = NonDelegatingUnknownView

    return out


def _collect_factory_view(cls: type, product: type) -> dict[UGUID, type]:
    """Build the `{IID_IEcoComponentFactory: view}` mapping for a factory class.

    Args:
        cls: The factory class being decorated.
        product: The `@component`-decorated class the factory creates.

    Returns:
        A single-entry mapping with the synthesised factory view.
    """
    factory_view = view(
        type(
            f"{cls.__name__}View",
            (IEcoComponentFactory,),
            IEcoComponentFactoryMethods.build(product),
        )
    )
    return {IID_IEcoComponentFactory: factory_view}


# -----------------------------------------------------------------------------
# Structure field resolution
# -----------------------------------------------------------------------------


def _build_server_fields(
    cls: type, views: dict[UGUID, type], aggregatable: bool = False
) -> list[tuple[str, type]]:
    """Build the ordered `_fields_` list for a component-like structure.

    Vtable pointers come first (one per view in declaration order), followed
    by `refs`, `outer` (when aggregatable), then any user-declared fields.

    Args:
        cls: The class being rebuilt — its annotations contribute the trailing user fields.
        views: The `{IID: view}` mapping previously collected for `cls`.
        aggregatable: When `True`, insert the `outer` controlling-unknown pointer.

    Returns:
        A list of `(field name, field type)` pairs ready to assign to `_fields_`.
    """
    fields: list[tuple[str, type]] = []

    for view_cls in views.values():
        iface = view_cls._eco_iface_
        fields.append((f"_vtbl_{iface.__name__}", Ptr[iface._vtbl_]))  # type: ignore

    fields.append(("refs", UInt32))
    if aggregatable:
        fields.append(("outer", Ptr[IEcoUnknown]))

    fields.extend(_resolve_fields(cls))
    return fields


# -----------------------------------------------------------------------------
# Method trampolines
# -----------------------------------------------------------------------------


def _resolve_parent(ptr: Any, offset: int, parent: type) -> Any:
    """Recover the parent instance from a vtable-field pointer.

    Subtracts the offset from `self` and reinterprets the result as parent class.

    Args:
        ptr: Pointer C has handed us — points at a vtable field of the parent struct.
        offset: Byte offset of that vtable field within the parent struct.
        parent: The component (or factory) class to reinterpret the resulting address as.

    Returns:
        The parent instance, or `None` if `ptr` is NULL.
    """
    if not bool(ptr):
        return None

    void_ptr = cast(ptr, Ptr[Void])
    parent_addr = void_ptr.value - offset  # type: ignore
    return cast(parent_addr, Ptr[parent]).obj


def _make_method_trampoline(
    method: Callable[..., Any], offset: int, parent_cls: type
) -> Callable[..., Any]:
    """Build a trampoline that dispatches a vtable slot to `method` on the parent.

    Args:
        method: The Python callable to dispatch to (a view method or an IEcoUnknown override).
        offset: Byte offset of the vtable field within the parent struct.
        parent_cls: The component (or factory) class to reinterpret `ptr` as.

    Returns:
        A trampoline callable ready to be wrapped in a `Func[...]` slot type.
    """

    def trampoline(ptr: Any, *args: list[Any]) -> Any:
        parent = _resolve_parent(ptr, offset, parent_cls)
        if parent is None:
            return 0
        result = method(parent, *args)
        if isinstance(result, CSimpleData):
            return result.value
        return result

    trampoline.__name__ = getattr(method, "__name__", "trampoline")
    return trampoline


# -----------------------------------------------------------------------------
# Decorator helpers
# -----------------------------------------------------------------------------


def _build_server_vtbl(
    view: type,
    offset: int,
    parent: type,
    iecounknown_overrides: dict[str, Callable[..., Any]] | None = None,
) -> Any:
    """Build the singleton vtable instance for one view.

    Each slot is filled with a `Func[...]`-wrapped trampoline. User slots
    invoke the view's method; `IEcoUnknown` slots come from `parent` unless
    explicitly overridden via `iecounknown_overrides`.

    Args:
        view: The `@view`-decorated namespace whose methods populate the user slots.
        offset: Byte offset of this view's vtable field within the parent struct.
        parent: The component (or factory) class trampolines must reinterpret to.
        iecounknown_overrides: Optional `{name: callable}` replacing the parent's
            `QueryInterface` / `AddRef` / `Release`.

    Returns:
        The singleton vtable instance ready to be referenced from the layout field.
    """
    vtbl = view._eco_iface_._vtbl_
    vtbl_obj = vtbl()
    iecounknown_methods = {"QueryInterface", "AddRef", "Release"}
    iecounknown_overrides = iecounknown_overrides or {}

    for field_name, slot_type in vtbl._fields_:
        method_name = field_name.removeprefix("_func_")
        if method_name in iecounknown_methods:
            if method_name in iecounknown_overrides:
                method = iecounknown_overrides[method_name]
            else:
                method = getattr(parent, method_name)
        else:
            method = view.__dict__[method_name]

        trampoline = _make_method_trampoline(method, offset, parent)
        setattr(vtbl_obj, field_name, slot_type(trampoline))

    return vtbl_obj


def _make_vtbls_installer(views: dict[UGUID, type], vtbls: dict[UGUID, Any]) -> Callable[..., None]:
    """Build a helper that points every `_vtbl_*` field at its singleton vtable.

    Args:
        views: The `{IID: view}` mapping the layout was built from.
        vtbls: The matching `{IID: vtbl}` mapping.

    Returns:
        A callable `installer(self)` that wires each vtable singleton into the
        corresponding `_vtbl_<IfaceName>` field of `self`.
    """

    def vtbls_installer(self) -> None:
        for iid, view_cls in views.items():
            iface = view_cls._eco_iface_
            field_name = f"_vtbl_{iface.__name__}"
            ptr_type = Ptr[iface._vtbl_]  # type: ignore
            setattr(self, field_name, ptr_type(vtbls[iid]))

    return vtbls_installer


def _eco_server_class(
    cls: type, aggregatable: bool = False, product: type | None = None
) -> tuple[type, dict[UGUID, Any]]:
    """Rebuild `cls` as a `CStructure` with vtable fields and singleton vtables.

    Args:
        cls: The user-decorated class being rebuilt.
        aggregatable: When `True`, build the component in inner-aggregation mode.
        product: When set, build `cls` as a factory that allocates instances of
            this `@component`-decorated class.

    Returns:
        The freshly built class and the `{IID: vtbl}` map`.
    """
    if product is not None:
        views = _collect_factory_view(cls, product)
    else:
        views = _collect_views(cls, aggregatable=aggregatable)

    fields = _build_server_fields(cls, views, aggregatable=aggregatable)
    namespace = _build_namespace(cls, extra={"_fields_": fields})
    new_cls = type(CStructure)(cls.__name__, (CStructure,), namespace)  # type: ignore

    offsets: dict[UGUID, int] = {}
    for view_cls in views.values():
        iface = view_cls._eco_iface_
        offsets[iface._iid_] = getattr(new_cls, f"_vtbl_{iface.__name__}").offset

    # Install the non-delegating IEcoUnknown triple on `new_cls`
    primary_iid = next(iid for iid in views if iid != IID_IEcoUnknown)
    for name, method in IEcoUnknownMethods.build(primary_iid, offsets).items():
        setattr(new_cls, name, method)

    vtbls: dict[UGUID, Any] = {}
    delegating = DelegatingIEcoUnknownMethods.build() if aggregatable else None
    for iid, view_cls in views.items():
        overrides = delegating if aggregatable and iid != IID_IEcoUnknown else None
        vtbls[iid] = _build_server_vtbl(view_cls, offsets[iid], new_cls, overrides)

    new_cls._eco_vtbls_ = vtbls
    new_cls._eco_views_ = views
    return new_cls, vtbls
