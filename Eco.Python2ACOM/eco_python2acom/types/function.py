"""Generic function pointer type for EcoOS/ACOM.

This module provides the `Func[R, [A, B, ...]]` generic type for type-safe
`__stdcall` function pointers used by EcoOS interface methods and callbacks.
"""

from ctypes import WINFUNCTYPE
from typing import TYPE_CHECKING, Generic, ParamSpec, TypeVar

from eco_python2acom.types.core import TYPE_NAMES, Void
from eco_python2acom.types.utils import addressof

R = TypeVar("R")
P = ParamSpec("P")


if TYPE_CHECKING:

    class Func(Generic[R, P]):
        """Generic `__stdcall` function type.

        Type Parameters:
            R: The return type.
            P: The parameter types (as a `[A, B, ...]` list).

        Attributes:
            _restype_: The return type of the function.
            _argtypes_: The tuple of argument types.
        """

        _restype_: type[R]
        _argtypes_: tuple[type, ...]

else:

    def _type_label(cls: type) -> str:
        """Return a display label for a type, preferring `TYPE_NAMES`."""
        return TYPE_NAMES.get(cls, getattr(cls, "__name__", str(cls)))

    class _FuncMeta(type):
        """Metaclass that enables `Func[R, [A, B, ...]]` subscript syntax.

        The subscript must be a tuple of (return type, [argument types...]).

        Attributes:
            _cache: Dictionary mapping (restype, argtypes) tuples to function classes.
        """

        _cache: dict[tuple[type, tuple[type, ...]], type] = {}

        def __getitem__(cls, params: tuple[type, list[type]]) -> type:
            """Create or retrieve a function type for the given signature.

            Args:
                params: A tuple of (restype, [argtypes...]) where:
                    - restype: The return type (any EcoOS type or `Void`).
                    - argtypes: A list of argument types (EcoOS types, not `Void`).

            Returns:
                A function class usable as a callback type.
            """
            # Validate parameters
            if not isinstance(params, tuple) or len(params) != 2:
                raise ValueError(
                    f"Func requires 2 parameters: Func[Return, [Arg, ...]], got '{params}'"
                )

            restype, argtypes = params

            if not isinstance(argtypes, list):
                raise TypeError(
                    f"Func argument types must be a list, got '{type(argtypes).__name__}'"
                )

            if not isinstance(restype, type):
                raise TypeError(f"Func return type must be a type, got '{type(restype).__name__}'")

            for index, arg in enumerate(argtypes):
                if not isinstance(arg, type):
                    raise TypeError(
                        f"Func argument #{index} must be a type, got '{type(arg).__name__}'"
                    )
                if arg is Void:
                    raise ValueError(
                        f"Func argument #{index} cannot be 'Void': argument has no size"
                    )

            # Check cache
            key = (restype, tuple(argtypes))
            if key in cls._cache:
                return cls._cache[key]

            # Build display name
            restype_name = _type_label(restype)
            argtypes_name = ", ".join(_type_label(arg) for arg in argtypes)
            display_name = f"Func[{restype_name}, [{argtypes_name}]]"

            # Create function type
            try:
                func_type = WINFUNCTYPE(restype, *argtypes)
            except TypeError as err:
                raise TypeError(f"Cannot create '{display_name}': {err}") from err

            # Create smart function wrapper class
            class SmartFunc(func_type):
                """Runtime function pointer implementation."""

                _flags_ = func_type._flags_
                _restype_ = func_type._restype_
                _argtypes_ = func_type._argtypes_

                def __repr__(self) -> str:
                    """String representation with signature and address."""
                    if not bool(self):
                        return f"<{display_name} NULL>"
                    return f"<{display_name} 0x{addressof(self):X}>"

            SmartFunc.__name__ = display_name
            SmartFunc.__qualname__ = display_name

            # Cache and return
            cls._cache[key] = SmartFunc
            return SmartFunc

    class Func(metaclass=_FuncMeta):
        """Generic `__stdcall` function type.

        `Func[R, [A, B, ...]]` is the Python equivalent of a C function
        pointer `R (__stdcall *)(A, B, ...)`. Subscripting creates a concrete
        callback class.

        Type Parameters:
            R: The return type.
            P: The parameter types (as a `[A, B, ...]` list).

        Attributes:
            _restype_: The return type of the function.
            _argtypes_: The tuple of argument types.

        Example:
            ```python
            from eco_python2acom.types.function import Func
            from eco_python2acom.types.core import Int32

            EcoCompareFunc = Func[Int32, [Int32, Int32]]

            def ascending(a: int, b: int) -> int:
                return a - b

            cmp = EcoCompareFunc(ascending)
            ```
        """

        pass


__all__ = ["Func"]
