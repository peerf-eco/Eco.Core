"""Error codes and exceptions for ACOM/EcoOS.

This module provides error code constants matching `ErrEcoCodes.h`
and a custom exception class for ACOM operations.

Note:
    Error codes are based on the EcoOS `ErrEcoCodes.h` header file.
    Values are 16-bit integers.
"""

from enum import IntEnum
from typing import Optional, Union

from eco_python2acom.types.core import Int16


class EcoErrorCode(IntEnum):
    """ACOM/EcoOS error codes from `ErrEcoCodes.h`.

    All ACOM interface methods return a 16-bit status code. Zero means success -
    any other value signals an error. The constants cover all codes defined
    in the standard EcoOS header.

    Example:
        ```python
        result = bus.obj.RegisterComponent(byref(cid), factory)
        if result.value != EcoErrorCode.SUCCESS:
            raise EcoError(result, "RegisterComponent failed")
        ```

    Note:
        `OK` is an alias for `SUCCESS` (both equal `0x0000`).
        User-defined codes start at `USER = 0x0002`.
    """

    # Success
    SUCCESS = 0x0000
    OK = 0x0000

    # User-defined error codes start
    USER = 0x0002

    # Failure codes (descending from 0xFFFF)
    UNEXPECTED = 0xFFFF  # Catastrophic failure
    POINTER = 0xFFEE  # Invalid pointer value
    NOINTERFACE = 0xFFED  # Interface not supported
    NOSYSTEM = 0xFFEC  # System interface not supported
    NOBUS = 0xFFEB  # Interface bus not supported
    NOAGGREGATION = 0xFFEA  # Aggregation not supported
    COMPONENT_NOTFOUND = 0xFFE9  # Component not found
    OUTOFMEMORY = 0xFFE8  # Memory allocation failure
    MEMMGR_NOTREG = 0xFFE7  # Memory manager not registered
    GET_MEMORY_ALLOCATOR = 0xFFE6  # Failed to get memory allocator
    FAIL = 0xFFE5  # Unspecified error
    NOTIMPL = 0xFFE4  # Not implemented
    INVALIDARG = 0xFFE3  # Invalid argument(s)
    FILEMGR_NOTREG = 0xFFE2  # File manager not registered
    NETMGR_NOTREG = 0xFFE1  # Network manager not registered
    OUTINTERFACE_NOCONNECTION = 0xFFE0  # Outgoing interface not connected
    NOPYTHONBRIDGE = 0xFFDF  # Python bridge component is unavailable


# Human-readable error messages
ERROR_MESSAGES: dict[EcoErrorCode, str] = {
    EcoErrorCode.SUCCESS: "Operation completed successfully",
    EcoErrorCode.UNEXPECTED: "Unexpected condition, catastrophic failure",
    EcoErrorCode.POINTER: "Invalid pointer value was passed",
    EcoErrorCode.NOINTERFACE: "Interface is not supported",
    EcoErrorCode.NOSYSTEM: "System interface is not supported",
    EcoErrorCode.NOBUS: "Interface bus is not supported",
    EcoErrorCode.NOAGGREGATION: "Aggregation is not supported",
    EcoErrorCode.COMPONENT_NOTFOUND: "Component not found",
    EcoErrorCode.OUTOFMEMORY: "Memory allocation failure",
    EcoErrorCode.MEMMGR_NOTREG: "Memory manager component is not registered",
    EcoErrorCode.GET_MEMORY_ALLOCATOR: "Failed to get memory allocator",
    EcoErrorCode.FAIL: "Unspecified error",
    EcoErrorCode.NOTIMPL: "Not implemented",
    EcoErrorCode.INVALIDARG: "One or more arguments are invalid",
    EcoErrorCode.FILEMGR_NOTREG: "File system manager is not registered",
    EcoErrorCode.NETMGR_NOTREG: "Network manager is not registered",
    EcoErrorCode.OUTINTERFACE_NOCONNECTION: "Outgoing interface has no connection",
    EcoErrorCode.NOPYTHONBRIDGE: "Python bridge component is unavailable",
}


class EcoError(Exception):
    """Exception raised when an ACOM/EcoOS operation returns a non-zero status code.

    Wraps a raw `Int16` (or plain `int` / `EcoErrorCode`) returned by any vtable
    method into a structured Python exception with a human-readable message.

    Attributes:
        code: Normalised `EcoErrorCode` value.
        message: Human-readable description of the failure.
        operation: Name of the ACOM operation that failed, if provided.

    Example:
        ```python
        result = factory.obj.Alloc(None, None, byref(iid), byref(ppv))
        if result.value != 0:
            raise EcoError(result, "Failed to create component instance")
        ```

    Note:
        Unknown error codes (not in `EcoErrorCode`) are normalised to
        `EcoErrorCode.FAIL` rather than raising a secondary exception.
    """

    def __init__(
        self,
        code: Union[Int16, int, EcoErrorCode],
        message: Optional[str] = None,
        operation: Optional[str] = None,
    ) -> None:
        """Initialize the EcoError exception.

        Args:
            code: The error code returned by an ACOM operation.
            message: Additional context about the error.
            operation: The name of the operation that failed.
        """
        try:
            if isinstance(code, EcoErrorCode):
                self.code = code
            elif isinstance(code, Int16):
                self.code = EcoErrorCode(code.value & 0xFFFF)
            else:
                self.code = EcoErrorCode(int(code) & 0xFFFF)
        except ValueError:
            # Unknown error code, default to FAIL
            self.code = EcoErrorCode.FAIL

        self.operation = operation

        # Build error message
        default_msg = ERROR_MESSAGES.get(self.code, "Unknown error")
        self.message = message if message else default_msg

        # Format full message
        full_msg = f"[0x{self.code:04X}] {self.message}"
        if operation:
            full_msg += f" (during {operation})"

        super().__init__(full_msg)


__all__ = ["EcoErrorCode", "EcoError"]
