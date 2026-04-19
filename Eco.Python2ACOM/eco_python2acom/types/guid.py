"""UGUID (Universal GUID) implementation for ACOM/EcoOS.

This module provides the UGUID class for working with EcoOS component
and interface identifiers. UGUID is an 18-byte structure consisting of:
- Preamble (1 byte): Version/format indicator
- Length (1 byte): Length of the data portion
- Data (16 bytes): The actual GUID data

Note:
    The UGUID structure is defined in 'IEcoBase1.h'.
    Standard format: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
"""

from __future__ import annotations

import re
from typing import ClassVar, Optional

from eco_python2acom.decorators.model import model
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import UInt8

__all__ = ["UGUID"]


@model
class UGUID:
    """ACOM Universal GUID structure.

    An 18-byte structure used for component and interface identification
    in the EcoOS/ACOM system.
    """

    Preamble: UInt8
    Length: UInt8
    Data: Array[UInt8, 0x10]

    # Regex pattern for GUID string validation
    GUID_PATTERN: ClassVar[re.Pattern[str]] = re.compile(
        r"^[{(]?"
        r"([0-9A-Fa-f]{8})-?"
        r"([0-9A-Fa-f]{4})-?"
        r"([0-9A-Fa-f]{4})-?"
        r"([0-9A-Fa-f]{4})-?"
        r"([0-9A-Fa-f]{12})"
        r"[})]?$"
    )

    def __init__(
        self,
        guid_string: Optional[str] = None,
        data: Optional[bytes] = None,
        preamble: int = 0x01,
        length: int = 0x10,
    ) -> None:
        """Initialize UGUID from a string or raw bytes.

        Args:
            guid_string: Optional GUID string in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".
            data: Optional 16-byte raw data for the GUID.
            preamble: Preamble byte (default 0x01).
            length: Length byte (default 0x10).

        Raises:
            ValueError: If both guid_string and data are provided, or if the
                provided data is not 16 bytes long.
        """
        self.Preamble = UInt8(preamble)
        self.Length = UInt8(length)

        if guid_string is not None and data is not None:
            raise ValueError("Provide either 'guid_string' or 'data', not both.")

        if guid_string is not None:
            self._from_string(guid_string)
        elif data is not None:
            self._from_bytes(data)

    def _from_string(self, guid_string: str) -> None:
        """Parse a GUID string into the Data field.

        Args:
            guid_string: GUID in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".

        Raises:
            ValueError: If the string format is invalid.
        """
        match = self.GUID_PATTERN.match(guid_string)
        if not match:
            raise ValueError(
                f"Invalid GUID format: {guid_string!r}. "
                "Expected format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
            )

        hex_str = "".join(match.groups())
        self.Data[:] = [UInt8(b) for b in bytes.fromhex(hex_str)]

    def _from_bytes(self, data: bytes) -> None:
        """Initialize from raw bytes.

        Args:
            data: 16-byte GUID data.

        Raises:
            ValueError: If data length is not 0x10 (16 bytes).
        """
        if len(data) != 0x10:
            raise ValueError(f"GUID data must be 16 bytes, got {len(data)}")

        self.Data[:] = [UInt8(b) for b in data]

    def to_bytes(self) -> bytes:
        """Convert the 'Data' field to bytes.

        Returns:
            The 16-byte GUID data.
        """
        return bytes(self.Data)

    def to_string(self, with_hyphens: bool = True) -> str:
        """Convert to standard GUID string format.

        Args:
            with_hyphens: If True, add hyphens between the parts of the GUID.

        Returns:
            GUID string in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
            or "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" if with_hyphens is False.
        """
        data = self.to_bytes()
        if with_hyphens:
            return (
                f"{data[0:4].hex().upper()}-"
                f"{data[4:6].hex().upper()}-"
                f"{data[6:8].hex().upper()}-"
                f"{data[8:10].hex().upper()}-"
                f"{data[10:16].hex().upper()}"
            )
        return data.hex().upper()

    def __eq__(self, other: object) -> bool:
        """Compare two UGUIDs for equality.

        Comparison is done on all 18 bytes (preamble, length, data).
        """
        if not isinstance(other, UGUID):
            return NotImplemented
        return (
            self.Preamble == other.Preamble
            and self.Length == other.Length
            and self.to_bytes() == other.to_bytes()
        )

    def __hash__(self) -> int:
        """Return hash of the UGUID."""
        return hash((self.Preamble, self.Length, self.to_bytes()))

    def __str__(self) -> str:
        """Return string representation."""
        return f"UGUID({self.to_string()})"

    def __repr__(self) -> str:
        """Return detailed representation."""
        return (
            f"UGUID(preamble=0x{self.Preamble:02X}, "
            f"length=0x{self.Length:02X}, "
            f"data={self.to_string()!r})"
        )

    @classmethod
    def from_raw(cls, preamble: int, length: int, data: list[int]) -> UGUID:
        """Create UGUID from raw components (as defined in C headers).

        This method matches the C-style initialization:
        ``{ 0x01, 0x10, {0x93, 0x22, ...} }``

        Args:
            preamble: Preamble byte.
            length: Length byte.
            data: List of 16 bytes.

        Returns:
            A new UGUID instance.
        """
        return cls(data=bytes(data), preamble=preamble, length=length)

    @classmethod
    def from_string(cls, guid_string: str) -> UGUID:
        """Create UGUID from a GUID string.

        Args:
            guid_string: GUID in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
                or without dashes.

        Returns:
            A new UGUID instance.
        """
        return cls(guid_string=guid_string)
