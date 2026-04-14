"""Unit tests for `eco_python2acom.types.errors` module.

This module tests error handling, error codes, and exception functionality
for EcoOS ACOM operations.

Test Classes:
    TestEcoErrorCode: Tests for `EcoErrorCode` enum values and behavior.
    TestErrorMessages: Tests for `ERROR_MESSAGES` dictionary completeness.
    TestEcoErrorInitialization: Tests for `EcoError` constructor variants.
    TestEcoErrorFormatting: Tests for `EcoError` string formatting and repr.
    TestEcoErrorEquality: Tests for `EcoError` equality comparison.
    TestEcoErrorHashing: Tests for `EcoError` hashing and collection usage.
"""

from typing import Any

import pytest

from eco_python2acom.types.core import Int16
from eco_python2acom.types.errors import ERROR_MESSAGES, EcoError, EcoErrorCode


class TestEcoErrorCode:
    """Tests for `EcoErrorCode` enum.

    Verifies that error codes match expected hex values from `ErrEcoCodes.h`
    and behave as `IntEnum` members.
    """

    @pytest.mark.parametrize(
        ["code", "expected"],
        [
            (EcoErrorCode.SUCCESS, 0x0000),
            (EcoErrorCode.USER, 0x0002),
            (EcoErrorCode.UNEXPECTED, 0xFFFF),
            (EcoErrorCode.POINTER, 0xFFEE),
            (EcoErrorCode.NOINTERFACE, 0xFFED),
            (EcoErrorCode.NOSYSTEM, 0xFFEC),
            (EcoErrorCode.NOBUS, 0xFFEB),
            (EcoErrorCode.NOAGGREGATION, 0xFFEA),
            (EcoErrorCode.COMPONENT_NOTFOUND, 0xFFE9),
            (EcoErrorCode.OUTOFMEMORY, 0xFFE8),
            (EcoErrorCode.MEMMGR_NOTREG, 0xFFE7),
            (EcoErrorCode.GET_MEMORY_ALLOCATOR, 0xFFE6),
            (EcoErrorCode.FAIL, 0xFFE5),
            (EcoErrorCode.NOTIMPL, 0xFFE4),
            (EcoErrorCode.INVALIDARG, 0xFFE3),
            (EcoErrorCode.FILEMGR_NOTREG, 0xFFE2),
            (EcoErrorCode.NETMGR_NOTREG, 0xFFE1),
            (EcoErrorCode.OUTINTERFACE_NOCONNECTION, 0xFFE0),
        ],
        ids=[code.name.lower() for code in EcoErrorCode],
    )
    def test_enum_values_match_expected(self, code: EcoErrorCode, expected: int) -> None:
        """Each enum member should match its expected hexadecimal value."""
        assert code == expected

    def test_enum_behaves_like_int(self) -> None:
        """Enum members should behave like integers."""
        code = EcoErrorCode.SUCCESS
        assert int(code) == 0
        assert code + 1 == 1

    def test_enum_can_be_created_from_int(self) -> None:
        """Enum should be constructible from raw integer values."""
        code = EcoErrorCode(0xFFEE)
        assert code is EcoErrorCode.POINTER


class TestErrorMessages:
    """Tests for `ERROR_MESSAGES` dictionary.

    Verifies message completeness and consistency with error codes.
    """

    def test_all_codes_have_messages(self) -> None:
        """Verifies every non-USER `EcoErrorCode` member has a corresponding message.

        `USER` is excluded because it is a placeholder for user-defined codes
        and has no predefined message.
        """
        for code in EcoErrorCode:
            if code == EcoErrorCode.USER:
                continue
            assert code in ERROR_MESSAGES, f"Missing message for {code.name}"

    def test_all_messages_are_nonempty_strings(self) -> None:
        """Verifies all messages are non-empty strings."""
        for code, msg in ERROR_MESSAGES.items():
            assert isinstance(msg, str)
            assert len(msg) > 0, f"Empty message for {code.name}"

    @pytest.mark.parametrize(
        ["code", "expected"],
        [
            (EcoErrorCode.SUCCESS, "Operation completed successfully"),
            (EcoErrorCode.POINTER, "Invalid pointer value was passed"),
            (EcoErrorCode.OUTOFMEMORY, "Memory allocation failure"),
            (EcoErrorCode.NOINTERFACE, "Interface is not supported"),
        ],
        ids=["success", "pointer", "out_of_memory", "no_interface"],
    )
    def test_message_content(self, code: EcoErrorCode, expected: str) -> None:
        """Verifies specific messages contain expected keywords."""
        assert expected.lower() in ERROR_MESSAGES[code].lower()


class TestEcoErrorInitialization:
    """Tests for `EcoError` constructor.

    Verifies initialization from `EcoErrorCode`, `int`, `Int16`,
    and with optional message/operation parameters.
    """

    @pytest.mark.parametrize(
        ["input", "expected"],
        [
            (EcoErrorCode.POINTER, EcoErrorCode.POINTER),
            (0xFFEE, EcoErrorCode.POINTER),
            (Int16(0xFFEE), EcoErrorCode.POINTER),
            (0, EcoErrorCode.SUCCESS),
            (0x1234, EcoErrorCode.FAIL),
        ],
        ids=["enum", "hex", "int16", "zero", "unknown"],
    )
    def test_code_initialization(self, input: Any, expected: EcoErrorCode) -> None:
        """Verifies constructor correctly resolves input values to error codes."""
        error = EcoError(input)
        assert error.code == expected

    def test_init_with_parameters(self) -> None:
        """Verifies all three parameters are stored correctly."""
        error = EcoError(
            EcoErrorCode.POINTER,
            message="Null pointer",
            operation="dereference",
        )
        assert error.code == EcoErrorCode.POINTER
        assert error.message == "Null pointer"
        assert error.operation == "dereference"

    def test_default_message_from_error_messages(self) -> None:
        """Verifies default message is taken from `ERROR_MESSAGES` dict."""
        error = EcoError(EcoErrorCode.SUCCESS)
        assert error.message == ERROR_MESSAGES[EcoErrorCode.SUCCESS]

    def test_operation_defaults_to_none(self) -> None:
        """Verifies operation defaults to `None`."""
        error = EcoError(EcoErrorCode.POINTER)
        assert error.operation is None


class TestEcoErrorFormatting:
    """Tests for EcoError string formatting.

    Verifies `__str__` and `__repr__` output format.
    """

    @pytest.mark.parametrize(
        ["error", "expected"],
        [
            (EcoError(EcoErrorCode.POINTER), "[0xFFEE]"),
            (EcoError(EcoErrorCode.POINTER, operation="dereference"), "(during dereference)"),
            (EcoError(EcoErrorCode.POINTER, message="Custom message"), "Custom message"),
        ],
        ids=["hex_code", "with_operation", "custom_message"],
    )
    def test_str_contains_expected_parts(self, error: EcoError, expected: str) -> None:
        """Verifies str() contains important parts."""
        assert expected in str(error)

    def test_str_without_operation(self) -> None:
        """Verifies `str()` excludes 'during' when no operation is set."""
        error = EcoError(EcoErrorCode.POINTER)
        assert "during" not in str(error)

    def test_repr_format(self) -> None:
        """Verifies repr() contains class name and key attributes."""
        error = EcoError(EcoErrorCode.POINTER, message="Test", operation="operation")
        result = repr(error)

        assert result.startswith("EcoError(")
        assert "Test" in result
        assert "operation" in result


class TestEcoErrorEquality:
    """Tests for EcoError equality comparison.

    Verifies `__eq__` compares code, message, and operation.
    """

    def test_equal_errors(self) -> None:
        """Verifies errors with same attributes are equal."""
        error1 = EcoError(EcoErrorCode.POINTER)
        error2 = EcoError(EcoErrorCode.POINTER)
        assert error1 == error2

    def test_different_codes_not_equal(self) -> None:
        """Verifies errors with different codes are not equal."""
        error1 = EcoError(EcoErrorCode.POINTER)
        error2 = EcoError(EcoErrorCode.NOINTERFACE)
        assert error1 != error2

    def test_different_messages_not_equal(self) -> None:
        """Verifies errors with different messages are not equal."""
        error1 = EcoError(EcoErrorCode.POINTER, message="Message 1")
        error2 = EcoError(EcoErrorCode.POINTER, message="Message 2")
        assert error1 != error2

    def test_different_operations_not_equal(self) -> None:
        """Verifies errors with different operations are not equal."""
        error1 = EcoError(EcoErrorCode.POINTER, operation="operation 1")
        error2 = EcoError(EcoErrorCode.POINTER, operation="operation 2")
        assert error1 != error2

    @pytest.mark.parametrize("other", ["error", 0xFFEE, None], ids=["str", "int", "none"])
    def test_not_equal_to_other_types(self, other: object) -> None:
        """Verifies `EcoError` instances are not equal to objects of other types."""
        error = EcoError(EcoErrorCode.POINTER)
        assert error != other


class TestEcoErrorHashing:
    """Tests for `EcoError` hashing.

    Verifies hash consistency and collection usage.
    """

    def test_is_hashable(self) -> None:
        """Verifies `EcoError` instances can be hashed."""
        error = EcoError(EcoErrorCode.POINTER)
        assert isinstance(hash(error), int)

    def test_equal_errors_same_hash(self) -> None:
        """Verifies equal errors produce the same hash value."""
        error1 = EcoError(EcoErrorCode.POINTER)
        error2 = EcoError(EcoErrorCode.POINTER)
        assert hash(error1) == hash(error2)

    def test_usable_in_set(self) -> None:
        """Verifies deduplication in a set."""
        error_set = {
            EcoError(EcoErrorCode.POINTER),
            EcoError(EcoErrorCode.POINTER),
            EcoError(EcoErrorCode.NOINTERFACE),
        }
        assert len(error_set) == 2

    def test_usable_as_dict_key(self) -> None:
        """Verifies `EcoError` works as a dictionary key."""
        key = EcoError(EcoErrorCode.POINTER)
        lookup = EcoError(EcoErrorCode.POINTER)
        dct = {key: "pointer_error"}
        assert dct[lookup] == "pointer_error"
