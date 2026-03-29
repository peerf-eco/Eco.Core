"""Internal utilities for @model and @interface decorators.

This module provides helper functions, metaclasses, and field/method resolution
for building EcoOS structure and interface classes.
"""

from __future__ import annotations

from collections.abc import Callable
from types import FrameType, NoneType
from typing import Any, ClassVar, Optional, Union, get_args, get_origin, get_type_hints

from eco_python2acom.types.core import CData, CFuncType, CStructure, Int16, Void
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


def resolve_fields(cls: type) -> list[tuple[str, type]]:
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


def resolve_methods(cls: type) -> list[tuple[str, type]]:
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


def install_dispatchers(cls: type, methods: list[tuple[str, type]]) -> None:
    """Replace method stubs with C function dispatchers for an interface class.

    The methods are stored as fields in the interface class with names
    like '_func_{method_name}'. This function creates Python method wrappers
    that:
    1. Access the function pointer field from the structure
    2. Call it with self.ptr as the first argument
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
        original = cls.__dict__.get(method_name)
        doc = getattr(original, "__doc__", None) if callable(original) else None
        param_names = method_params.get(field_name, [])
        return_type = method_returns.get(field_name)

        def make_dispatch(
            fname: str,
            mname: str,
            pnames: list[str],
            rtype: Optional[type] = None,
            docstring: Optional[str] = None,
        ) -> Callable[..., Any]:
            """Create a dispatcher closure for a specific method."""

            def dispatch(self, *args: Any, **kwargs: Any) -> Any:
                vtbl = getattr(self, "vtbl", self)
                func_ptr = getattr(vtbl, fname)
                if kwargs:
                    full_args = list(args)
                    for i, pname in enumerate(pnames):
                        if i < len(args):
                            continue
                        if pname in kwargs:
                            full_args.append(kwargs[pname])
                        else:
                            raise TypeError(f"{mname}() missing required argument: '{pname}'")
                    result = func_ptr(self.ptr, *full_args)
                else:
                    result = func_ptr(self.ptr, *args)

                if rtype is not None and result is not None:
                    return rtype(result)
                return result

            dispatch.__name__ = mname
            dispatch.__doc__ = docstring
            return dispatch

        setattr(
            cls, method_name, make_dispatch(field_name, method_name, param_names, return_type, doc)
        )


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

    methods: list[tuple[str, type]] = []
    if cls.__dict__.get("_eco_interface_"):
        try:
            methods = resolve_methods(cls)
        except TypeError:
            raise
        except Exception as err:
            raise TypeError(
                f"Interface '{cls.__name__}': contains unresolved method types"
            ) from err

    fields: list[tuple[str, type]] = []
    if cls.__dict__.get("_eco_model_"):
        try:
            fields = resolve_fields(cls)
        except TypeError:
            raise
        except Exception as err:
            raise TypeError(f"Model '{cls.__name__}': contains unresolved field types") from err

    cls._fields_ = fields + methods
    cls._eco_ready_ = True

    if methods:
        install_dispatchers(cls, methods)


# -----------------------------------------------------------------------------
# Metaclass for EcoOS structures
# -----------------------------------------------------------------------------

StructureMeta = type(CStructure)


def struct_eq(self, other: Any) -> bool:
    """Compare two EcoOS structures by field values."""
    if type(self) is not type(other):
        return False
    for field_name, _ in getattr(type(self), "_fields_", []):
        if getattr(self, field_name) != getattr(other, field_name):
            return False
    return True


def struct_repr(self: Any) -> str:
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


class EcoStructMeta(StructureMeta):  # type: ignore[misc]
    """Metaclass for EcoOS C structures with lazy field resolution.

    - On class creation: finalizes bases.
    - On _fields_ access or instance creation: finalizes the class.
    - Adds __eq__ and __repr__ methods for better debugging.
    """

    def __new__(
        cls, name: str, bases: tuple[type, ...], namespace: dict[str, Any], **kwargs: Any
    ) -> type:
        """Create the class and finalize bases."""
        for base in reversed(bases):
            if issubclass(base, CStructure) and base != CStructure:
                finalize(base)

        if "__eq__" not in namespace:
            namespace["__eq__"] = struct_eq
        if "__repr__" not in namespace:
            namespace["__repr__"] = struct_repr

        return super().__new__(cls, name, bases, namespace, **kwargs)  # type: ignore

    def __call__(cls, *args: Any, **kwargs: Any) -> Any:
        """Create instance, ensuring fields are resolved."""
        finalize(cls)
        return super().__call__(*args, **kwargs)

    def __getattribute__(cls, name: str) -> Any:
        """Intercept _fields_ access to trigger lazy resolution."""
        if name == "_fields_":
            finalize(cls)
        return super().__getattribute__(name)


# -----------------------------------------------------------------------------
# Decorator helpers
# -----------------------------------------------------------------------------


def validate_bases(cls: type, kind: str) -> tuple[type, ...]:
    """Validate class inheritance and return bases tuple for EcoStructMeta.

    Args:
        cls: The class being decorated.
        kind: Descriptor for error messages.

    Returns:
        Tuple of base classes for the new EcoStructMeta class.

    Raises:
        TypeError: If multiple inheritance is used or base is invalid.
    """
    if len(cls.__bases__) > 1:
        raise TypeError(f"{kind} '{cls.__name__}' uses multiple inheritance")

    base = next(iter(cls.__bases__)) if cls.__bases__ else object
    if base is not object and not issubclass(base, CStructure):
        raise TypeError(
            f"{kind} '{cls.__name__}' inherits from non-structure base '{base.__name__}'"
        )

    return (base,) if base is not object else (CStructure,)


def build_namespace(cls: type, extra: dict[str, Any] | None = None) -> dict[str, Any]:
    """Build namespace dict for a new EcoStructMeta class.

    Copies module, qualname, doc, annotations, and all non-special attributes
    from the original class. Optionally merges extra attributes.

    Args:
        cls: The original class being decorated.
        extra: Additional attributes to add to namespace.

    Returns:
        Namespace dict ready for EcoStructMeta.
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


def register_class(frame: FrameType, cls: type) -> None:
    """Register a class in the frame namespace for forward reference resolution.

    Sets 'cls.__decl_localns__' and 'cls.__decl_globalns__' so that
    get_type_hints can resolve forward references in annotations.

    Args:
        frame: Caller's frame where the decorated class is defined.
        cls: The newly created class to register.
    """
    if "__type_registry__" not in frame.f_locals:
        frame.f_locals["__type_registry__"] = {}

    registry = frame.f_locals["__type_registry__"]
    registry.update({k: v for k, v in frame.f_locals.items() if isinstance(v, type)})

    registry[cls.__name__] = cls
    cls.__decl_localns__ = registry  # type: ignore[attr-defined]
    cls.__decl_globalns__ = frame.f_globals  # type: ignore[attr-defined]
