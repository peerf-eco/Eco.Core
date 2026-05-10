"""Shared building blocks for `@view`, `@component` and `@factory` decorators.

Hosts the layout, trampoline, and `IEcoUnknown` machinery used by every
server-side decorator.
"""

from collections.abc import Callable
from typing import Any

from eco_python2acom.decorators.utils import _build_namespace, _resolve_fields
from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.types.core import CSimpleData, CStructure, Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof, cast

# Holds Python instances whose vtable address is currently exposed to C, so
# refcount-based collection cannot pull them out from under the C caller.
_alive: dict[int, Any] = {}


def _vtbl_field_name(iface: type) -> str:
    """Return the layout field name for the vtable pointer of `iface`."""
    return f"_vtbl_{iface.__name__}"


def _collect_views(cls: type) -> dict[UGUID, type]:
    """Collect nested `@view` namespaces from `cls` in declaration order.

    Args:
        cls: The component-like class being decorated.

    Returns:
        A mapping from interface IID to view namespace.

    Raises:
        TypeError: If two views target the same interface.
    """
    out: dict[UGUID, type] = {}

    for value in cls.__dict__.values():
        if isinstance(value, type) and getattr(value, "_eco_view_", False):
            iface = value._eco_iface_
            if iface._iid_ in out:
                raise TypeError(
                    f"Component '{cls.__name__}' has two views for interface '{iface.__name__}'"
                )
            out[iface._iid_] = value

    return out


def _build_layout_fields(cls: type, views: dict[UGUID, type]) -> list[tuple[str, type]]:
    """Build the ordered `_fields_` list for a component-like structure.

    Vtable pointers come first (one per view in declaration order), followed
    by `refs`, then any user-declared fields.
    """
    fields: list[tuple[str, type]] = []

    for view in views.values():
        iface = view._eco_iface_
        fields.append((_vtbl_field_name(iface), Ptr[iface._vtbl_]))  # type: ignore

    fields.append(("refs", UInt32))
    fields.extend(_resolve_fields(cls))
    return fields


def _resolve_parent(ptr: Any, offset: int, parent: type) -> Any:
    """Recover the parent instance from a vtable-field pointer.

    Subtracts the offset from `self` and reinterprets the result as parent class.
    """
    if not bool(ptr):
        return None

    void_ptr = cast(ptr, Ptr[Void])
    parent_addr = void_ptr.value - offset  # type: ignore
    return cast(parent_addr, Ptr[parent]).obj


def _make_method_trampoline(
    method: Callable[..., Any], offset: int, parent_cls: type
) -> Callable[..., Any]:
    """Build a trampoline that dispatches a vtable slot to `method` on the parent."""

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


def _build_vtable_for(view: type, offset: int, parent: type) -> Any:
    """Build the singleton vtable instance for one view.

    Each slot of the interface's vtable is filled with a `Func[...]`-wrapped
    trampoline. `IEcoUnknown` slots dispatch to methods on the parent class
    (`QueryInterface` / `AddRef` / `Release`); user slots invoke the
    corresponding method declared in the view namespace.
    """
    vtbl = view._eco_iface_._vtbl_
    vtbl_obj = vtbl()
    iecounknown_methods = {"QueryInterface", "AddRef", "Release"}

    for field_name, slot_type in vtbl._fields_:
        method_name = field_name.removeprefix("_func_")
        if method_name in iecounknown_methods:
            method = getattr(parent, method_name)
        else:
            method = view.__dict__[method_name]

        trampoline = _make_method_trampoline(method, offset, parent)
        setattr(vtbl_obj, field_name, slot_type(trampoline))

    return vtbl_obj


def _make_install_vtables(
    views: dict[UGUID, type], vtables: dict[UGUID, Any]
) -> Callable[..., None]:
    """Build a helper that points every `_vtbl_*` field at its singleton vtable."""

    def install_vtables(self) -> None:
        for iid, view in views.items():
            field_name = _vtbl_field_name(view._eco_iface_)
            ptr_type = Ptr[view._eco_iface_._vtbl_]  # type: ignore
            setattr(self, field_name, ptr_type(vtables[iid]))

    return install_vtables


def _make_query_interface(primary_iid: UGUID, offsets: dict[UGUID, int]) -> Callable[..., Int16]:
    """Build a `QueryInterface` that returns the address of the right vtable field."""

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for another interface on this component.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out):
            return Int16(EcoErrorCode.POINTER)

        target_iid = iid.obj
        if target_iid == IID_IEcoUnknown:
            target_iid = primary_iid

        offset = offsets.get(target_iid)
        if offset is None:
            out.obj.value = 0
            return Int16(EcoErrorCode.NOINTERFACE)

        out.obj.value = addressof(self) + offset
        self.AddRef()
        return Int16(EcoErrorCode.SUCCESS)

    return QueryInterface


def _make_add_ref() -> Callable[..., UInt32]:
    """Build an `AddRef` that increments the shared refcount."""

    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        self.refs += 1
        return self.refs

    return AddRef


def _make_release() -> Callable[..., UInt32]:
    """Build a `Release` that runs `__eco_del__` when refcount reaches zero."""

    def Release(self) -> UInt32:
        """Decrement the reference count.

        When the count reaches zero, the component is freed.

        Returns:
            The new reference count.
        """
        self.refs -= 1
        if self.refs == 0:
            self.__eco_del__()
            _alive.pop(addressof(self), None)
        return self.refs

    return Release


def _build_layout_class(cls: type, views: dict[UGUID, type]) -> tuple[type, dict[UGUID, Any]]:
    """Rebuild `cls` as a `CStructure` with vtable fields and singleton vtables.

    Computes `_fields_`, subclasses `CStructure`, builds one singleton vtable
    per view, and installs the `IEcoUnknown` triple on the resulting class.

    Args:
        cls: The user-decorated class being rebuilt.
        views: Views collected from the class body.

    Returns:
        The tuple of rebuilt class and its singleton vtables.
    """
    fields = _build_layout_fields(cls, views)
    namespace = _build_namespace(cls, extra={"_fields_": fields})
    new_cls = type(CStructure)(cls.__name__, (CStructure,), namespace)  # type: ignore

    offsets: dict[UGUID, int] = {
        view._eco_iface_._iid_: getattr(new_cls, _vtbl_field_name(view._eco_iface_)).offset
        for view in views.values()
    }

    # Install the IEcoUnknown triple before building vtables, so the unified
    # trampoline factory can pick `QueryInterface` / `AddRef` / `Release`
    primary_iid = next(iter(views))
    new_cls.QueryInterface = _make_query_interface(primary_iid, offsets)
    new_cls.AddRef = _make_add_ref()
    new_cls.Release = _make_release()

    # Vtables hold a reference to `new_cls` so trampolines can recover the
    # parent instance from a vtable-field pointer via cast.
    vtables: dict[UGUID, Any] = {
        iid: _build_vtable_for(view, offsets[iid], new_cls) for iid, view in views.items()
    }
    new_cls._eco_vtables_ = vtables
    new_cls._eco_views_ = views

    return new_cls, vtables
