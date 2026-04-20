"""Decorators for defining EcoOS C structures and unions from annotated classes.

Use these decorators on classes that contain field annotations.

Exports:
    `model`: Decorator for a C-compatible structure.
    `union`: Decorator for a C-compatible union.
    `stub`: Forward declaration helper for any EcoOS layout class.
"""

import inspect
from typing import TypeVar, dataclass_transform

from eco_python2acom.decorators.utils import _eco_class, _finalize
from eco_python2acom.types.core import CStructure, CUnion

C = TypeVar("C", bound=type)


def _validate_layout_body(cls: type) -> None:
    """Validate body of a `@model` or `@union` class.

    Args:
        cls: The class being decorated.

    Raises:
        TypeError: If a bare attribute without annotation is found.
    """
    annotations = getattr(cls, "__annotations__", {}) or {}
    for key, value in cls.__dict__.items():
        if key.startswith("__") and key.endswith("__"):
            continue
        if key in annotations:
            continue
        if isinstance(value, classmethod | staticmethod | property):
            continue
        if callable(value):
            continue
        raise TypeError(f"'{cls.__name__}.{key}' has no type annotation")


@dataclass_transform()
def model(cls: C) -> C:
    """Decorator for defining EcoOS C Structure data classes.

    Converts a class with field annotations into a C-compatible structure
    type suitable for interop with EcoOS.

    Args:
        cls: Class with field annotations to convert.

    Returns:
        C structure class ready for use with EcoOS.

    Raises:
        TypeError: If the class uses multiple inheritance or inherits from a non-structure base.
    """
    new_class = _eco_class(
        cls,
        base=CStructure,
        frame=inspect.currentframe().f_back,
        extra={"_eco_model_": True, "_eco_forward_": False},
        validator=_validate_layout_body,
    )
    return _finalize(new_class)  # type: ignore


@dataclass_transform()
def union(cls: C) -> C:
    """Decorator for defining EcoOS C Union data classes.

    Converts a class with field annotations into a C-compatible union
    type suitable for interop with EcoOS.

    Args:
        cls: Class with field annotations to convert.

    Returns:
        C union class ready for use with EcoOS.

    Raises:
        TypeError: If the class uses multiple inheritance or inherits from a non-structure base.
    """
    new_class = _eco_class(
        cls,
        base=CUnion,
        frame=inspect.currentframe().f_back,
        extra={"_eco_union_": True, "_eco_forward_": False},
        validator=_validate_layout_body,
    )
    return _finalize(new_class)  # type: ignore


def stub(name: str, base: type = CStructure) -> type:
    """Forward-declare an EcoOS layout class.

    Creates an empty placeholder under the given name so that sibling
    definitions can reference `Ptr[name]` before the full definition
    is seen. Works for any EcoOS base (interfaces, structures, unions).

    Args:
        name: Name of the class to forward-declare.
        base: Base class for the stub (`CStructure` by default; pass
            `CUnion` for union forward declarations).

    Returns:
        Empty placeholder acting as a forward declaration.
    """
    frame = inspect.currentframe().f_back
    module = frame.f_globals.get("__name__")
    placeholder = type(name, (), {"__module__": module})

    return _eco_class(
        placeholder,
        base=base,
        frame=frame,
        extra={"_eco_forward_": True},
    )  # type: ignore


__all__ = ["model", "stub", "union"]
