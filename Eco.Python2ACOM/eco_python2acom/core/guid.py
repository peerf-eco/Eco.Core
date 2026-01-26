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

import ctypes
import re
from typing import ClassVar, Optional


class UGUID(ctypes.Structure):
    """ACOM Universal GUID structure.

    An 18-byte structure used for component and interface identification
    in the EcoOS/ACOM system.

    Args:
        guid_string: A GUID string in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".
        data: Raw 16-byte GUID data.
        preamble: Preamble byte (default: 0x01).
        length: Length byte (default: 0x10 = 16).

    Attributes:
        Preamble: Version/format indicator (1 byte).
        Length: Length of data portion (1 byte).
        Data: The 16-byte GUID data.
    """

    _fields_: ClassVar[list[tuple[str, type]]] = [
        ("Preamble", ctypes.c_uint8),
        ("Length", ctypes.c_uint8),
        ("Data", ctypes.c_uint8 * 16),
    ]

    # Well-known GUIDs
    IID_IEcoUnknown: ClassVar[UGUID]
    IID_IEcoComponentFactory: ClassVar[UGUID]

    # Regex pattern for GUID string validation
    _GUID_PATTERN: ClassVar[re.Pattern[str]] = re.compile(
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
        super().__init__()
        self.Preamble = preamble
        self.Length = length

        if guid_string is not None:
            self._from_string(guid_string)
        elif data is not None:
            self._from_bytes(data)
        else:
            # Initialize with zeros
            for i in range(16):
                self.Data[i] = 0

    def _from_string(self, guid_string: str) -> None:
        """Parse a GUID string into the Data field.

        Args:
            guid_string: GUID in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".

        Raises:
            ValueError: If the string format is invalid.
        """
        match = self._GUID_PATTERN.match(guid_string)
        if not match:
            raise ValueError(
                f"Invalid GUID format: {guid_string!r}. "
                "Expected format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
            )

        # Concatenate all groups and convert to bytes
        hex_str = "".join(match.groups())
        data_bytes = bytes.fromhex(hex_str)

        for i, byte in enumerate(data_bytes):
            self.Data[i] = byte

    def _from_bytes(self, data: bytes) -> None:
        """Initialize from raw bytes.

        Args:
            data: 16-byte GUID data.

        Raises:
            ValueError: If data length is not 16.
        """
        if len(data) != 16:
            raise ValueError(f"GUID data must be 16 bytes, got {len(data)}")

        for i, byte in enumerate(data):
            self.Data[i] = byte

    def to_bytes(self) -> bytes:
        """Convert the Data field to bytes.

        Returns:
            The 16-byte GUID data.
        """
        return bytes(self.Data)

    def to_string(self, with_braces: bool = False) -> str:
        """Convert to standard GUID string format.

        Args:
            with_braces: If True, wrap the GUID in curly braces.

        Returns:
            GUID string in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX".
        """
        data = self.to_bytes()
        guid_str = (
            f"{data[0:4].hex().upper()}-"
            f"{data[4:6].hex().upper()}-"
            f"{data[6:8].hex().upper()}-"
            f"{data[8:10].hex().upper()}-"
            f"{data[10:16].hex().upper()}"
        )
        if with_braces:
            return f"{{{guid_str}}}"
        return guid_str

    def __eq__(self, other: object) -> bool:
        """Compare two UGUIDs for equality.

        Comparison is done on all 18 bytes (Preamble + Length + Data).
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


# =============================================================================
# Well-known GUIDs (from IEcoBase1.h)
# =============================================================================

# IEcoUnknown IID = 00000000-0000-0000-0000-0000000000AA
UGUID.IID_IEcoUnknown = UGUID.from_raw(
    0x01,
    0x10,
    [
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xAA,
    ],
)

# IEcoComponentFactory IID = 00000000-0000-0000-0000-000000000055
UGUID.IID_IEcoComponentFactory = UGUID.from_raw(
    0x01,
    0x10,
    [
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x55,
    ],
)


# Type alias for flexibility
UGUIDPtr = ctypes.POINTER(UGUID)
