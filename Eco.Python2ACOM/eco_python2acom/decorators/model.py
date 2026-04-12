"""Decorator for defining EcoOS C structures from annotated classes.

Use this decorator on classes that contain field annotations.
"""

import inspect
from typing import TypeVar, dataclass_transform

from eco_python2acom.decorators.utils import eco_class

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
    return eco_class(
        cls,
        kind="Model",
        frame=inspect.currentframe().f_back,
        extra={"_eco_model_": True},
    )  # type: ignore


__all__ = ["model"]
