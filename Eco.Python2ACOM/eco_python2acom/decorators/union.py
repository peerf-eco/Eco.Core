"""Decorator for defining EcoOS C unions from annotated classes.

Use this decorator on classes that contain field annotations.
"""

import inspect
from typing import TypeVar, dataclass_transform

from eco_python2acom.decorators.utils import eco_class
from eco_python2acom.types.core import CUnion

C = TypeVar("C", bound=type)


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
        TypeError: If the class uses multiple inheritance or inherits
        from a non-structure base.
    """
    return eco_class(
        cls,
        base=CUnion,
        frame=inspect.currentframe().f_back,
        extra={"_eco_union_": True},
    )  # type: ignore


__all__ = ["union"]
