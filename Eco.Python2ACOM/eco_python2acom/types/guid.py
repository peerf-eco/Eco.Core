"""UGUID (Universal GUID) implementation for ACOM/EcoOS.

This module provides the UGUID class for working with EcoOS component
and interface identifiers. UGUID is an 18-byte structure consisting of:
- `Preamble` (1 byte): Version/format indicator
- `Length` (1 byte): Length of the data portion
- `Data` (16 bytes): The actual GUID data

Note:
    The UGUID structure is defined in `IEcoBase1.h`.
    Standard format: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
"""

import re
from typing import ClassVar, Optional, Self

from eco_python2acom.decorators.layout import model
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import UInt8


@model
class UGUID:
    """ACOM Universal GUID structure (18 bytes).

    Used for component (`CID`) and interface (`IID`) identification in EcoOS.
    Consists of a 1-byte preamble, 1-byte length, and 16 bytes of GUID data.

    Attributes:
        preamble: Version/format indicator (default `0x01`).
        length: Length of the data portion (default `0x10`).
        data: 16-byte GUID payload as `Array[UInt8, 16]`.

    Example:
        ```python
        from eco_python2acom.types.guid import UGUID

        # From a standard GUID string
        iid = UGUID("93221116-2248-4742-AE06-82819447843D")
        print(iid.to_string())   # 93221116-2248-4742-AE06-82819447843D

        # From raw bytes (as in C headers)
        cid = UGUID.from_raw(
            preamble=0x01,
            length=0x10,
            data=[0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42,
                  0xAE, 0x06, 0x82, 0x81, 0x94, 0x47, 0x84, 0x3D],
        )

        # Pass to ACOM methods via `byref`
        from eco_python2acom.types.utils import byref
        bus.obj.QueryComponent(bus, byref(cid), None, byref(iid), byref(ppv))
        ```
    """

    preamble: UInt8
    length: UInt8
    data: Array[UInt8, 0x10]

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
            ValueError: If both `guid_string` and `data` are provided, or if the
                provided data is not 16 bytes long.
        """
        self.preamble = UInt8(preamble)
        self.length = UInt8(length)

        if guid_string is not None and data is not None:
            raise ValueError("Provide either 'guid_string' or 'data', not both")

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
            raise ValueError(f"Invalid GUID format: {guid_string!r}")

        hex_str = "".join(match.groups())
        self.data[:] = [UInt8(byte) for byte in bytes.fromhex(hex_str)]

    def _from_bytes(self, data: bytes) -> None:
        """Initialize from raw bytes.

        Args:
            data: 16-byte GUID data.

        Raises:
            ValueError: If data length is not `0x10` (16 bytes).
        """
        if len(data) != 0x10:
            raise ValueError(f"GUID data must be 16 bytes, got {len(data)}")

        self.data[:] = [UInt8(byte) for byte in data]

    def to_bytes(self) -> bytes:
        """Convert the `data` field to bytes.

        Returns:
            The 16-byte GUID data.
        """
        return bytes(self.data)

    def to_string(self, with_hyphens: bool = True) -> str:
        """Convert to standard GUID string format.

        Args:
            with_hyphens: If True, add hyphens between the parts of the GUID.

        Returns:
            GUID string in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
            or "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" if `with_hyphens` is False.
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
            self.preamble == other.preamble
            and self.length == other.length
            and self.to_bytes() == other.to_bytes()
        )

    def __str__(self) -> str:
        """Return string representation."""
        return self.to_string()

    def __repr__(self) -> str:
        """Return detailed representation."""
        return f"UGUID(preamble=0x{self.preamble:02X}, length=0x{self.length:02X}, data={self.to_string()})"

    @classmethod
    def from_raw(cls, preamble: int, length: int, data: list[int]) -> Self:
        """Create UGUID from raw components (as defined in C headers).

        This method matches the C-style initialization: `{ 0x01, 0x10, {0x93, 0x22, ...} }`

        Args:
            preamble: Preamble byte.
            length: Length byte.
            data: List of 16 bytes.

        Returns:
            A new UGUID instance.
        """
        return cls(data=bytes(data), preamble=preamble, length=length)

    @classmethod
    def from_string(cls, guid_string: str) -> Self:
        """Create UGUID from a GUID string.

        Args:
            guid_string: GUID in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" or without dashes.

        Returns:
            A new UGUID instance.
        """
        return cls(guid_string=guid_string)


__all__ = ["UGUID"]
