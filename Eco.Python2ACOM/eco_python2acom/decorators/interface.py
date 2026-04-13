"""Decorator for defining ACOM interface classes.

Use this decorator on classes that represent a single pointer with an associated IID.
"""

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.decorators.utils import eco_class
from eco_python2acom.types.core import CStructure
from eco_python2acom.types.guid import UGUID

C = TypeVar("C", bound=type)


def interface(iid: Union[str, UGUID], preamble: int = 0x01, length: int = 0x10) -> Callable[[C], C]:
    """Decorator for defining EcoOS ACOM interface classes.

    Converts a class with method signatures into an interface type suitable
    for ACOM-style interaction with EcoOS.

    Args:
        iid: Interface ID.
        preamble: Preamble byte for UGUID.
        length: Length byte for UGUID.

    Returns:
        A decorator that converts the class into an interface class ready
        for use with EcoOS.

    Raises:
        TypeError: If the class uses multiple inheritance or inherits
        from a non-structure base.
    """

    def decorator(cls: C) -> C:
        if isinstance(iid, str):
            guid = UGUID(iid, preamble=preamble, length=length)
        else:
            guid = iid

        return eco_class(
            cls,
            base=CStructure,
            frame=inspect.currentframe().f_back,
            extra={"_iid_": guid, "_eco_interface_": True},
        )  # type: ignore

    return decorator


__all__ = ["interface"]
