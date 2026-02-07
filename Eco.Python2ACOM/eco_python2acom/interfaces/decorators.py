"""Decorators for declarative interface definitions.

This module provides decorators for defining ACOM interfaces
in a Pythonic, declarative way with proper inheritance support.

After decoration, interface classes are directly instantiable from
a VoidPtr, and all methods dispatch through the VTbl.

Key Components:
    @interface: Class decorator to define an ACOM interface.
    @method: Method decorator to mark interface methods.
    MethodDescriptor: Metadata container for method information.
    InterfaceDescriptor: Metadata container for interface information.

Example:
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX(IEcoUnknown):
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
    >>>
    >>> calc = IEcoCalculatorX(ptr)
    >>> calc.Addition(10, 20)
"""

from __future__ import annotations

from collections.abc import Callable
from dataclasses import dataclass, field
from typing import Any, ClassVar, Optional, TypeVar, Union, get_args, get_origin, get_type_hints

from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import FUNCTYPE, CastPtr, EcoStructure, Int16, Ptr, VoidPtr

# TypeVar for @method
F = TypeVar("F", bound=Callable[..., Any])

# TypeVar for @interface
C = TypeVar("C", bound=type)

# =============================================================================
# Method Descriptor
# =============================================================================


@dataclass
class MethodDescriptor:
    """Descriptor for an interface method.

    Stores metadata about a method including its name, parameter types,
    and return type. Used internally by the @method decorator.

    Attributes:
        name: The method name.
        param_types: List of ctypes parameter types (excluding 'self'/me pointer).
        return_type: The ctypes return type.
        doc: The method docstring.
    """

    name: str
    param_types: list[type] = field(default_factory=list)
    return_type: type = field(default=Int16)
    doc: Optional[str] = None


def _unwrap_optional(hint: type) -> type:
    """Extract the wrapped type from an Optional or Union type hint.

    Unwraps Optional[X] (which is Union[X, None]) to return just X.
    Non-Union types are returned unchanged.

    Args:
        hint: A type hint, potentially Optional[T] or Union[T, None].

    Returns:
        The unwrapped type if Optional/Union, otherwise the original hint.
    """
    if get_origin(hint) is Union:
        args = [arg for arg in get_args(hint) if arg is not type(None)]
        return next(iter(args))  # type: ignore
    return hint


def method(func: F) -> F:
    """Decorator to mark a method as an ACOM interface method.

    Extracts type hints from the function signature and creates a
    MethodDescriptor. The @interface decorator will later replace
    it with a real VTbl dispatcher.

    Typed as ``F -> F`` (identity) so the IDE preserves the original
    method signature for autocomplete and type checking.

    Args:
        func: The method function with type annotations.

    Returns:
        MethodDescriptor at runtime (typed as F for IDE).

    Example:
        >>> @method
        ... def Addition(self, a: Int16, b: Int16) -> Int32:
        ...     '''Add two numbers.'''
        ...     ...
    """
    hints = get_type_hints(func)
    return_type = hints.pop("return", Int16)

    # Get parameter types (skip 'self')
    param_types = [_unwrap_optional(t) for t in hints.values()]
    return_type = _unwrap_optional(return_type)

    descriptor = MethodDescriptor(
        name=func.__name__,
        param_types=param_types,
        return_type=return_type,
        doc=func.__doc__,
    )

    func._method_descriptor_ = descriptor
    return func


# =============================================================================
# Interface Descriptor
# =============================================================================


@dataclass
class InterfaceDescriptor:
    """Descriptor for an ACOM interface.

    Contains all metadata needed to generate ctypes structures
    for an interface.

    Attributes:
        name: The interface name.
        iid: The interface identifier.
        methods: List of ALL interface methods (including inherited).
        own_methods: List of methods defined in this interface only.
        vtbl_type: The generated VTbl ctypes structure.
        interface_type: The generated interface ctypes structure.
        interface_ptr: The POINTER type for this interface.
    """

    name: str
    iid: UGUID
    methods: list[MethodDescriptor] = field(default_factory=list)
    own_methods: list[MethodDescriptor] = field(default_factory=list)
    vtbl_type: Optional[type[EcoStructure]] = None
    interface_type: Optional[type[EcoStructure]] = None
    interface_ptr: Optional[type] = None


# =============================================================================
# Interface Decorator
# =============================================================================


def interface(iid: Union[str, UGUID], preamble: int = 0x01, length: int = 0x10) -> Callable[[C], C]:
    """Decorator to define an ACOM interface.

    This decorator transforms a class with @method-decorated stubs
    into a fully functional interface. After decoration the class:

    1. Can be instantiated from a ``VoidPtr``:  ``obj = IFoo(ptr)``
    2. Has real methods that dispatch through the C VTbl.
    3. Preserves type signatures so the IDE shows autocomplete.

    Inheritance is fully supported - methods from parent interfaces
    are automatically included in the VTbl in the correct order.

    Args:
        iid: The interface identifier as a GUID string or UGUID instance.
        preamble: UGUID preamble byte (default: 0x01).
        length: UGUID length byte (default: 0x10).

    Returns:
        A class decorator.

    Example:
        >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
        ... class IEcoCalculatorX(IEcoUnknown):
        ...     @method
        ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
        >>>
        >>> calc = IEcoCalculatorX(some_ptr)
        >>> calc.Addition(10, 20)
    """

    def decorator(cls: C) -> C:
        # Parse IID
        if isinstance(iid, str):
            guid = UGUID(iid, preamble=preamble)
            guid.Length = length
        else:
            guid = iid

        # Collect own methods from this class (preserving definition order)
        own_methods: list[MethodDescriptor] = []
        for attr in cls.__dict__.values():
            if hasattr(attr, "_method_descriptor_") and isinstance(
                attr._method_descriptor_, MethodDescriptor
            ):
                own_methods.append(attr._method_descriptor_)

        # Collect inherited methods from parent classes (in MRO order)
        inherited_methods: list[MethodDescriptor] = []
        for base in cls.__mro__:
            if base is cls:
                continue

            if hasattr(base, "_descriptor_") and base._descriptor_ is not None:
                inherited_methods = list(base._descriptor_.methods)
                break

        # Combine: inherited first, then own
        all_methods = inherited_methods + own_methods

        # Create interface descriptor
        descriptor = InterfaceDescriptor(
            name=cls.__name__,
            iid=guid,
            methods=all_methods,
            own_methods=own_methods,
        )

        # Generate ctypes structures
        _generate_ctypes_structures(descriptor)

        # Make the class directly instantiable from VoidPtr
        class_name = cls.__name__

        def __init__(self, ptr: VoidPtr) -> None:
            if not ptr:
                raise ValueError(f"{class_name}: NULL pointer")
            self._ptr = ptr
            self._vtbl = CastPtr(ptr, descriptor.interface_ptr).contents.pVTbl.contents  # type: ignore

        cls.__init__ = __init__  # type: ignore

        # Replace each @method stub with a real VTbl dispatcher
        def _make_dispatch(name: str, doc: Optional[str]) -> Callable[..., Any]:
            def dispatch(self, *args: Any) -> Any:
                return getattr(self._vtbl, name)(self._ptr, *args)

            dispatch.__name__ = name
            dispatch.__doc__ = doc
            return dispatch

        for method in all_methods:
            setattr(cls, method.name, _make_dispatch(method.name, method.doc))

        def __repr__(self) -> str:
            addr = self._ptr.value if self._ptr else 0
            return f"<{class_name} at 0x{addr:X}>"

        cls.__repr__ = __repr__  # type: ignore

        # Store metadata on the class
        cls._descriptor_ = descriptor
        cls._iid_ = guid
        cls._vtbl_type_ = descriptor.vtbl_type
        cls._interface_type_ = descriptor.interface_type
        cls._interface_ptr_ = descriptor.interface_ptr

        return cls

    return decorator


def _generate_ctypes_structures(descriptor: InterfaceDescriptor) -> None:
    """Generate ctypes VTbl and interface structures for a descriptor.

    Creates:
    1. VTbl structure with all methods (inherited + own)
    2. Interface structure with pVTbl pointer
    3. Pointer type for the interface

    Args:
        descriptor: The interface descriptor to generate structures for.
    """
    # Build VTbl fields from all methods
    vtbl_fields: list[tuple[str, type]] = []

    for method_desc in descriptor.methods:
        # Build function type: return_type, me_ptr, *param_types
        func_type = FUNCTYPE(
            method_desc.return_type,
            VoidPtr,
            *method_desc.param_types,
        )
        vtbl_fields.append((method_desc.name, func_type))

    # Create VTbl structure
    class VTblType(EcoStructure):
        _fields_: ClassVar[list[tuple[str, type]]] = vtbl_fields

    VTblType.__name__ = f"{descriptor.name}VTbl"
    VTblType.__qualname__ = f"{descriptor.name}VTbl"

    # Create interface structure
    class InterfaceType(EcoStructure):
        pass

    InterfaceType.__name__ = descriptor.name
    InterfaceType.__qualname__ = descriptor.name
    InterfaceType._fields_ = [("pVTbl", Ptr(VTblType))]

    # Create pointer type
    interface_ptr = Ptr(InterfaceType)

    # Store in descriptor
    descriptor.vtbl_type = VTblType
    descriptor.interface_type = InterfaceType
    descriptor.interface_ptr = interface_ptr
