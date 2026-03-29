"""Decorator for defining EcoOS C structures from annotated classes.

Use this decorator on classes that contain field annotations.
"""

from __future__ import annotations

import inspect
from typing import TypeVar, dataclass_transform

from eco_python2acom.decorators.utils import (
    EcoStructMeta,
    build_namespace,
    register_class,
    validate_bases,
)

C = TypeVar("C", bound=type)


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
        TypeError: If the class uses multiple inheritance or inherits
        from a non-structure base.
    """
    bases = validate_bases(cls, "Structure")
    namespace = build_namespace(cls, {"_eco_model_": True})
    new_class = EcoStructMeta(cls.__name__, bases, namespace)

    frame = inspect.currentframe()
    if frame is not None and frame.f_back is not None:
        register_class(frame.f_back, new_class)

    return new_class


__all__ = ["model"]
