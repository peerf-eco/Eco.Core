"""Base ACOM interfaces: IEcoUnknown and IEcoComponentFactory.

This module provides ctypes structures for the fundamental ACOM interfaces
that all components must implement.

Note:
    These interfaces correspond to IEcoBase1.h in the EcoOS framework.
    On Windows, methods use __stdcall calling convention (WINFUNCTYPE).
"""

from __future__ import annotations

import ctypes
from typing import ClassVar, Optional

from eco_python2acom.core.guid import UGUIDPtr
from eco_python2acom.core.types import FUNCTYPE, Int16, UInt32, VoidPtr, VoidPtrPtr

# =============================================================================
# Forward Declarations
# =============================================================================


class IEcoUnknown(ctypes.Structure):
    """Base interface for all ACOM components.

    All ACOM interfaces inherit from IEcoUnknown, which provides
    reference counting and interface querying capabilities.

    Attributes:
        pVTbl: Pointer to the virtual table containing method implementations.

    Note:
        This corresponds to IEcoUnknown in IEcoBase1.h.
    """

    pass  # Fields defined after VTbl


class IEcoComponentFactory(ctypes.Structure):
    """Factory interface for creating ACOM component instances.

    The component factory is obtained by calling GetIEcoComponentFactoryPtr()
    from a component DLL.

    Attributes:
        pVTbl: Pointer to the virtual table containing method implementations.

    Note:
        This corresponds to IEcoComponentFactory in IEcoBase1.h.
    """

    pass  # Fields defined after VTbl


# Pointer types
IEcoUnknownPtr = ctypes.POINTER(IEcoUnknown)
IEcoComponentFactoryPtr = ctypes.POINTER(IEcoComponentFactory)

# Double pointer for out parameters
IEcoUnknownPtrPtr = ctypes.POINTER(IEcoUnknownPtr)


# =============================================================================
# IEcoUnknown Virtual Table
# =============================================================================

# Method signatures for IEcoUnknown
# int16_t QueryInterface(IEcoUnknownPtr me, const UGUID* riid, voidptr_t* ppv)
QueryInterfaceFunc = FUNCTYPE(Int16, IEcoUnknownPtr, UGUIDPtr, VoidPtrPtr)

# uint32_t AddRef(IEcoUnknownPtr me)
AddRefFunc = FUNCTYPE(UInt32, IEcoUnknownPtr)

# uint32_t Release(IEcoUnknownPtr me)
ReleaseFunc = FUNCTYPE(UInt32, IEcoUnknownPtr)


class IEcoUnknownVTbl(ctypes.Structure):
    """Virtual table for IEcoUnknown interface.

    Contains function pointers for the three fundamental methods:
    QueryInterface, AddRef, and Release.

    Attributes:
        QueryInterface: Query for a specific interface by IID.
        AddRef: Increment the reference count.
        Release: Decrement the reference count.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        ("QueryInterface", QueryInterfaceFunc),
        ("AddRef", AddRefFunc),
        ("Release", ReleaseFunc),
    ]


# Now define IEcoUnknown fields
IEcoUnknown._fields_ = [("pVTbl", ctypes.POINTER(IEcoUnknownVTbl))]


# =============================================================================
# IEcoComponentFactory Virtual Table
# =============================================================================

# Method signatures for IEcoComponentFactory
# int16_t Alloc(me, pISystem, pIUnknownOuter, riid, ppv)
AllocFunc = FUNCTYPE(
    Int16,
    IEcoComponentFactoryPtr,  # me
    IEcoUnknownPtr,  # pISystem
    IEcoUnknownPtr,  # pIUnknownOuter
    UGUIDPtr,  # riid
    VoidPtrPtr,  # ppv (out)
)

# int16_t Init(me, pISystem, pv)
InitFunc = FUNCTYPE(
    Int16,
    IEcoComponentFactoryPtr,  # me
    IEcoUnknownPtr,  # pISystem
    VoidPtr,  # pv
)

# char_t* get_Name(me)
GetNameFunc = FUNCTYPE(ctypes.c_char_p, IEcoComponentFactoryPtr)

# char_t* get_Version(me)
GetVersionFunc = FUNCTYPE(ctypes.c_char_p, IEcoComponentFactoryPtr)

# char_t* get_Manufacturer(me)
GetManufacturerFunc = FUNCTYPE(ctypes.c_char_p, IEcoComponentFactoryPtr)


class IEcoComponentFactoryVTbl(ctypes.Structure):
    """Virtual table for IEcoComponentFactory interface.

    Contains function pointers for factory methods including
    IEcoUnknown methods and factory-specific methods.

    Attributes:
        QueryInterface: Query for a specific interface by IID (inherited).
        AddRef: Increment the reference count (inherited).
        Release: Decrement the reference count (inherited).
        Alloc: Allocate a new component instance.
        Init: Initialize a component instance.
        get_Name: Get the component name.
        get_Version: Get the component version.
        get_Manufacturer: Get the component manufacturer.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        # IEcoUnknown methods
        ("QueryInterface", FUNCTYPE(Int16, IEcoComponentFactoryPtr, UGUIDPtr, VoidPtrPtr)),
        ("AddRef", FUNCTYPE(UInt32, IEcoComponentFactoryPtr)),
        ("Release", FUNCTYPE(UInt32, IEcoComponentFactoryPtr)),
        # IEcoComponentFactory methods
        ("Alloc", AllocFunc),
        ("Init", InitFunc),
        ("get_Name", GetNameFunc),
        ("get_Version", GetVersionFunc),
        ("get_Manufacturer", GetManufacturerFunc),
    ]


# Now define IEcoComponentFactory fields
IEcoComponentFactory._fields_ = [("pVTbl", ctypes.POINTER(IEcoComponentFactoryVTbl))]


# =============================================================================
# Helper Functions
# =============================================================================


def create_vtbl_type(
    name: str,
    methods: list[tuple[str, type]],
    base_methods: Optional[list[tuple[str, type]]] = None,
) -> type[ctypes.Structure]:
    """Create a new VTbl structure type dynamically.

    This is useful for creating VTbl types for custom interfaces
    that inherit from IEcoUnknown.

    Args:
        name: The name of the VTbl structure.
        methods: List of (method_name, FUNCTYPE) tuples for interface methods.
        base_methods: List of base interface methods. Defaults to IEcoUnknown.

    Returns:
        A new ctypes Structure class for the VTbl.

    Example:
        >>> IEcoCalculatorXVTbl = create_vtbl_type(
        ...     "IEcoCalculatorXVTbl",
        ...     [
        ...         ("Addition", FUNCTYPE(Int32, IEcoCalculatorXPtr, Int16, Int16)),
        ...         ("Subtraction", FUNCTYPE(Int16, IEcoCalculatorXPtr, Int16, Int16)),
        ...     ]
        ... )
    """
    if base_methods is None:
        # Default IEcoUnknown methods - will be added by caller with correct pointer type
        base_methods = []

    all_methods = list(base_methods) + list(methods)

    class NewVTbl(ctypes.Structure):
        _fields_ = all_methods

    NewVTbl.__name__ = name
    NewVTbl.__qualname__ = name

    return NewVTbl


def create_interface_type(name: str, vtbl_type: type[ctypes.Structure]) -> type[ctypes.Structure]:
    """Create a new interface structure type dynamically.

    Args:
        name: The name of the interface structure.
        vtbl_type: The VTbl structure type for this interface.

    Returns:
        A new ctypes Structure class for the interface.

    Example:
        >>> IEcoCalculatorX = create_interface_type("IEcoCalculatorX", IEcoCalculatorXVTbl)
    """

    class NewInterface(ctypes.Structure):
        _fields_ = [("pVTbl", ctypes.POINTER(vtbl_type))]

    NewInterface.__name__ = name
    NewInterface.__qualname__ = name

    return NewInterface
