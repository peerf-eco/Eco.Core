"""Shared building blocks for `@view`, `@component` and `@factory` decorators.

Hosts the layout, trampoline, and view-gathering machinery used by every
server-side decorator.
"""

from collections.abc import Callable
from typing import Any, Optional

from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.view import view
from eco_python2acom.decorators.utils import _build_namespace, _resolve_fields
from eco_python2acom.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.runtime.logging import eco_logger
from eco_python2acom.types.core import (
    CPointer,
    CSimpleData,
    CString,
    CStructure,
    Int16,
    UInt32,
    Void,
)
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, cast, offsetof

# -----------------------------------------------------------------------------
# IEcoUnknown — non-delegating
# -----------------------------------------------------------------------------


class IEcoUnknownMethods:
    """Non-delegating `IEcoUnknown` triple: `QueryInterface`, `AddRef`, `Release`."""

    @staticmethod
    def build(primary: UGUID, offsets: dict[UGUID, int]) -> dict[str, Callable[..., Any]]:
        """Build the triple bound to the given primary IID and view-offset table."""

        def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
            """Query for another interface on this component.

            Args:
                iid: Pointer to the requested interface ID.
                out: Output pointer to receive the interface.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(iid) or not bool(out):
                self.logger.debug("Null pointer")
                return EcoErrorCode.POINTER

            target = iid.obj
            offset = offsets.get(target)

            if offset is None and target == IID_IEcoUnknown:
                offset = offsets.get(primary)
            if offset is None:
                self.logger.debug(f"IID = <{target}> ---> Interface not supported")
                out.obj.value = 0
                return EcoErrorCode.NOINTERFACE

            out.obj.value = addressof(self) + offset
            self.AddRef()
            self.logger.debug(f"IID = <{target}> ---> OK")
            return EcoErrorCode.SUCCESS

        def AddRef(self) -> UInt32:
            """Increment the reference count.

            Returns:
                The new reference count.
            """
            self.refs += 1
            self.logger.debug(f"Refs = {self.refs}")
            return self.refs

        def Release(self) -> UInt32:
            """Decrement the reference count.

            When the count reaches zero, the component is freed.

            Returns:
                The new reference count.
            """
            self.refs -= 1
            if self.refs == 0:
                self.__eco_del__()
                ALIVE.pop(addressof(self), None)
                self.logger.debug("Refs = 0 ---> Destroyed")
            else:
                self.logger.debug(f"Refs = {self.refs}")
            return self.refs

        return {"QueryInterface": QueryInterface, "AddRef": AddRef, "Release": Release}


# -----------------------------------------------------------------------------
# IEcoUnknown — delegating
# -----------------------------------------------------------------------------


class DelegatingIEcoUnknownMethods:
    """Delegating `IEcoUnknown` triple that forwards every call to outer component."""

    @staticmethod
    def build() -> dict[str, Callable[..., Any]]:
        """Build the triple — every call is forwarded to `self.outer`."""

        def is_aggregated(self) -> bool:
            """True if `self.outer` points at an external outer."""
            return self.outer.value != pointer(self, IEcoUnknown, shift=True).value

        def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
            """Query for another interface on this component.

            Args:
                iid: Pointer to the requested interface ID.
                out: Output pointer to receive the interface.

            Returns:
                0 on success, error code otherwise.
            """
            if is_aggregated(self):
                self.logger.debug("Delegating to outer")
            return self.outer.obj.QueryInterface(iid, out)

        def AddRef(self) -> UInt32:
            """Increment the reference count.

            Returns:
                The new reference count.
            """
            if is_aggregated(self):
                self.logger.debug("Delegating to outer")
            return self.outer.obj.AddRef()

        def Release(self) -> UInt32:
            """Decrement the reference count.

            When the count reaches zero, the component is freed.

            Returns:
                The new reference count.
            """
            if is_aggregated(self):
                self.logger.debug("Delegating to outer")
            return self.outer.obj.Release()

        return {"QueryInterface": QueryInterface, "AddRef": AddRef, "Release": Release}


# -----------------------------------------------------------------------------
# IEcoComponentFactory
# -----------------------------------------------------------------------------


class IEcoComponentFactoryMethods:
    """Method bundle implementing `IEcoComponentFactory`."""

    @staticmethod
    def build(cls: type) -> dict[str, Callable[..., Any]]:
        """Build the bundle that allocates instances of the given component class."""

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
                self.logger.debug("Null pointer")
                return EcoErrorCode.POINTER

            if bool(outer) and iid.obj != IID_IEcoUnknown:
                self.logger.debug("Aggregation requires 'IID_IEcoUnknown'")
                return EcoErrorCode.NOAGGREGATION

            instance = cls()
            result = instance.__eco_new__(system, outer)
            if result != 0:
                self.logger.debug(f"Instance creation failed | code = {result}")
                return result

            result = instance.__eco_init__(system)
            if result != 0:
                self.logger.debug(f"Instance init failed | code = {result}")
                return result

            ALIVE[addressof(instance)] = instance
            self.logger.debug(f"Instance at <0x{addressof(instance):X}>")

            result = instance.QueryInterface(iid, out)
            if result != 0:
                self.logger.debug(f"Interface query failed | code = {result}")
                return result

            instance.Release()
            return EcoErrorCode.SUCCESS

        def Init(self, system: Optional[Ptr[IEcoUnknown]], context: Ptr[Void]) -> Int16:
            """Initialize the factory with system context.

            Args:
                system: Pointer to system interface (can be NULL).
                context: Additional context (e.g., bus pointer).

            Returns:
                0 on success, error code otherwise.
            """
            return EcoErrorCode.SUCCESS

        def get_Name(self) -> CString:
            """Get the component name.

            Returns:
                Pointer to null-terminated string.
            """
            return self.name

        def get_Version(self) -> CString:
            """Get the component version.

            Returns:
                Pointer to null-terminated string.
            """
            return self.version

        def get_Manufacturer(self) -> CString:
            """Get the component manufacturer.

            Returns:
                Pointer to null-terminated string.
            """
            return self.manufacturer

        return {
            "Alloc": Alloc,
            "Init": Init,
            "get_Name": get_Name,
            "get_Version": get_Version,
            "get_Manufacturer": get_Manufacturer,
        }


@view
class NDU(IEcoUnknown):
    """Non-delegating `IEcoUnknown` view — methods fall through to the parent."""


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
        out[IID_IEcoUnknown] = NDU

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

    instance = ALIVE.get(parent_addr)
    if instance is not None:
        return instance
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

        # Unwrap EcoOS instances: callback restype marshalling only accepts raw values
        if isinstance(result, CPointer | CSimpleData):
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

    Each slot is filled with a `Func[...]`-wrapped trampoline. For `IEcoUnknown`
    slots the priority is: a method declared on the view itself takes precedence,
    then an entry in `iecounknown_overrides`, finally the parent's method.

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
            if method_name in view.__dict__:
                method = view.__dict__[method_name]
            elif method_name in iecounknown_overrides:
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
        offsets[iface._iid_] = offsetof(new_cls, iface)

    # Install the non-delegating IEcoUnknown triple on `new_cls`
    primary_iid = next(iid for iid in views if iid != IID_IEcoUnknown)
    for name, method in IEcoUnknownMethods.build(primary_iid, offsets).items():
        if name not in new_cls.__dict__:
            setattr(new_cls, name, method)

    vtbls: dict[UGUID, Any] = {}
    delegating = DelegatingIEcoUnknownMethods.build() if aggregatable else None
    for iid, view_cls in views.items():
        overrides = delegating if aggregatable and iid != IID_IEcoUnknown else None
        vtbls[iid] = _build_server_vtbl(view_cls, offsets[iid], new_cls, overrides)

    new_cls._eco_vtbls_ = vtbls
    new_cls._eco_views_ = views
    new_cls.logger = eco_logger.getChild(new_cls.__name__)
    return new_cls, vtbls
