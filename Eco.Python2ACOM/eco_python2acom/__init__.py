"""Eco.Python2ACOM - Python to ACOM Bridge Library.

This library provides a bridge between Python and ACOM (Eco Component Object Model),
enabling Python applications to load and interact with EcoOS components.

Version: 0.2.0 - Core types + Interface decorators

Current features:
    - ctypes aliases for EcoOS C types (Int16, Int32, etc.)
    - UGUID structure for component/interface identification
    - Error handling with EcoError and EcoErrorCode
    - Interface decorators (@interface, @method) for declarative definitions
    - Base interfaces (IEcoUnknown, IEcoComponentFactory)

Planned features (next versions):
    - EcoSystem container (Unikernel Bridge)
    - ComponentWrapper for calling component methods
    - Logging support

Example (defining an interface):
    >>> from eco_python2acom import interface, method, Int16, Int32
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX:
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...

Notes:
    - This library is part of the EcoOS ecosystem.
    - For more information, see: https://docs.ecoos.dev/
"""

__version__ = "0.2.0"

# Core exports (types, errors, GUID)
from eco_python2acom.core import (
    FUNCTYPE,
    IS_64BIT,
    IS_LINUX,
    IS_MACOS,
    IS_WINDOWS,
    TYPE_SIZES,
    UGUID,
    Bool,
    Byte,
    Char,
    CharPtr,
    Double,
    DoublePtr,
    EcoError,
    EcoErrorCode,
    EcoResult,
    Float,
    FloatPtr,
    HResult,
    Int8,
    Int8Ptr,
    Int16,
    Int16Ptr,
    Int32,
    Int32Ptr,
    Int64,
    Int64Ptr,
    Long,
    LongLong,
    Ptr,
    SizeT,
    SSizeT,
    UChar,
    UInt8,
    UInt8Ptr,
    UInt16,
    UInt16Ptr,
    UInt32,
    UInt32Ptr,
    UInt64,
    UInt64Ptr,
    ULong,
    ULongLong,
    VoidPtr,
    VoidPtrPtr,
    WChar,
    WCharPtr,
    check_result,
    get_type_size,
    is_success,
)

# Interface exports
from eco_python2acom.interfaces import (
    IEcoComponentFactory,
    IEcoComponentFactoryVTbl,
    IEcoUnknown,
    IEcoUnknownVTbl,
    InterfaceDescriptor,
    MethodDescriptor,
    get_all_interfaces,
    get_interface,
    iid_of,
    interface,
    method,
)

__all__ = [
    # Version
    "__version__",
    # Platform detection
    "IS_WINDOWS",
    "IS_LINUX",
    "IS_MACOS",
    "IS_64BIT",
    "FUNCTYPE",
    # Boolean
    "Bool",
    # Byte types
    "Byte",
    "Int8",
    "UInt8",
    # Character types
    "Char",
    "UChar",
    "WChar",
    "CharPtr",
    "WCharPtr",
    # 16-bit integers
    "Int16",
    "UInt16",
    # 32-bit integers
    "Int32",
    "UInt32",
    # 64-bit integers
    "Int64",
    "UInt64",
    # Platform-dependent integers
    "Long",
    "ULong",
    "LongLong",
    "ULongLong",
    "SizeT",
    "SSizeT",
    # Floating-point
    "Float",
    "Double",
    # Pointers
    "VoidPtr",
    "Ptr",
    "VoidPtrPtr",
    # Pre-defined pointer types
    "Int8Ptr",
    "UInt8Ptr",
    "Int16Ptr",
    "UInt16Ptr",
    "Int32Ptr",
    "UInt32Ptr",
    "Int64Ptr",
    "UInt64Ptr",
    "FloatPtr",
    "DoublePtr",
    # EcoOS-specific
    "EcoResult",
    "HResult",
    # Type utilities
    "TYPE_SIZES",
    "get_type_size",
    # GUID
    "UGUID",
    # Errors
    "EcoError",
    "EcoErrorCode",
    "check_result",
    "is_success",
    # Base interfaces
    "IEcoUnknown",
    "IEcoUnknownVTbl",
    "IEcoComponentFactory",
    "IEcoComponentFactoryVTbl",
    # Interface decorators
    "interface",
    "method",
    "iid_of",
    # Interface descriptors
    "InterfaceDescriptor",
    "MethodDescriptor",
    # Interface registry
    "get_interface",
    "get_all_interfaces",
]
