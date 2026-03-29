"""Decorator for defining ACOM interface classes.

Use this decorator on classes that represent a single pointer with an associated IID.
"""

from __future__ import annotations

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.decorators.utils import (
    EcoStructMeta,
    build_namespace,
    register_class,
    validate_bases,
)
from eco_python2acom.types.guid import UGUID

C = TypeVar("C", bound=type)


def interface(
    iid: Union[str, UGUID],
    preamble: int = 0x01,
    length: int = 0x10,
) -> Callable[[C], C]:
    """Decorator for defining EcoOS ACOM interface classes.

    Converts a class with method signatures into an interface type suitable
    for ACOM-style interaction with EcoOS.

    Args:
        iid: Interface ID as a string or UGUID instance.
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

        bases = validate_bases(cls, "Interface")
        namespace = build_namespace(cls, {"_iid_": guid, "_eco_interface_": True})
        new_class = EcoStructMeta(cls.__name__, bases, namespace)

        frame = inspect.currentframe()
        if frame is not None and frame.f_back is not None:
            register_class(frame.f_back, new_class)

        return new_class

    return decorator


__all__ = ["interface"]
