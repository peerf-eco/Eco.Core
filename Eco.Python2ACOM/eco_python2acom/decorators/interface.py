"""Decorator for defining ACOM interface classes.

Use this decorator on classes that represent an ACOM interface instance
(typically a single `vtbl` pointer field) with an associated IID.
"""

import inspect
from collections.abc import Callable
from typing import TypeVar, Union

from eco_python2acom.decorators.utils import _eco_class, _finalize
from eco_python2acom.types.core import CStructure
from eco_python2acom.types.guid import UGUID

C = TypeVar("C", bound=type)


def _validate_interface_body(cls: type) -> None:
    """Validate body of an `@interface` class.

    Args:
        cls: The class being decorated.

    Raises:
        TypeError: If an invalid method kind or unannotated parameter is found.
    """
    for name, value in cls.__dict__.items():
        if name.startswith("__") and name.endswith("__"):
            continue
        if isinstance(value, classmethod | staticmethod | property):
            raise TypeError(
                f"'{cls.__name__}.{name}' must not be a `classmethod`, `staticmethod`, or `property`"
            )
        if not inspect.isfunction(value):
            continue

        signature = inspect.signature(value)
        if not signature.parameters:
            raise TypeError(f"'{cls.__name__}.{name}' must declare 'self' as the first parameter")

        parameters = iter(signature.parameters.values())
        next(parameters)  # Skip 'self' parameter wout type annotation
        for param in parameters:
            if param.annotation is inspect.Parameter.empty:
                raise TypeError(
                    f"'{cls.__name__}.{name}' parameter '{param.name}' has no type annotation"
                )
        if signature.return_annotation is inspect.Signature.empty:
            raise TypeError(f"'{cls.__name__}.{name}' has no return type annotation")


def interface(iid: Union[str, UGUID], preamble: int = 0x01, length: int = 0x10) -> Callable[[C], C]:
    """Decorator for defining EcoOS ACOM interface classes.

    Converts a class with method signatures into an interface type suitable
    for ACOM-style interaction with EcoOS. Every interface is laid out as a flat
    `{ vtbl* }` struct, regardless of declared inheritance.

    Args:
        iid: Interface ID as a string or pre-built `UGUID`.
        preamble: Preamble byte for UGUID when `iid` is a string.
        length: Length byte for UGUID when `iid` is a string.

    Returns:
        A decorator that converts the class into an interface EcoOS class.

    Raises:
        TypeError: If the class uses multiple inheritance or inherits from a non-structure base.
    """

    def decorator(cls: C) -> C:
        """Convert a class into a C-compatible interface structure.

        Applies IID metadata, supports forward-declared stubs, and replaces them
        if a matching placeholder exists in the defining scope.
        """
        if isinstance(iid, UGUID):
            guid = iid
        else:
            guid = UGUID(iid, preamble=preamble, length=length)

        new_class = _eco_class(
            cls,
            base=CStructure,
            frame=inspect.currentframe().f_back,
            extra={"_iid_": guid, "_eco_interface_": True, "_eco_forward_": False},
            validator=_validate_interface_body,
            flatten=True,
        )
        return _finalize(new_class)  # type: ignore

    return decorator


__all__ = ["interface"]
