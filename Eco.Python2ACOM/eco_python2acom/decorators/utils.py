"""Utilities for @model, @union and @interface decorators.

This module provides helper tools for building EcoOS structure, union and interface classes.
"""

import inspect
from collections.abc import Callable
from types import FrameType, NoneType, UnionType
from typing import Any, ClassVar, Optional, Union, get_args, get_origin, get_type_hints

from eco_python2acom.types.core import CData, CFuncType, CLayout, CStructure, Int16
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import pointer

# -----------------------------------------------------------------------------
# Type validation and normalization
# -----------------------------------------------------------------------------


def _validate(hint: type) -> type:
    """Validate EcoOS data type.

    Args:
        hint: The type hint to validate.

    Returns:
        The same type if valid.

    Raises:
        TypeError: If hint is not a valid EcoOS data type.
    """
    try:
        if hint is NoneType or issubclass(hint, CData):
            return hint
    except TypeError:
        pass
    raise TypeError(f"Invalid EcoOS data type: '{hint}'")


def _normalize(hint: type) -> type:
    """Normalize EcoOS data type.

    Args:
        hint: The type hint to normalize.

    Returns:
        The normalized data type.

    Raises:
        TypeError: If union has more than one non-None type.
    """
    if get_origin(hint) in (Union, UnionType):
        args = [arg for arg in get_args(hint) if arg is not type(None)]
        if len(args) != 1:
            raise TypeError(f"Invalid EcoOS union type: '{hint}'")
        hint = next(iter(args))
    return hint


# -----------------------------------------------------------------------------
# Structure field resolution
# -----------------------------------------------------------------------------


def _resolve_fields(cls: type) -> list[tuple[str, type]]:
    """Resolve EcoOS fields defined in this class only.

    Args:
        cls: The structure class to resolve fields of.

    Returns:
        A list of tuples of field names and their resolved EcoOS types.
    """
    hints = get_type_hints(cls)
    return [
        (name, _validate(_normalize(hints[name])))
        for name in getattr(cls, "__annotations__", {})
        if get_origin(hints.get(name)) is not ClassVar
    ]


def _inherited_slots(cls: type) -> list[tuple[str, type, list[type], list[str]]]:
    """Collect vtable slots inherited from the direct parent interface.

    Args:
        cls: The interface class whose parent to inspect.

    Returns:
        A list of `(field name, return type, param types, param names)` tuples.
    """
    parent = cls.__dict__.get("_eco_parent_")
    if parent is None:
        return []

    parent_vtbl = getattr(parent, "_vtbl_", None)
    if parent_vtbl is None:
        return []

    slots: list[tuple[str, type, list[type], list[str]]] = []
    parent_params = getattr(parent, "_eco_method_params_", {})

    for field_name, slot_type in parent_vtbl._fields_:
        argtypes = iter(slot_type._argtypes_)
        next(argtypes)  # Skip implicit 'self' / instance parameter
        slots.append(
            (
                field_name,
                slot_type._restype_,
                list(argtypes),
                parent_params.get(field_name, []),
            )
        )
    return slots


def _declared_slots(cls: type) -> list[tuple[str, type, list[type], list[str]]]:
    """Collect vtable slots from methods declared on `cls` itself.

    Args:
        cls: The interface class to inspect.

    Returns:
        A list of `(field name, return type, param types, param names)` tuples.
    """
    slots: list[tuple[str, type, list[type], list[str]]] = []

    for name, value in cls.__dict__.items():
        if name.startswith("__") and name.endswith("__"):
            continue
        if not inspect.isfunction(value):
            continue

        hints = get_type_hints(value)
        return_type = _validate(_normalize(hints.pop("return", Int16)))
        param_names, param_types = [], []

        params = iter(inspect.signature(value).parameters.values())
        next(params)  # Skip implicit 'self' / instance parameter
        for param in params:
            param_types.append(_validate(_normalize(hints[param.name])))
            param_names.append(param.name)

        slots.append((f"_func_{name}", return_type, param_types, param_names))
    return slots


def _resolve_methods(cls: type) -> list[tuple[str, type]]:
    """Resolve EcoOS interface methods for the vtable of ``cls``.

    Collects inherited slots from finalized ancestor vtables first,
    then appends methods declared on `cls` itself. The first parameter of
    every slot is `Ptr[cls]` — the derived-interface pointer.

    Args:
        cls: The interface class to resolve methods of.

    Returns:
        A list of tuples with vtable field names and their CFuncType.

    Raises:
        TypeError: If `cls` redeclares a method already defined by an ancestor.
    """
    self_ptr_type = Ptr[cls]  # type: ignore
    methods: list[tuple[str, type]] = []
    method_params: dict[str, list[str]] = {}
    method_returns: dict[str, type] = {}
    seen: set[str] = set()

    for field_name, return_type, param_types, param_names in (
        *_inherited_slots(cls),
        *_declared_slots(cls),
    ):
        if field_name in seen:
            method_name = field_name.removeprefix("_func_")
            raise TypeError(
                f"'{cls.__name__}.{method_name}' is already defined by an ancestor interface"
            )
        seen.add(field_name)
        func_type = CFuncType(return_type, self_ptr_type, *param_types)  # type: ignore
        methods.append((field_name, func_type))
        method_params[field_name] = param_names
        method_returns[field_name] = return_type

    cls._eco_method_params_ = method_params
    cls._eco_method_returns_ = method_returns
    return methods


def _apply_resolver(
    cls: type, marker: str, resolver: Callable[[type], list[tuple[str, type]]]
) -> list[tuple[str, type]]:
    """Apply a resolver function to populate fields of a class.

    Args:
        cls: The class to populate fields of.
        marker: The class attribute marker to check.
        resolver: A function to resolve field types.

    Returns:
        A list of tuples with field names and resolved types.
    """
    if not cls.__dict__.get(marker):
        return []

    try:
        return resolver(cls)
    except TypeError:
        raise
    except Exception as err:
        raise TypeError(f"Class '{cls.__name__}' contains unresolved types") from err


def _install_dispatchers(cls: type, methods: list[tuple[str, type]]) -> None:
    """Replace method stubs with C function dispatchers for an interface class.

    Each dispatcher reads the function pointer from the vtable
    and calls it with self pointer as the implicit first argument.

    Args:
        cls: The interface class to install dispatchers on.
        methods: List of tuples with method names and EcoOS types.
    """
    method_params = getattr(cls, "_eco_method_params_", {})
    method_returns = getattr(cls, "_eco_method_returns_", {})

    for field_name, _ in methods:
        method_name = field_name.removeprefix("_func_")
        param_names = method_params.get(field_name, [])
        return_type = method_returns.get(field_name)
        original = cls.__dict__.get(method_name)
        docstring = getattr(original, "__doc__", None) if callable(original) else None

        def make_dispatch(
            field_name: str,
            method_name: str,
            param_names: list[str],
            return_type: Optional[type] = None,
            docstring: Optional[str] = None,
        ) -> Callable[..., Any]:
            """Create a dispatcher closure for a specific method."""

            def dispatch(self, *args: Any, **kwargs: Any) -> Any:
                func_ptr = getattr(self.vtbl.contents, field_name)
                self_ptr = pointer(self)
                if kwargs:
                    full_args = list(args)
                    for idx, param_name in enumerate(param_names):
                        if idx < len(args):
                            continue
                        if param_name in kwargs:
                            full_args.append(kwargs[param_name])
                        else:
                            raise TypeError(
                                f"'{method_name}' missing required argument: '{param_name}'"
                            )
                    result = func_ptr(self_ptr, *full_args)
                else:
                    result = func_ptr(self_ptr, *args)

                if return_type is not None and result is not None:
                    return return_type(result)
                return result

            dispatch.__name__ = method_name
            dispatch.__doc__ = docstring
            return dispatch

        dispatcher = make_dispatch(field_name, method_name, param_names, return_type, docstring)
        setattr(cls, method_name, dispatcher)


# -----------------------------------------------------------------------------
# Finalization
# -----------------------------------------------------------------------------


def _finalize(cls: type) -> type:
    """Finalize an EcoOS structure, union, or interface class.

    Resolves field and method annotations, validates the class layout,
    and prepares it for use with EcoOS. No-op for non-layout types.

    Args:
        cls: The class to finalize.

    Returns:
        The same class, finalized if applicable.

    Raises:
        TypeError: If field or method types cannot be resolved.
    """
    if not (isinstance(cls, type) and issubclass(cls, CLayout) and cls not in CLayout.__args__):
        return cls

    methods = _apply_resolver(cls, "_eco_interface_", _resolve_methods)
    model_fields = _apply_resolver(cls, "_eco_model_", _resolve_fields)
    union_fields = _apply_resolver(cls, "_eco_union_", _resolve_fields)

    interface_fields: list[tuple[str, type]] = []
    if methods:
        interface_fields = [("vtbl", _build_vtbl(cls, methods))]

    cls._fields_ = union_fields + model_fields + interface_fields
    if methods:
        _install_dispatchers(cls, methods)

    return cls


def _build_vtbl(cls: type, methods: list[tuple[str, type]]) -> type:
    """Create the hidden vtable class for an interface.

    Args:
        cls: The interface class being finalized.
        methods: List of method tuples to install as slots.

    Returns:
        Pointer type to the newly created vtable class.
    """
    meta = _get_eco_layout_meta(CStructure)
    namespace: dict[str, Any] = {
        "__module__": cls.__module__,
        "__qualname__": f"{cls.__qualname__}._vtbl_",
        "__doc__": f"Virtual table for {cls.__name__}.",
        "_fields_": methods,
    }
    vtbl_cls = meta(f"{cls.__name__}VTbl", (CStructure,), namespace)
    cls._vtbl_ = vtbl_cls
    return Ptr[vtbl_cls]  # type: ignore


# -----------------------------------------------------------------------------
# Metaclass for EcoOS structures and unions
# -----------------------------------------------------------------------------

_eco_layout_meta_cache: dict[type, type] = {}


def _inherits_dunder(bases: tuple[type, ...], name: str) -> bool:
    """Check whether a dunder method is inherited from base classes.

    Args:
        bases: Tuple of base classes to inspect.
        name: Name of the dunder method.

    Returns:
        True if the method is found in any base class in the MRO, False otherwise.
    """
    for base in bases:
        for cls in base.__mro__:
            if cls is object:
                continue
            if name in cls.__dict__:
                return True
    return False


def _struct_eq(self, other: Any) -> bool:
    """Compare two EcoOS structures by field values."""
    if not isinstance(other, type(self)):
        return NotImplemented  # type: ignore

    for field_name, _ in getattr(type(self), "_fields_", []):
        if getattr(self, field_name) != getattr(other, field_name):
            return False

    return True


def _struct_repr(self) -> str:
    """Return string representation of an EcoOS structure or interface."""
    cls = type(self)
    field_strs = [
        f"{field_name}={getattr(self, field_name)!r}"
        for field_name, _ in getattr(cls, "_fields_", [])
        if not field_name.startswith("_func_")
    ]
    return f"{cls.__name__}({', '.join(field_strs)})"


def _get_eco_layout_meta(base: type) -> type:
    """Get or create an `EcoLayoutMeta` metaclass for the given base.

    Args:
        base: The base type.

    Returns:
        The `EcoLayoutMeta` metaclass for the given base type.
    """
    if base in _eco_layout_meta_cache:
        return _eco_layout_meta_cache[base]

    native_meta = type(base)

    class EcoLayoutMeta(native_meta):  # type: ignore
        """Metaclass for EcoOS C structures and unions.

        Injects default `__eq__` and `__repr__` methods for better debugging.
        """

        def __new__(
            cls, name: str, bases: tuple[type, ...], namespace: dict[str, Any], **kwargs: Any
        ) -> type:
            """Create the class with default `__eq__` / `__repr__`."""
            if "__eq__" not in namespace and not _inherits_dunder(bases, "__eq__"):
                namespace["__eq__"] = _struct_eq
            if "__repr__" not in namespace and not _inherits_dunder(bases, "__repr__"):
                namespace["__repr__"] = _struct_repr

            return super().__new__(cls, name, bases, namespace, **kwargs)  # type: ignore

    EcoLayoutMeta.__name__ = f"EcoLayoutMeta[{base.__name__}]"
    EcoLayoutMeta.__qualname__ = EcoLayoutMeta.__name__
    _eco_layout_meta_cache[base] = EcoLayoutMeta
    return EcoLayoutMeta


# -----------------------------------------------------------------------------
# Decorator helpers
# -----------------------------------------------------------------------------


def _resolve_base(cls: type, base: type) -> type:
    """Resolve and validate the base class for a decorated class.

    Args:
        cls: The class being decorated.
        base: The default base type.

    Returns:
        The resolved base class.

    Raises:
        TypeError: If multiple inheritance is used or base is invalid.
    """
    if len(cls.__bases__) > 1:
        raise TypeError(f"'{cls.__name__}' uses multiple inheritance")

    parent = next(iter(cls.__bases__)) if cls.__bases__ else object
    if parent is not object and not issubclass(parent, base):
        raise TypeError(f"Class '{cls.__name__}' inherits from invalid base '{parent.__name__}'")

    return parent if parent is not object else base


def _build_namespace(cls: type, extra: dict[str, Any] | None = None) -> dict[str, Any]:
    """Build namespace dict for a new `EcoLayoutMeta` class.

    Copies module, qualname, docs, annotations, and all non-special attributes
    from the original class. Optionally merges extra attributes.

    Args:
        cls: The original class being decorated.
        extra: Additional attributes to add to namespace.

    Returns:
        Namespace dict ready for `EcoLayoutMeta`.
    """
    namespace: dict[str, Any] = {
        "__module__": cls.__module__,
        "__qualname__": cls.__qualname__,
        "__doc__": cls.__doc__,
        "__annotations__": getattr(cls, "__annotations__", None) or {},
    }

    if extra:
        namespace.update(extra)

    skip = frozenset({"__dict__", "__weakref__", "_fields_"})
    for key, value in cls.__dict__.items():
        if key in namespace or key in skip:
            continue
        namespace[key] = value

    return namespace


def _eco_class(
    cls: type,
    base: type,
    frame: FrameType | None,
    extra: dict[str, Any] | None = None,
    validator: Callable[[type], None] | None = None,
    flatten: bool = False,
) -> type:
    """Create or fill in an `EcoLayoutMeta` class from a decorated class.

    Args:
        cls: The original class being decorated.
        base: The default base type.
        frame: Caller's frame for namespace registration.
        extra: Additional attributes to add to namespace.
        validator: Optional function validating the class body before creation.
        flatten: If True, drop inheritance and stash the declared parent in `_eco_parent_`.

    Returns:
        The resulting class (new or adopted).
    """
    if validator is not None:
        validator(cls)

    parent = _resolve_base(cls, base)
    extra = dict(extra) if extra else {}
    if flatten:
        extra["_eco_parent_"] = parent if parent is not base else None
        base_cls = base
    else:
        base_cls = parent
    namespace = _build_namespace(cls, extra)

    existing = frame.f_locals.get(cls.__name__) if frame else None
    target = (
        existing
        if isinstance(existing, type)
        and issubclass(existing, base)
        and existing.__dict__.get("_eco_forward_")
        else None
    )
    if target is None:
        meta = _get_eco_layout_meta(base)
        result = meta(cls.__name__, (base_cls,), namespace)
    else:
        skip = frozenset({"__module__", "__qualname__"})
        for key, value in namespace.items():
            if key in skip:
                continue
            setattr(target, key, value)
        result = target

    return result  # type: ignore
