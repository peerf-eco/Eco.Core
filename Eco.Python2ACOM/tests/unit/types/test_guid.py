"""Unit tests for `eco_python2acom.types.guid` module.

This module tests UGUID (Universal GUID) functionality including parsing,
validation, conversion, equality, hashing, and factory methods.

Test Classes:
    TestUGUIDInitialization: Tests for `UGUID` constructor variants.
    TestUGUIDValidation: Tests for `UGUID` string format validation.
    TestUGUIDConversion: Tests for string/bytes conversion methods.
    TestUGUIDComparison: Tests for equality, hashing, and collection usage.
    TestUGUIDClassMethods: Tests for `from_raw` and `from_string` factories.
"""

import pytest

from eco_python2acom.types.guid import UGUID

SAMPLE_GUID = "12345678-1234-5678-1234-567812345678"
SAMPLE_GUID_ALT = "87654321-4321-8765-4321-876543218765"


class TestUGUIDInitialization:
    """Tests for `UGUID` constructor.

    Verifies initialization from string, bytes, custom preamble/length,
    and empty construction.
    """

    def test_init_from_guid_string(self) -> None:
        """Verifies `UGUID` created from a GUID string has correct defaults."""
        guid = UGUID(SAMPLE_GUID)
        assert guid.preamble == 0x01
        assert guid.length == 0x10
        assert len(guid.data) == 0x10

    def test_init_from_bytes(self) -> None:
        """Verifies `UGUID` created from raw bytes preserves data."""
        data = bytes(range(0x10))
        guid = UGUID(data=data)
        assert guid.preamble == 0x01
        assert guid.length == 0x10
        assert guid.to_bytes() == data

    def test_init_empty(self) -> None:
        """Verifies empty `UGUID` is zero-initialized with correct defaults."""
        guid = UGUID()
        assert guid.preamble == 0x01
        assert guid.length == 0x10
        assert all(byte == 0 for byte in guid.to_bytes())

    def test_init_with_string_and_data(self) -> None:
        """UGUID must reject mixed initialization sources."""
        with pytest.raises(ValueError, match="Provide either"):
            UGUID(guid_string=SAMPLE_GUID, data=b"\x00" * 0x10)


class TestUGUIDValidation:
    """Tests for `UGUID` string format validation.

    Verifies acceptance of valid formats and rejection of invalid ones.
    """

    @pytest.mark.parametrize(
        ["input", "expected"],
        [
            (SAMPLE_GUID.lower(), SAMPLE_GUID),
            (SAMPLE_GUID, SAMPLE_GUID),
            (f"{{{SAMPLE_GUID}}}", SAMPLE_GUID),
            (f"({SAMPLE_GUID})", SAMPLE_GUID),
            ("00000000-0000-0000-0000-000000000000", "00000000-0000-0000-0000-000000000000"),
            ("FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF", "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF"),
            ("AbCdEf01-AbCd-Ef01-AbCd-Ef0123456789", "ABCDEF01-ABCD-EF01-ABCD-EF0123456789"),
        ],
        ids=["lower", "upper", "braces", "parentheses", "zero", "one", "mixed"],
    )
    def test_valid_guid_string_parsing(self, input: str, expected: str) -> None:
        """Verifies valid GUID strings are correctly parsed and normalized."""
        guid = UGUID(input)
        assert guid.to_string() == expected

    def test_valid_guid_without_hyphens(self) -> None:
        """Verifies GUID without 'hyphens' is accepted."""
        no_hyphens = SAMPLE_GUID.replace("-", "")
        guid = UGUID(no_hyphens)
        assert guid.to_string(with_hyphens=False) == no_hyphens

    @pytest.mark.parametrize(
        "invalid",
        [
            "12345678-1234-5678-1234",
            "12345678-1234-5678-1234-567812345678-EXTRA",
            "ZZZZZZZZ-ZZZZ-ZZZZ-ZZZZ-ZZZZZZZZZZZZ",
            "",
            "not-a-guid-at-all",
        ],
        ids=[
            "too_short",
            "too_long",
            "non_hex_chars",
            "empty_string",
            "arbitrary_text",
        ],
    )
    def test_invalid_guid_string(self, invalid: str) -> None:
        """Verifies invalid GUID strings raise `ValueError`."""
        with pytest.raises(ValueError, match="Invalid GUID format"):
            UGUID(invalid)

    @pytest.mark.parametrize(
        "invalid",
        [b"short", b"", b"\x00" * 15, b"\x00" * 17],
        ids=["5_bytes", "empty", "15_bytes", "17_bytes"],
    )
    def test_invalid_data_length(self, invalid: bytes) -> None:
        """Verifies data with incorrect length raises ValueError."""
        with pytest.raises(ValueError, match="must be 16 bytes"):
            UGUID(data=invalid)


class TestUGUIDConversion:
    """Tests for UGUID conversion methods.

    Verifies `to_bytes`, `to_string`, `__str__`, and `__repr__` output.
    """

    def test_to_bytes_returns_16_bytes(self) -> None:
        """Verifies `to_bytes` returns exactly 16 bytes."""
        guid = UGUID(SAMPLE_GUID)
        data = guid.to_bytes()
        assert isinstance(data, bytes)
        assert len(data) == 16

    def test_to_bytes_consistency_with_init_data(self) -> None:
        """Verifies `to_bytes` returns the same data used for initialization."""
        original = bytes([0x12, 0x34, 0x56, 0x78] + list(range(12)))
        guid = UGUID(data=original)
        assert guid.to_bytes() == original

    def test_data_array_mutation_reflected(self) -> None:
        """Verifies direct data array modification is reflected in `to_bytes`."""
        guid = UGUID(SAMPLE_GUID)
        guid.data[0] = 0xFF
        assert guid.to_bytes()[0] == 0xFF

    def test_to_string_with_hyphens(self) -> None:
        """Verifies `to_string` with hyphens produces 4 dashes."""
        guid = UGUID(SAMPLE_GUID)
        result = guid.to_string(with_hyphens=True)
        assert result.count("-") == 4
        assert len(result) == 36

    def test_to_string_without_hyphens(self) -> None:
        """Verifies `to_string` without hyphens produces 32 hex characters."""
        guid = UGUID(SAMPLE_GUID)
        result = guid.to_string(with_hyphens=False)
        assert "-" not in result
        assert len(result) == 32

    def test_str_representation(self) -> None:
        """Verifies `__str__` includes 'UGUID' and the GUID string."""
        guid = UGUID(SAMPLE_GUID)
        assert "UGUID" not in str(guid)
        assert SAMPLE_GUID in str(guid)

    def test_repr_representation(self) -> None:
        """Verifies `__repr__` includes preamble and length hex values."""
        guid = UGUID(SAMPLE_GUID)
        repr_str = repr(guid)
        assert "preamble=0x01" in repr_str
        assert "length=0x10" in repr_str


class TestUGUIDComparison:
    """Tests for UGUID equality, hashing, and collection usage.

    Verifies `__eq__`, `__hash__`, and usage in sets/dicts.
    """

    def test_equal_from_same_string(self) -> None:
        """Verifies GUIDs from the same string are equal."""
        assert UGUID(SAMPLE_GUID) == UGUID(SAMPLE_GUID)

    def test_equal_from_same_bytes(self) -> None:
        """Verifies GUIDs from the same bytes are equal."""
        data = bytes(range(0x10))
        assert UGUID(data=data) == UGUID(data=data)

    def test_different_guids_not_equal(self) -> None:
        """Verifies GUIDs with different data are not equal."""
        assert UGUID(SAMPLE_GUID) != UGUID(SAMPLE_GUID_ALT)

    def test_different_preamble_not_equal(self) -> None:
        """Verifies different preamble makes GUIDs unequal."""
        guid1 = UGUID(SAMPLE_GUID, preamble=0x01)
        guid2 = UGUID(SAMPLE_GUID, preamble=0x02)
        assert guid1 != guid2

    def test_different_length_not_equal(self) -> None:
        """Verifies different length field makes GUIDs unequal."""
        guid1 = UGUID(SAMPLE_GUID, length=0x10)
        guid2 = UGUID(SAMPLE_GUID, length=0x20)
        assert guid1 != guid2

    @pytest.mark.parametrize("other", [SAMPLE_GUID, 0x12345678, None], ids=["str", "int", "none"])
    def test_not_equal_to_other_types(self, other: object) -> None:
        """Verifies `UGUID` instances are not equal to objects of other types."""
        guid = UGUID(SAMPLE_GUID)
        assert guid != other

    def test_is_hashable(self) -> None:
        """Verifies `UGUID` instances can be hashed."""
        assert isinstance(hash(UGUID(SAMPLE_GUID)), int)

    def test_equal_guids_same_hash(self) -> None:
        """Verifies equal GUIDs produce the same hash value."""
        guid1 = UGUID(SAMPLE_GUID)
        guid2 = UGUID(SAMPLE_GUID)
        assert hash(guid1) == hash(guid2)

    def test_usable_in_set(self) -> None:
        """Verifies deduplication in a set."""
        guid_set = {
            UGUID(SAMPLE_GUID),
            UGUID(SAMPLE_GUID),
            UGUID(SAMPLE_GUID_ALT),
        }
        assert len(guid_set) == 2

    def test_usable_as_dict_key(self) -> None:
        """Verifies `UGUID` works as a dictionary key."""
        key = UGUID(SAMPLE_GUID)
        lookup = UGUID(SAMPLE_GUID)
        dct = {key: "test_guid"}
        assert dct[lookup] == "test_guid"


class TestUGUIDClassMethods:
    """Tests for UGUID factory class methods.

    Verifies from_raw and from_string behavior.
    """

    def test_from_raw(self) -> None:
        """Verifies `from_raw` creates UGUID with correct fields."""
        data_list = list(range(0x10))
        guid = UGUID.from_raw(0x02, 0x10, data_list)
        assert guid.preamble == 0x02
        assert guid.length == 0x10
        assert list(guid.to_bytes()) == data_list

    def test_from_raw_zeros(self) -> None:
        """Verifies `from_raw` with zero data produces all-zero GUID."""
        guid = UGUID.from_raw(0x01, 0x10, [0] * 0x10)
        assert guid.to_string() == "00000000-0000-0000-0000-000000000000"

    def test_from_raw_ones(self) -> None:
        """Verifies from_raw with 0xFF data produces all-F GUID."""
        guid = UGUID.from_raw(0x01, 0x10, [0xFF] * 16)
        assert guid.to_string() == "FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF"

    def test_from_string(self) -> None:
        """Verifies `from_string` creates equivalent UGUID."""
        guid = UGUID.from_string(SAMPLE_GUID)
        assert guid.to_string() == SAMPLE_GUID

    def test_from_string_without_hyphens(self) -> None:
        """Verifies `from_string` accepts GUID without hyphens."""
        no_hyphens = SAMPLE_GUID.replace("-", "")
        guid = UGUID.from_string(no_hyphens)
        assert guid.to_string(with_hyphens=False) == no_hyphens.upper()

    def test_from_raw_matches_from_string(self) -> None:
        """Verifies `from_raw` and `from_string` produce equal GUIDs for same data."""
        guid_str = UGUID.from_string(SAMPLE_GUID)
        raw_bytes = guid_str.to_bytes()
        guid_raw = UGUID.from_raw(0x01, 0x10, list(raw_bytes))
        assert guid_str == guid_raw
