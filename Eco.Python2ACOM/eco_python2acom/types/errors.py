"""Error codes and exceptions for ACOM/EcoOS.

This module provides error code constants matching 'ErrEcoCodes.h'
and a custom exception class for ACOM operations.

Note:
    Error codes are based on the EcoOS 'ErrEcoCodes.h' header file.
    Values are 16-bit integers (int16_t).
"""

from enum import IntEnum
from typing import Optional, Union

from eco_python2acom.types.core import Int16


class EcoErrorCode(IntEnum):
    """ACOM/EcoOS error codes.

    These codes correspond to the definitions in 'ErrEcoCodes.h'.
    Success is indicated by 0x0000, errors by other values.
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
}


class EcoError(Exception):
    """Exception raised for ACOM/EcoOS errors."""

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
                self.code = EcoErrorCode(code.value)
            else:
                self.code = EcoErrorCode(code)
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

    def __repr__(self) -> str:
        """Return string representation of the EcoError exception."""
        return f"EcoError(code={self.code}, message={self.message}, operation={self.operation})"

    def __eq__(self, other: object) -> bool:
        """Compare two EcoError exceptions.

        Comparison is done on all three attributes: code, message, and operation.
        """
        if not isinstance(other, EcoError):
            return NotImplemented
        return (
            self.code == other.code
            and self.message == other.message
            and self.operation == other.operation
        )

    def __hash__(self) -> int:
        """Hash the EcoError exception."""
        return hash((self.code, self.message, self.operation))


__all__ = ["EcoErrorCode", "EcoError", "ERROR_MESSAGES"]
