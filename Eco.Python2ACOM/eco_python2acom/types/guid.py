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
from typing import ClassVar, Union

from eco_python2acom.decorators.layout import model
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import UInt8


@model
class UGUID:
    """ACOM Universal GUID structure (18 bytes).

    Used for component (`CID`), interface (`IID`), and generation (`GID`)
    identification in EcoOS. Consists of a 1-byte preamble, 1-byte length,
    and 16 bytes of GUID data.

    Attributes:
        preamble: Version/format indicator (default `0x01`).
        length: Length of the data portion (`0x10`).
        data: 16-byte GUID payload as `Array[UInt8, 16]`.

    Example:
        ```python
        from eco_python2acom.types.guid import UGUID

        # From a standard GUID string
        iid = UGUID("93221116-2248-4742-AE06-82819447843D")
        print(iid)   # 93221116-2248-4742-AE06-82819447843D

        # From raw bytes (as in C headers)
        cid = UGUID(
            data=bytes([0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42,
                0xAE, 0x06, 0x82, 0x81, 0x94, 0x47, 0x84, 0x3D]),
            preamble=0x01,
        )

        # Pass to ACOM methods via `byref`
        from eco_python2acom.types.utils import byref
        bus.obj.QueryComponent(byref(cid), None, byref(iid), byref(ppv))
        ```

    Note:
        UGUID is intended as an immutable value object. Mutating fields
        after construction will break hash consistency.
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

    # Size of the GUID payload in bytes
    GUID_LENGTH: ClassVar[int] = 0x10

    def __init__(
        self,
        data: Union[str, bytes] = bytes(GUID_LENGTH),
        preamble: int = 0x01,
    ) -> None:
        """Initialize UGUID from a GUID string or raw 16 bytes.

        Args:
            data: Either a GUID string (`"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"`)
                or 16 raw bytes. Defaults to an all-zero GUID.
            preamble: Preamble byte (default `0x01`).

        Raises:
            TypeError: If `data` is not `str` or `bytes`.
            ValueError: If the string format is invalid or raw bytes are not 16 bytes long.
        """
        self.preamble = preamble
        self.length = self.GUID_LENGTH

        if isinstance(data, str):
            self._from_string(data)
        elif isinstance(data, bytes):
            self._from_bytes(data)
        else:
            raise TypeError(f"UGUID data must be 'str' or 'bytes', got '{type(data).__name__}'")

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
        if len(data) != self.GUID_LENGTH:
            raise ValueError(f"GUID data must be 16 bytes, got {len(data)}")

        self.data[:] = [UInt8(byte) for byte in data]

    def to_bytes(self) -> bytes:
        """Convert the `data` field to bytes.

        Returns:
            The 16-byte GUID payload (without `preamble`/`length`).
        """
        return bytes(self.data)

    def to_string(self, with_hyphens: bool = True) -> str:
        """Convert to standard GUID string format.

        Args:
            with_hyphens: If True, add hyphens between the parts of the GUID.

        Returns:
            GUID in format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" or without dashes.
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

        Comparison is done on all 18 bytes of the ABI layout
        (`preamble`, `length`, `data`).
        """
        if not isinstance(other, UGUID):
            return NotImplemented
        return bytes(self) == bytes(other)  # type: ignore

    def __hash__(self) -> int:
        """Return hash of the UGUID."""
        return hash(bytes(self))  # type: ignore

    def __str__(self) -> str:
        """Return string representation."""
        return self.to_string()


__all__ = ["UGUID"]
