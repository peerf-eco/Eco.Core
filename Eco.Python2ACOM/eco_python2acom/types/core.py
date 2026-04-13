"""Core types for EcoOS/ACOM interoperability.

This module provides core primitive types for working with EcoOS C APIs.

Platform Support:
    - Windows (x86, x64) with __stdcall convention

Type Mapping:

    | EcoOS    | Python  | Bytes | Description                       |
    |----------|---------|-------|-----------------------------------|
    | bool_t   | Bool    | 1     | Boolean (0=false, non-zero=true)  |
    | byte_t   | Byte    | 1     | Unsigned 8-bit integer            |
    | char_t   | Char    | 1     | Single-byte character (ASCII)     |
    | int8_t   | Int8    | 1     | Signed 8-bit integer              |
    | uint8_t  | UInt8   | 1     | Unsigned 8-bit integer            |
    | int16_t  | Int16   | 2     | Signed 16-bit integer             |
    | uint16_t | UInt16  | 2     | Unsigned 16-bit integer           |
    | int32_t  | Int32   | 4     | Signed 32-bit integer             |
    | uint32_t | UInt32  | 4     | Unsigned 32-bit integer           |
    | int64_t  | Int64   | 8     | Signed 64-bit integer             |
    | uint64_t | UInt64  | 8     | Unsigned 64-bit integer           |
    | float_t  | Float   | 4     | Single-precision floating-point   |
    | double_t | Double  | 8     | Double-precision floating-point   |
"""

from __future__ import annotations

from ctypes import WINFUNCTYPE
from ctypes import Array as CArray
from ctypes import Structure, Union, WinDLL
from ctypes import _CFuncPtr as CFuncPtr
from ctypes import _Pointer as CPointer
from ctypes import _SimpleCData as CSimpleData
from ctypes import (
    c_char,
    c_char_p,
    c_double,
    c_float,
    c_int,
    c_int8,
    c_int16,
    c_int32,
    c_int64,
    c_long,
    c_longlong,
    c_short,
    c_size_t,
    c_ssize_t,
    c_ubyte,
    c_uint,
    c_uint8,
    c_uint16,
    c_uint32,
    c_uint64,
    c_ulong,
    c_ulonglong,
    c_ushort,
    c_void_p,
    c_wchar,
    c_wchar_p,
)
from typing import TYPE_CHECKING, TypeAlias

# =============================================================================
# Calling Convention
# =============================================================================

CFuncType = WINFUNCTYPE
"""Function type for __stdcall convention (Windows).

Use this to define function signatures for EcoOS interface methods.
"""

CDLL: TypeAlias = WinDLL
"""DLL loader with __stdcall convention.

Use this to load EcoOS DLLs on Windows.
"""

# =============================================================================
# Boolean Type
# =============================================================================

Bool: TypeAlias = c_uint8
"""Boolean type (bool_t in EcoOS).

In EcoOS, booleans are 8-bit unsigned integers where 0 is false and
any non-zero value is true.
"""

# =============================================================================
# Integer Types - 8-bit
# =============================================================================

Byte: TypeAlias = c_uint8
"""Unsigned 8-bit integer (byte_t in EcoOS)."""

Int8: TypeAlias = c_int8
"""Signed 8-bit integer (int8_t in EcoOS).

Range: -128 to 127
"""

UInt8: TypeAlias = c_uint8
"""Unsigned 8-bit integer (uint8_t in EcoOS).

Range: 0 to 255
"""

UByte: TypeAlias = c_ubyte
"""Alternative unsigned 8-bit type."""

# =============================================================================
# Character Types
# =============================================================================

Char: TypeAlias = c_char
"""Single-byte character (char_t in EcoOS).

Used for ASCII/ANSI text. For wide characters, use WChar.
"""

UChar: TypeAlias = c_uint8
"""Unsigned character (uchar_t in EcoOS)."""

WChar: TypeAlias = c_wchar
"""Wide character (wchar_t in C).

Used for Unicode text (UTF-16 on Windows).
"""

# =============================================================================
# Integer Types - 16-bit
# =============================================================================

Short: TypeAlias = c_short
"""Short integer (typically 16-bit)."""

UShort: TypeAlias = c_ushort
"""Unsigned short integer (typically 16-bit)."""

Int16: TypeAlias = c_int16
"""Signed 16-bit integer (int16_t in EcoOS).

Range: -32,768 to 32,767
"""

UInt16: TypeAlias = c_uint16
"""Unsigned 16-bit integer (uint16_t in EcoOS).

Range: 0 to 65,535
"""

# =============================================================================
# Integer Types - 32-bit
# =============================================================================

Int: TypeAlias = c_int
"""Platform-dependent signed integer (typically 32-bit)."""

UInt: TypeAlias = c_uint
"""Platform-dependent unsigned integer (typically 32-bit)."""

Int32: TypeAlias = c_int32
"""Signed 32-bit integer (int32_t in EcoOS).

Range: -2,147,483,648 to 2,147,483,647
"""

UInt32: TypeAlias = c_uint32
"""Unsigned 32-bit integer (uint32_t in EcoOS).

Range: 0 to 4,294,967,295
"""

# =============================================================================
# Integer Types - 64-bit
# =============================================================================

Int64: TypeAlias = c_int64
"""Signed 64-bit integer (int64_t in EcoOS).

Range: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
"""

UInt64: TypeAlias = c_uint64
"""Unsigned 64-bit integer (uint64_t in EcoOS).

Range: 0 to 18,446,744,073,709,551,615
"""

# =============================================================================
# Platform-Dependent Integer Types
# =============================================================================

Long: TypeAlias = c_long
"""Platform-dependent long integer.

32-bit on Windows, 64-bit on Linux x64.
"""

ULong: TypeAlias = c_ulong
"""Platform-dependent unsigned long integer."""

LongLong: TypeAlias = c_longlong
"""64-bit long long integer."""

ULongLong: TypeAlias = c_ulonglong
"""64-bit unsigned long long integer."""

SizeT: TypeAlias = c_size_t
"""size_t - unsigned integer for sizes (platform word size)."""

SSizeT: TypeAlias = c_ssize_t
"""ssize_t - signed integer for sizes (platform word size)."""

# =============================================================================
# Floating-Point Types
# =============================================================================

Float: TypeAlias = c_float
"""Single-precision floating-point (float_t in EcoOS).

32-bit IEEE 754 floating-point number.
"""

Double: TypeAlias = c_double
"""Double-precision floating-point (double_t in EcoOS).

64-bit IEEE 754 floating-point number.
"""

# =============================================================================
# Result/Error Codes
# =============================================================================

EcoResult: TypeAlias = Int16
"""EcoOS result code (16-bit signed integer).

Used for function return values indicating success or error.
Typically 0 indicates success, negative values indicate errors.
"""

# =============================================================================
# Void and Pointer Types
# =============================================================================

if TYPE_CHECKING:

    class Void:
        """Void type (equivalent to C void).

        Used in type annotations for functions that return nothing.
        """

        pass

else:
    Void: TypeAlias = type(None)

VoidPtr: TypeAlias = c_void_p
"""Void pointer (voidptr_t in EcoOS).

Generic pointer that can point to any type. Use Ptr[T] for type-safe pointers.
"""

# =============================================================================
# String Pointer Types
# =============================================================================

CString: TypeAlias = c_char_p
"""Pointer to null-terminated ANSI/ASCII string (char* in C).

Used for ANSI/ASCII C strings.
"""

WString: TypeAlias = c_wchar_p
"""Pointer to null-terminated wide string (wchar_t* in C).

Used for Unicode strings (UTF-16 on Windows).
"""

# =============================================================================
# Structure and Union Base Classes
# =============================================================================

CStructure: TypeAlias = Structure
"""Base class for EcoOS structures.

Inherit from this to define C-compatible structures.
"""

CUnion: TypeAlias = Union
"""Base class for EcoOS unions.

Inherit from this to define C-compatible unions.
"""

CData: TypeAlias = CSimpleData | CPointer | CUnion | CArray | CStructure | CFuncPtr
"""Base instance type for EcoOS data types.

This type is used to represent any EcoOS data type.
"""

CLayout: TypeAlias = CStructure | CUnion
"""Base type for layout-compatible types.

This includes structures and unions, which have a defined memory layout.
"""

# Canonical EcoOS names
TYPE_NAMES: dict[type, str] = {
    Int8: "Int8",
    UInt8: "UInt8",
    Int16: "Int16",
    UInt16: "UInt16",
    Int32: "Int32",
    UInt32: "UInt32",
    Int64: "Int64",
    UInt64: "UInt64",
    Float: "Float",
    Double: "Double",
    Char: "Char",
    WChar: "WChar",
    CString: "CString",
    WString: "WString",
    Void: "Void",
}

# =============================================================================
# Public API
# =============================================================================

__all__ = [
    # Calling convention
    "CFuncType",
    "CDLL",
    # Boolean
    "Bool",
    # 8-bit integers
    "Byte",
    "Int8",
    "UInt8",
    "UByte",
    # Characters
    "Char",
    "UChar",
    "WChar",
    # 16-bit integers
    "Short",
    "UShort",
    "Int16",
    "UInt16",
    # 32-bit integers
    "Int",
    "UInt",
    "Int32",
    "UInt32",
    # 64-bit integers
    "Int64",
    "UInt64",
    "Long",
    "ULong",
    "LongLong",
    "ULongLong",
    # Size types
    "SizeT",
    "SSizeT",
    # Floating-point
    "Float",
    "Double",
    # Result codes
    "EcoResult",
    # Void and pointers
    "Void",
    "VoidPtr",
    # String pointers
    "CString",
    "WString",
    # Base classes
    "CArray",
    "CStructure",
    "CUnion",
    "CFuncPtr",
    "CPointer",
    "CSimpleData",
    "CData",
    "CLayout",
]
