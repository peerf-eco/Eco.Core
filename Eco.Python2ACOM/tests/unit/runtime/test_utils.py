"""Unit tests for `eco_python2acom.runtime.utils`.

Verifies the helpers responsible for converting between UGUIDs and the
hex-encoded DLL filenames used by EcoOS.
"""

import re
from typing import Optional

import pytest

from eco_python2acom.runtime.utils import LIB_EXTENSION, guid_to_lib_filename, lib_filename_to_guid
from eco_python2acom.types.guid import UGUID


@pytest.mark.unit
class TestGuidToLibFilename:
    """Tests for function `guid_to_lib_filename`."""

    @pytest.mark.parametrize(
        "guid",
        [
            UGUID("12345678-90AB-CDEF-1234-567890ABCDEF"),
            UGUID("00000000-0000-0000-0000-000000000000"),
            UGUID("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF"),
        ],
        ids=["standard", "zeros", "ones"],
    )
    def test_guid_to_filename(self, guid: UGUID) -> None:
        """A UGUID is converted into a 32-hex-character library filename."""
        filename = guid_to_lib_filename(guid)
        assert filename.endswith(LIB_EXTENSION)
        stem = filename[: -len(LIB_EXTENSION)]
        assert re.fullmatch(r"[0-9A-F]{32}", stem) is not None


@pytest.mark.unit
class TestLibFilenameToGuid:
    """Tests for function `lib_filename_to_guid`."""

    @pytest.mark.parametrize(
        ["stem", "expected"],
        [
            ("12345678901234567890123456789012", UGUID("12345678-9012-3456-7890-123456789012")),
            ("00000000000000000000000000000000", UGUID("00000000-0000-0000-0000-000000000000")),
            ("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", UGUID("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF")),
            ("abcdefabcdefabcdefabcdefabcdefab", UGUID("ABCDEFAB-CDEF-ABCD-EFAB-CDEFABCDEFAB")),
            (
                "subdir/12345678901234567890123456789012",
                UGUID("12345678-9012-3456-7890-123456789012"),
            ),
            ("ZZZZZZZZ901234567890123456789012", None),
            ("1234567890123456789012345678901", None),  # 31 chars
            ("123456789012345678901234567890123", None),  # 33 chars
            ("12345678-9012-3456-7890-123456789012", None),  # hyphens
        ],
        ids=[
            "valid_hex",
            "zeros",
            "ones",
            "lower_hex",
            "with_directory",
            "invalid_hex",
            "too_short",
            "too_long",
            "with_hyphens",
        ],
    )
    def test_filename_to_guid(self, stem: str, expected: Optional[UGUID]) -> None:
        """A valid hex filename is parsed into the correct UGUID or None."""
        result = lib_filename_to_guid(f"{stem}{LIB_EXTENSION}")
        assert result == expected

    def test_missing_extension_returns_none(self) -> None:
        """A filename without any extension is rejected."""
        assert lib_filename_to_guid("12345678901234567890123456789012") is None

    @pytest.mark.parametrize("ext", [".txt", ".bin", ".invalid"])
    def test_wrong_extension_returns_none(self, ext: str) -> None:
        """Non-matching extensions are rejected."""
        if ext == LIB_EXTENSION:
            pytest.skip("Extension matches `LIB_EXTENSION`")

        result = lib_filename_to_guid(f"12345678901234567890123456789012{ext}")
        assert result is None
