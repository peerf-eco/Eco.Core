"""Decorators for declarative interface definitions.

This module provides decorators and utilities for defining ACOM interfaces
in a Pythonic, declarative way. Instead of manually creating ctypes structures,
you can use class decorators and method annotations.

Example:
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>> from eco_python2acom.core.types import Int16, Int32
    >>> from eco_python2acom.core import UGUID
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    >>> class IEcoCalculatorX:
    ...     '''Calculator X interface.'''
    ...
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32:
    ...         '''Add two numbers.'''
    ...         ...
    ...
    ...     @method
    ...     def Subtraction(self, a: Int16, b: Int16) -> Int16:
    ...         '''Subtract two numbers.'''
    ...         ...
    >>>
    >>> # Access generated ctypes structures
    >>> vtbl_type = IEcoCalculatorX._vtbl_type_
    >>> interface_type = IEcoCalculatorX._interface_type_
"""

from __future__ import annotations

import ctypes
from collections.abc import Callable
from dataclasses import dataclass, field
from typing import Any, ClassVar, Optional, get_type_hints

from eco_python2acom.core.guid import UGUID, UGUIDPtr
from eco_python2acom.core.types import FUNCTYPE, Int16, UInt32, VoidPtrPtr

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


def method(func: Callable[..., Any]) -> MethodDescriptor:
    """Decorator to mark a method as an interface method.

    The decorator extracts type hints from the function signature
    and creates a MethodDescriptor that will be used to generate
    the ctypes function type.

    Args:
        func: The method function with type annotations.

    Returns:
        A descriptor containing the method metadata.

    Example:
        >>> @method
        ... def Addition(self, a: Int16, b: Int16) -> Int32:
        ...     '''Add two numbers.'''
        ...     ...
    """
    hints = get_type_hints(func)
    return_type = hints.pop("return", Int16)

    # Get parameter types (skip 'self')
    param_types = list(hints.values())

    return MethodDescriptor(
        name=func.__name__,
        param_types=param_types,
        return_type=return_type,
        doc=func.__doc__,
    )


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
        methods: List of interface methods.
        vtbl_type: The generated VTbl ctypes structure.
        interface_type: The generated interface ctypes structure.
        interface_ptr: The POINTER type for this interface.
    """

    name: str
    iid: UGUID
    methods: list[MethodDescriptor] = field(default_factory=list)
    vtbl_type: Optional[type[ctypes.Structure]] = None
    interface_type: Optional[type[ctypes.Structure]] = None
    interface_ptr: Optional[type] = None


# Registry of all defined interfaces
_interface_registry: dict[str, InterfaceDescriptor] = {}


def get_interface(name: str) -> Optional[InterfaceDescriptor]:
    """Get an interface descriptor by name.

    Args:
        name: The interface name.

    Returns:
        The interface descriptor, or None if not found.
    """
    return _interface_registry.get(name)


def get_all_interfaces() -> dict[str, InterfaceDescriptor]:
    """Get all registered interface descriptors.

    Returns:
        Dictionary mapping interface names to descriptors.
    """
    return _interface_registry.copy()


# =============================================================================
# Interface Decorator
# =============================================================================


def interface(iid: str | UGUID, preamble: int = 0x01, length: int = 0x10) -> Callable[[type], type]:
    """Decorator to define an ACOM interface.

    This decorator transforms a class with @method-decorated methods
    into an interface definition, generating the necessary ctypes
    structures for VTbl and the interface itself.

    Args:
        iid: The interface identifier as a GUID string or UGUID instance.
        preamble: UGUID preamble byte (default: 0x01).
        length: UGUID length byte (default: 0x10).

    Returns:
        A class decorator.

    Example:
        >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
        ... class IEcoCalculatorX:
        ...     @method
        ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
    """

    def decorator(cls: type) -> type:
        # Parse IID
        if isinstance(iid, str):
            guid = UGUID(iid, preamble=preamble)
            guid.Length = length
        else:
            guid = iid

        # Collect methods
        methods: list[MethodDescriptor] = []
        for attr in cls.__dict__.values():
            if isinstance(attr, MethodDescriptor):
                methods.append(attr)

        # Create interface descriptor
        descriptor = InterfaceDescriptor(
            name=cls.__name__,
            iid=guid,
            methods=methods,
        )

        # Generate ctypes structures
        _generate_ctypes_structures(descriptor)

        # Store metadata on the class
        cls._descriptor_ = descriptor
        cls._iid_ = guid
        cls._vtbl_type_ = descriptor.vtbl_type
        cls._interface_type_ = descriptor.interface_type
        cls._interface_ptr_ = descriptor.interface_ptr

        # Register interface
        _interface_registry[cls.__name__] = descriptor

        return cls

    return decorator


def _generate_ctypes_structures(descriptor: InterfaceDescriptor) -> None:
    """Generate ctypes VTbl and interface structures for a descriptor.

    Args:
        descriptor: The interface descriptor to generate structures for.
    """

    # Create the interface structure (empty, for forward reference)
    class InterfaceType(ctypes.Structure):
        pass

    InterfaceType.__name__ = descriptor.name
    InterfaceType.__qualname__ = descriptor.name

    # Create pointer type
    interface_ptr = ctypes.POINTER(InterfaceType)

    # Build VTbl fields
    # Start with IEcoUnknown methods
    vtbl_fields: list[tuple[str, type]] = [
        ("QueryInterface", FUNCTYPE(Int16, interface_ptr, UGUIDPtr, VoidPtrPtr)),
        ("AddRef", FUNCTYPE(UInt32, interface_ptr)),
        ("Release", FUNCTYPE(UInt32, interface_ptr)),
    ]

    # Add interface-specific methods
    for method_desc in descriptor.methods:
        # Build function type: return_type, self_ptr, *param_types
        func_type = FUNCTYPE(
            method_desc.return_type,
            interface_ptr,
            *method_desc.param_types,
        )
        vtbl_fields.append((method_desc.name, func_type))

    # Create VTbl structure
    class VTblType(ctypes.Structure):
        _fields_: ClassVar[list[tuple[str, type]]] = vtbl_fields

    VTblType.__name__ = f"{descriptor.name}VTbl"
    VTblType.__qualname__ = f"{descriptor.name}VTbl"

    # Now set the interface structure fields
    InterfaceType._fields_ = [("pVTbl", ctypes.POINTER(VTblType))]

    # Store in descriptor
    descriptor.vtbl_type = VTblType
    descriptor.interface_type = InterfaceType
    descriptor.interface_ptr = interface_ptr


# =============================================================================
# Convenience function for IID comparison
# =============================================================================


def iid_of(interface_class: type) -> UGUID:
    """Get the IID of a decorated interface class.

    Args:
        interface_class: An interface class decorated with @interface.

    Returns:
        The interface identifier.

    Raises:
        AttributeError: If the class is not a decorated interface.

    Example:
        >>> from eco_python2acom.interfaces.decorators import interface, iid_of
        >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
        ... class IMyInterface: ...
        >>> iid = iid_of(IMyInterface)
    """
    return interface_class._iid_  # type: ignore[no-any-return]
