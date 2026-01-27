"""Base ACOM interfaces for EcoOS.

This module provides ctypes structures for the fundamental ACOM interfaces:
- IEcoUnknown: Base interface for all components
- IEcoComponentFactory: Factory for creating component instances
- IEcoInterfaceBus1: Interface bus for component registration/querying
- IEcoInterfaceBus1MemExt: Memory extension for the interface bus
- IEcoMemoryManager1: Memory management interface
"""

from __future__ import annotations

import ctypes
from typing import ClassVar, Optional

from eco_python2acom.core.guid import UGUID, UGUIDPtr
from eco_python2acom.core.types import FUNCTYPE, CharPtr, Int16, UInt32, VoidPtr, VoidPtrPtr

# =============================================================================
# Well-known GUIDs
# =============================================================================

# IID_IEcoUnknown = {00000000-0000-0000-0000-0000000000AA}
IID_IEcoUnknown = UGUID.from_string("00000000-0000-0000-0000-0000000000AA")

# IID_IEcoComponentFactory = {00000000-0000-0000-0000-000000000055}
IID_IEcoComponentFactory = UGUID.from_string("00000000-0000-0000-0000-000000000055")

# CID_EcoInterfaceBus1 = {00000000-0000-0000-0000-000042757331}
CID_EcoInterfaceBus1 = UGUID.from_string("00000000-0000-0000-0000-000042757331")

# IID_IEcoInterfaceBus1 = {00000000-0000-0000-0000-A00000000101}
IID_IEcoInterfaceBus1 = UGUID.from_string("00000000-0000-0000-0000-A00000000101")

# IID_IEcoInterfaceBus1MemExt = {00000000-0000-0000-0000-A00100000101}
IID_IEcoInterfaceBus1MemExt = UGUID.from_string("00000000-0000-0000-0000-A00100000101")

# CID_EcoMemoryManager1 = {00000000-0000-0000-0000-00004D656D31}
CID_EcoMemoryManager1 = UGUID.from_string("00000000-0000-0000-0000-00004D656D31")

# IID_IEcoMemoryManager1 = {00000000-0000-0000-0000-00014D656D31}
IID_IEcoMemoryManager1 = UGUID.from_string("00000000-0000-0000-0000-00014D656D31")

# GID_IEcoSystem_x86_64 = {00000000-0000-0000-0000-000086640300}
GID_IEcoSystem_x64 = UGUID.from_string("00000000-0000-0000-0000-000086640300")

# GID_IEcoSystem_x86_32 = {00000000-0000-0000-0000-000014C00200}
GID_IEcoSystem_x86 = UGUID.from_string("00000000-0000-0000-0000-000014C00200")


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


class IEcoInterfaceBus1(ctypes.Structure):
    """Interface bus for component registration and querying.

    Attributes:
        pVTbl: Pointer to the virtual table containing method implementations.

    Note:
        This corresponds to IEcoInterfaceBus1 in IdEcoInterfaceBus1.h.
    """

    pass  # Fields defined after VTbl


class IEcoInterfaceBus1MemExt(ctypes.Structure):
    """Memory extension interface for the interface bus.

    Attributes:
        pVTbl: Pointer to the virtual table containing method implementations.
    """

    pass  # Fields defined after VTbl


class IEcoMemoryManager1(ctypes.Structure):
    """Memory manager interface.

    Attributes:
        pVTbl: Pointer to the virtual table containing method implementations.
    """

    pass  # Fields defined after VTbl


# Pointer types
IEcoUnknownPtr = ctypes.POINTER(IEcoUnknown)
IEcoComponentFactoryPtr = ctypes.POINTER(IEcoComponentFactory)
IEcoInterfaceBus1Ptr = ctypes.POINTER(IEcoInterfaceBus1)
IEcoInterfaceBus1MemExtPtr = ctypes.POINTER(IEcoInterfaceBus1MemExt)
IEcoMemoryManager1Ptr = ctypes.POINTER(IEcoMemoryManager1)

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
    Int16, IEcoComponentFactoryPtr, IEcoUnknownPtr, IEcoUnknownPtr, UGUIDPtr, VoidPtrPtr
)

# int16_t Init(me, pISystem, pv)
InitFunc = FUNCTYPE(Int16, IEcoComponentFactoryPtr, IEcoUnknownPtr, VoidPtr)

# char_t* get_Name(me)
GetNameFunc = FUNCTYPE(CharPtr, IEcoComponentFactoryPtr)

# char_t* get_Version(me)
GetVersionFunc = FUNCTYPE(CharPtr, IEcoComponentFactoryPtr)

# char_t* get_Manufacturer(me)
GetManufacturerFunc = FUNCTYPE(CharPtr, IEcoComponentFactoryPtr)


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
# IEcoInterfaceBus1 Virtual Table
# =============================================================================


class IEcoInterfaceBus1VTbl(ctypes.Structure):
    """Virtual table for IEcoInterfaceBus1 interface.

    Provides methods for component registration and querying.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        # IEcoUnknown methods
        ("QueryInterface", FUNCTYPE(Int16, VoidPtr, UGUIDPtr, VoidPtrPtr)),
        ("AddRef", FUNCTYPE(UInt32, VoidPtr)),
        ("Release", FUNCTYPE(UInt32, VoidPtr)),
        # IEcoInterfaceBus1 methods
        ("Init", FUNCTYPE(Int16, VoidPtr)),
        ("InitWith", FUNCTYPE(Int16, VoidPtr, VoidPtr, UInt32)),
        ("RegisterComponent", FUNCTYPE(Int16, VoidPtr, UGUIDPtr, VoidPtr)),
        ("UnRegisterComponent", FUNCTYPE(Int16, VoidPtr, UGUIDPtr)),
        (
            "QueryComponent",
            FUNCTYPE(Int16, VoidPtr, UGUIDPtr, VoidPtr, UGUIDPtr, VoidPtrPtr),
        ),
    ]


# Now define IEcoInterfaceBus1 fields
IEcoInterfaceBus1._fields_ = [("pVTbl", ctypes.POINTER(IEcoInterfaceBus1VTbl))]


# =============================================================================
# IEcoInterfaceBus1MemExt Virtual Table
# =============================================================================


class IEcoInterfaceBus1MemExtVTbl(ctypes.Structure):
    """Virtual table for IEcoInterfaceBus1MemExt interface.

    Provides methods for memory manager configuration.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        # IEcoUnknown methods
        ("QueryInterface", FUNCTYPE(Int16, VoidPtr, UGUIDPtr, VoidPtrPtr)),
        ("AddRef", FUNCTYPE(UInt32, VoidPtr)),
        ("Release", FUNCTYPE(UInt32, VoidPtr)),
        # IEcoInterfaceBus1MemExt methods
        ("set_Manager", FUNCTYPE(Int16, VoidPtr, UGUIDPtr)),
        ("get_Manager", FUNCTYPE(UGUIDPtr, VoidPtr)),
        ("set_ExpandPool", FUNCTYPE(Int16, VoidPtr, Int16)),
    ]


# Now define IEcoInterfaceBus1MemExt fields
IEcoInterfaceBus1MemExt._fields_ = [("pVTbl", ctypes.POINTER(IEcoInterfaceBus1MemExtVTbl))]


# =============================================================================
# IEcoMemoryManager1 Virtual Table
# =============================================================================


class IEcoMemoryManager1VTbl(ctypes.Structure):
    """Virtual table for IEcoMemoryManager1 interface.

    Provides methods for memory initialization and status.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        # IEcoUnknown methods
        ("QueryInterface", FUNCTYPE(Int16, VoidPtr, UGUIDPtr, VoidPtrPtr)),
        ("AddRef", FUNCTYPE(UInt32, VoidPtr)),
        ("Release", FUNCTYPE(UInt32, VoidPtr)),
        # IEcoMemoryManager1 methods
        ("Init", FUNCTYPE(Int16, VoidPtr, VoidPtr, UInt32)),
        ("get_Status", FUNCTYPE(Int16, VoidPtr, VoidPtr)),
    ]


# Now define IEcoMemoryManager1 fields
IEcoMemoryManager1._fields_ = [("pVTbl", ctypes.POINTER(IEcoMemoryManager1VTbl))]


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
        base_methods = IEcoUnknownVTbl._fields_

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
