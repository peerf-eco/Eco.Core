"""Type aliases for ACOM/EcoOS types.

This module provides Python ctypes aliases that correspond to EcoOS C types
from the depend.h header file. These aliases simplify the declaration of
interface methods and ensure type compatibility when calling DLL functions.

Supported platforms:
    - Windows (x86_32, x86_64)

Type Mapping:
    EcoOS Type    | Python Alias | Size (bytes) | Description
    --------------|--------------|--------------|-------------
    bool_t        | Bool         | 1            | Boolean (0/1)
    byte_t        | Byte         | 1            | Unsigned byte
    char_t        | Char         | 1            | Signed character
    uchar_t       | UChar        | 1            | Unsigned character
    int8_t        | Int8         | 1            | Signed 8-bit integer
    uint8_t       | UInt8        | 1            | Unsigned 8-bit integer
    int16_t       | Int16        | 2            | Signed 16-bit integer
    uint16_t      | UInt16       | 2            | Unsigned 16-bit integer
    int32_t       | Int32        | 4            | Signed 32-bit integer
    uint32_t      | UInt32       | 4            | Unsigned 32-bit integer
    int64_t       | Int64        | 8            | Signed 64-bit integer
    uint64_t      | UInt64       | 8            | Unsigned 64-bit integer
    float_t       | Float        | 4            | Single-precision float
    double_t      | Double       | 8            | Double-precision float
    voidptr_t     | VoidPtr      | 4/8          | Void pointer (platform-dependent)

Note:
    On Windows, ECOCALLMETHOD is __stdcall, which requires WINFUNCTYPE.
"""

from __future__ import annotations

from ctypes import (
    POINTER,
    WINFUNCTYPE,
    Structure,
    WinDLL,
    _Pointer,
    byref,
    c_char,
    c_char_p,
    c_double,
    c_float,
    c_int8,
    c_int16,
    c_int32,
    c_int64,
    c_long,
    c_longlong,
    c_size_t,
    c_ssize_t,
    c_uint8,
    c_uint16,
    c_uint32,
    c_uint64,
    c_ulong,
    c_ulonglong,
    c_void_p,
    c_wchar,
    c_wchar_p,
    cast,
)
from typing import TYPE_CHECKING, Any, TypeAlias, TypeVar

# TypeVar for generic pointer operations
T = TypeVar("T")

# =============================================================================
# Calling Convention
# =============================================================================

FUNCTYPE = WINFUNCTYPE
DLL = WinDLL

# =============================================================================
# Boolean Type
# =============================================================================

# bool_t = unsigned char (1 byte) in EcoOS
Bool: TypeAlias = c_uint8

# =============================================================================
# Byte Types (8-bit)
# =============================================================================

Byte: TypeAlias = c_uint8
Int8: TypeAlias = c_int8
UInt8: TypeAlias = c_uint8

# =============================================================================
# Character Types
# =============================================================================

Char: TypeAlias = c_char
UChar: TypeAlias = c_uint8
WChar: TypeAlias = c_wchar

# =============================================================================
# Integer Types (16/32/64-bit)
# =============================================================================

Int16: TypeAlias = c_int16
UInt16: TypeAlias = c_uint16

Int32: TypeAlias = c_int32
UInt32: TypeAlias = c_uint32

Int64: TypeAlias = c_int64
UInt64: TypeAlias = c_uint64

# =============================================================================
# Platform-Dependent Integer Types
# =============================================================================

Long: TypeAlias = c_long
ULong: TypeAlias = c_ulong
LongLong: TypeAlias = c_longlong
ULongLong: TypeAlias = c_ulonglong
SizeT: TypeAlias = c_size_t
SSizeT: TypeAlias = c_ssize_t

# =============================================================================
# Floating-Point Types
# =============================================================================

Float: TypeAlias = c_float
Double: TypeAlias = c_double

# =============================================================================
# Pointer Types
# =============================================================================

ByRefArg: TypeAlias = Any
Void: TypeAlias = None
VoidPtr: TypeAlias = c_void_p
CharPtr: TypeAlias = c_char_p
WCharPtr: TypeAlias = c_wchar_p

# =============================================================================
# Structure Base Class
# =============================================================================

EcoResult: TypeAlias = int


class EcoStructure(Structure):
    """Base class for EcoOS structures."""

    pass


# =============================================================================
# Pointer & Cast Type Factory
# =============================================================================


def Ptr(ctype: type[T]) -> type[POINTER[T]]:
    """Create a pointer type for the given ctypes type.

    Args:
        ctype: ctypes structure or primitive type.

    Returns:
        ctypes POINTER type for ctype.
    """
    return POINTER(ctype)


def CastPtr(ptr: VoidPtr, ptr_to: type[POINTER[T]]) -> POINTER[T]:
    """Cast a void pointer to a ctypes pointer type.

    Args:
        ptr: VoidPtr to cast.
        ptr_to: ctypes POINTER(...) type to cast to.

    Returns:
        An instance of ptr_to pointing to the same address as ptr.
    """
    return cast(ptr, ptr_to)


def ByRef(obj: T) -> ByRefArg:
    """Get a by-reference pointer for a ctypes data object

    Args:
        obj: CData object to get a by-reference pointer for.

    Returns:
        A by-reference pointer to obj.
    """
    return byref(obj)  # type: ignore


# =============================================================================
# Common Pointer Types (Pre-defined for convenience)
# =============================================================================

if TYPE_CHECKING:
    Int8Ptr = _Pointer[Int8]
    UInt8Ptr = _Pointer[UInt8]
    Int16Ptr = _Pointer[Int16]
    UInt16Ptr = _Pointer[UInt16]
    Int32Ptr = _Pointer[Int32]
    UInt32Ptr = _Pointer[UInt32]
    Int64Ptr = _Pointer[Int64]
    UInt64Ptr = _Pointer[UInt64]
    FloatPtr = _Pointer[Float]
    DoublePtr = _Pointer[Double]
    VoidPtrPtr = _Pointer[VoidPtr]
else:
    Int8Ptr = Ptr(Int8)
    UInt8Ptr = Ptr(UInt8)
    Int16Ptr = Ptr(Int16)
    UInt16Ptr = Ptr(UInt16)
    Int32Ptr = Ptr(Int32)
    UInt32Ptr = Ptr(UInt32)
    Int64Ptr = Ptr(Int64)
    UInt64Ptr = Ptr(UInt64)
    FloatPtr = Ptr(Float)
    DoublePtr = Ptr(Double)
    VoidPtrPtr = Ptr(VoidPtr)
