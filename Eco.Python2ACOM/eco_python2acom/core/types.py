"""Type aliases for ACOM/EcoOS types.

This module provides Python ctypes aliases that correspond to EcoOS C types
from the depend.h header file. These aliases simplify the declaration of
interface methods and ensure type compatibility when calling DLL functions.

Supported platforms:
    - Windows (x86, x64)
    - Linux (x86, x64)

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
    On Linux/macOS, it uses the default cdecl calling convention (CFUNCTYPE).
"""

from __future__ import annotations

import ctypes
import sys
from ctypes import (
    CFUNCTYPE,
    POINTER,
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
)
from typing import TYPE_CHECKING, TypeAlias

# =============================================================================
# Platform Detection
# =============================================================================

IS_WINDOWS: bool = sys.platform == "win32"
IS_LINUX: bool = sys.platform.startswith("linux")
IS_MACOS: bool = sys.platform == "darwin"
IS_64BIT: bool = sys.maxsize > 2**32

# =============================================================================
# Calling Convention
# =============================================================================

# On Windows, ACOM uses __stdcall (ECOCALLMETHOD)
# On other platforms, use default cdecl calling convention
if IS_WINDOWS:
    FUNCTYPE = ctypes.WINFUNCTYPE
else:
    FUNCTYPE = CFUNCTYPE

# =============================================================================
# Boolean Type
# =============================================================================

# bool_t = unsigned char (1 byte) in EcoOS
# Note: Using c_uint8 instead of c_bool for binary compatibility
Bool: TypeAlias = c_uint8

# =============================================================================
# Byte Types (8-bit)
# =============================================================================

# byte_t = unsigned char (1 byte)
Byte: TypeAlias = c_uint8

# int8_t = signed char (1 byte)
Int8: TypeAlias = c_int8

# uint8_t = unsigned char (1 byte)
UInt8: TypeAlias = c_uint8

# =============================================================================
# Character Types
# =============================================================================

# char_t = char (1 byte, signed on most platforms)
Char: TypeAlias = c_char

# uchar_t = unsigned char (1 byte)
UChar: TypeAlias = c_uint8

# wchar_t = wide character (platform-dependent: 2 bytes on Windows, 4 on Linux)
WChar: TypeAlias = c_wchar

# =============================================================================
# 16-bit Integer Types
# =============================================================================

# int16_t = short (2 bytes)
Int16: TypeAlias = c_int16

# uint16_t = unsigned short (2 bytes)
UInt16: TypeAlias = c_uint16

# =============================================================================
# 32-bit Integer Types
# =============================================================================

# int32_t = int (4 bytes)
Int32: TypeAlias = c_int32

# uint32_t = unsigned int (4 bytes)
UInt32: TypeAlias = c_uint32

# =============================================================================
# 64-bit Integer Types
# =============================================================================

# int64_t = long long int (8 bytes)
Int64: TypeAlias = c_int64

# uint64_t = unsigned long long int (8 bytes)
UInt64: TypeAlias = c_uint64

# =============================================================================
# Platform-Dependent Integer Types
# =============================================================================

# long_t = long (4 bytes on Windows, 4/8 bytes on Linux depending on arch)
Long: TypeAlias = c_long

# ulong_t = unsigned long
ULong: TypeAlias = c_ulong

# long long (always 8 bytes)
LongLong: TypeAlias = c_longlong

# unsigned long long (always 8 bytes)
ULongLong: TypeAlias = c_ulonglong

# size_t = unsigned size type (platform-dependent: 4 or 8 bytes)
SizeT: TypeAlias = c_size_t

# ssize_t = signed size type (platform-dependent: 4 or 8 bytes)
SSizeT: TypeAlias = c_ssize_t

# =============================================================================
# Floating-Point Types
# =============================================================================

# float_t = float (4 bytes, IEEE 754 single-precision)
Float: TypeAlias = c_float

# double_t = double (8 bytes, IEEE 754 double-precision)
Double: TypeAlias = c_double

# =============================================================================
# Pointer Types
# =============================================================================

# voidptr_t = void* (4 bytes on x86, 8 bytes on x64)
VoidPtr: TypeAlias = c_void_p

# char* = pointer to null-terminated string (ANSI)
CharPtr: TypeAlias = c_char_p

# wchar_t* = pointer to null-terminated wide string (Unicode)
WCharPtr: TypeAlias = c_wchar_p

# =============================================================================
# Pointer Type Factory
# =============================================================================


def Ptr(ctype: type) -> type:
    """Create a pointer type for the given ctypes type.

    Args:
        ctype: The ctypes type to create a pointer for.

    Returns:
        A ctypes POINTER type.

    Example:
        >>> Int32Ptr = Ptr(Int32)
        >>> my_array = (Int32 * 10)()
        >>> ptr = Ptr(Int32)(my_array)
    """
    return POINTER(ctype)


# =============================================================================
# Common Pointer Types (Pre-defined for convenience)
# =============================================================================

# Pointer to basic types
Int8Ptr = POINTER(c_int8)
UInt8Ptr = POINTER(c_uint8)
Int16Ptr = POINTER(c_int16)
UInt16Ptr = POINTER(c_uint16)
Int32Ptr = POINTER(c_int32)
UInt32Ptr = POINTER(c_uint32)
Int64Ptr = POINTER(c_int64)
UInt64Ptr = POINTER(c_uint64)
FloatPtr = POINTER(c_float)
DoublePtr = POINTER(c_double)

# Pointer to void pointer (double pointer)
VoidPtrPtr = POINTER(c_void_p)

# =============================================================================
# EcoOS-specific Result Type
# =============================================================================

# EcoOS functions return int16_t for result codes
EcoResult: TypeAlias = c_int16

# =============================================================================
# Type Information (for runtime introspection)
# =============================================================================

if TYPE_CHECKING:
    # Type hints for static analysis
    pass

# Type size information (in bytes)
TYPE_SIZES: dict[str, int] = {
    "Bool": 1,
    "Byte": 1,
    "Int8": 1,
    "UInt8": 1,
    "Char": 1,
    "UChar": 1,
    "Int16": 2,
    "UInt16": 2,
    "Int32": 4,
    "UInt32": 4,
    "Int64": 8,
    "UInt64": 8,
    "Float": 4,
    "Double": 8,
    "VoidPtr": 8 if IS_64BIT else 4,
    "SizeT": 8 if IS_64BIT else 4,
}


def get_type_size(type_name: str) -> int:
    """Get the size of a type in bytes.

    Args:
        type_name: Name of the type (e.g., "Int32", "VoidPtr").

    Returns:
        Size in bytes.

    Raises:
        KeyError: If type_name is not found.
    """
    return TYPE_SIZES[type_name]
