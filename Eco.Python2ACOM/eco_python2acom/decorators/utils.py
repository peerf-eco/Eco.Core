"""Utilities for @model and @interface decorators.

This module provides helper tools for building EcoOS structure and interface classes.
"""

from collections.abc import Callable
from types import FrameType, NoneType
from typing import Any, ClassVar, Optional, Union, get_args, get_origin, get_type_hints

from eco_python2acom.types.core import CData, CFuncType, CLayout, Int16, Void
from eco_python2acom.types.pointer import Ptr

# -----------------------------------------------------------------------------
# Type validation and normalization
# -----------------------------------------------------------------------------


def validate(hint: type) -> type:
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


def normalize(hint: type) -> type:
    """Normalize EcoOS data type.

    Args:
        hint: The type hint to normalize.

    Returns:
        The normalized data type.

    Raises:
        TypeError: If union has more than one non-None type.
    """
    if get_origin(hint) is Union:
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
    hints = get_type_hints(
        cls,
        globalns=getattr(cls, "__decl_globalns__", {}),
        localns=getattr(cls, "__decl_localns__", {}),
    )

    return [
        (name, validate(normalize(hints[name])))
        for name in getattr(cls, "__annotations__", {})
        if get_origin(hints.get(name)) is not ClassVar
    ]


def _resolve_methods(cls: type) -> list[tuple[str, type]]:
    """Resolve EcoOS interface methods defined in this class only.

    Args:
        cls: The interface class to resolve methods of.

    Returns:
        A list of tuples with method names and resolved EcoOS types.

    Note:
        Sets cls._eco_method_params_ with parameter names for dispatchers.
        Sets cls._eco_method_returns_ with return types for result wrapping.
    """
    methods: list[tuple[str, type]] = []
    method_params: dict[str, list[str]] = {}
    method_returns: dict[str, type] = {}

    for name, value in cls.__dict__.items():
        if name.startswith("__") and name.endswith("__"):
            continue
        if not callable(value):
            continue

        hints = get_type_hints(
            value,
            globalns=getattr(cls, "__decl_globalns__", {}),
            localns=getattr(cls, "__decl_localns__", {}),
        )

        return_type = validate(normalize(hints.pop("return", Int16)))
        param_names, param_types = [], []
        for key, hint in hints.items():
            if key not in ("self", "cls"):
                param_types.append(validate(normalize(hint)))
                param_names.append(key)

        func_type = CFuncType(return_type, Ptr[Void], *param_types)  # type: ignore
        field_name = f"_func_{name}"
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

    The methods are stored as fields in the interface class with names
    like '_func_{method_name}'. This function creates Python method wrappers
    that:
    1. Access the function pointer field from the structure
    2. Call it with `ptr` as the first argument
    3. Support both positional and keyword arguments
    4. Wrap the result in the declared return type

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
                func_ptr = getattr(getattr(self, "vtbl", self), field_name)
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
                    result = func_ptr(self.ptr, *full_args)
                else:
                    result = func_ptr(self.ptr, *args)

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


def finalize(cls: type) -> None:
    """Finalize an EcoOS structure or interface class.

    Resolves field and method annotations, validates the class layout,
    and prepares it for use with EcoOS.

    Args:
        cls: The class to finalize.

    Raises:
        TypeError: If field or method types cannot be resolved.
    """
    if cls.__dict__.get("_eco_ready_"):
        return

    methods = _apply_resolver(cls, "_eco_interface_", _resolve_methods)
    model_fields = _apply_resolver(cls, "_eco_model_", _resolve_fields)
    union_fields = _apply_resolver(cls, "_eco_union_", _resolve_fields)

    cls._fields_ = union_fields + model_fields + methods
    cls._eco_ready_ = True

    if methods:
        _install_dispatchers(cls, methods)


# -----------------------------------------------------------------------------
# Metaclass for EcoOS structures and unions
# -----------------------------------------------------------------------------

_eco_layout_meta_cache: dict[type, type] = {}


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
    fields = getattr(cls, "_fields_", [])
    field_strs = []

    # For interfaces: show ptr as hex address
    if hasattr(self, "ptr") and hasattr(self.ptr, "value"):
        addr = self.ptr.value or 0
        field_strs.append(f"ptr=0x{addr:X}")

    for field_name, _ in fields:
        if field_name.startswith("_func_"):
            continue
        try:
            value = getattr(self, field_name)
            field_strs.append(f"{field_name}={value!r}")
        except Exception:
            field_strs.append(f"{field_name}=<error>")

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
        """Metaclass for EcoOS C structures and unions with lazy field resolution.

        - On class creation: finalizes bases.
        - On `_fields_` access or instance creation: finalizes the class.
        - Adds `__eq__` and `__repr__` methods for better debugging.
        """

        def __new__(
            cls, name: str, bases: tuple[type, ...], namespace: dict[str, Any], **kwargs: Any
        ) -> type:
            """Create the class and finalize bases."""
            for base in reversed(bases):
                if issubclass(base, CLayout) and base not in CLayout.__args__:
                    finalize(base)

            if "__eq__" not in namespace:
                namespace["__eq__"] = _struct_eq
            if "__repr__" not in namespace:
                namespace["__repr__"] = _struct_repr

            return super().__new__(cls, name, bases, namespace, **kwargs)  # type: ignore

        def __call__(cls, *args: Any, **kwargs: Any) -> Any:
            """Create instance, ensuring fields are resolved."""
            finalize(cls)
            return super().__call__(*args, **kwargs)

        def __getattribute__(cls, name: str) -> Any:
            """Intercept `_fields_` access to trigger lazy resolution."""
            if name == "_fields_":
                finalize(cls)
            return super().__getattribute__(name)

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
        "_eco_ready_": False,
    }

    if extra:
        namespace.update(extra)

    skip = frozenset({"__dict__", "__weakref__", "_fields_"})
    for key, value in cls.__dict__.items():
        if key in namespace or key in skip:
            continue
        namespace[key] = value

    return namespace


def _register_class(frame: FrameType, cls: type) -> None:
    """Register a class in the frame namespace for forward reference resolution.

    Sets `cls.__decl_localns__` and `cls.__decl_globalns__`.

    Args:
        frame: Caller's frame where the decorated class is defined.
        cls: The newly created class to register.
    """
    if "__type_registry__" not in frame.f_locals:
        frame.f_locals["__type_registry__"] = {}

    registry = frame.f_locals["__type_registry__"]
    registry.update(
        {key: value for key, value in frame.f_locals.items() if isinstance(value, type)}
    )

    registry[cls.__name__] = cls
    cls.__decl_localns__ = registry  # type: ignore
    cls.__decl_globalns__ = frame.f_globals  # type: ignore


def eco_class(
    cls: type,
    base: type,
    frame: FrameType | None,
    extra: dict[str, Any] | None = None,
) -> type:
    """Create an `EcoLayoutMeta` class from a decorated class.

    Args:
        cls: The original class being decorated.
        base: The default base type.
        frame: Caller's frame for namespace registration.
        extra: Additional attributes to add to namespace.

    Returns:
        The new class created by `EcoLayoutMeta`.
    """
    base_cls = _resolve_base(cls, base)
    namespace = _build_namespace(cls, extra)
    meta = _get_eco_layout_meta(base)
    new_class = meta(cls.__name__, (base_cls,), namespace)

    if frame is not None:
        _register_class(frame, new_class)

    return new_class  # type: ignore


__all__ = ["eco_class", "finalize", "normalize", "validate"]
