"""Core module for eco_python2acom.

This module provides fundamental types, GUID handling, and error codes
for working with ACOM (Eco Component Object Model) components.

Submodules:
    - types: ctypes aliases for EcoOS C types
    - guid: UGUID structure and utilities
    - errors: EcoError exception and error codes

Example:
    >>> from eco_python2acom.core import Int16, Int32, UGUID, EcoError
    >>> from eco_python2acom.core import IS_WINDOWS, IS_64BIT
"""

from eco_python2acom.core.errors import EcoError, EcoErrorCode, check_result, is_success
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import (
    FUNCTYPE,
    IS_64BIT,
    IS_LINUX,
    IS_MACOS,
    IS_WINDOWS,
    TYPE_SIZES,
    Bool,
    Byte,
    Char,
    CharPtr,
    Double,
    DoublePtr,
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
    get_type_size,
)

__all__ = [
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
]
