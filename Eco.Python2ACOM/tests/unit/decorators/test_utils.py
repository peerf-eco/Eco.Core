"""Unit tests for `eco_python2acom.decorators.utils` public helpers.

Verifies the behavior of `validate` and `normalize` used during
type-hint resolution for `@model`, `@union`, and `@interface` decorators.

Test Classes:
    TestValidate: Verifies acceptance and rejection of EcoOS data types.
    TestNormalize: Verifies normalization of annotations.
"""

from types import NoneType
from typing import Any, Optional, Union

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model, union
from eco_python2acom.decorators.utils import normalize, validate
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import (
    Double,
    Float,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    Void,
)
from eco_python2acom.types.pointer import Ptr


@model
class SampleModel:
    value: Int32


@union
class SampleUnion:
    as_int: Int32
    as_float: Float


@interface(iid="ABCDEF12-3456-7890-ABCD-EF1234567890")
class SampleInterface(IEcoUnknown):
    def Method(self) -> Int16:
        ...


class TestValidate:
    """Tests for `validate` function.

    Verifies that valid EcoOS data types pass through unchanged
    and non-EcoOS types raise `TypeError`.
    """

    @pytest.mark.parametrize(
        "hint",
        [
            Int8,
            UInt8,
            Int16,
            UInt16,
            Int32,
            UInt32,
            Int64,
            UInt64,
            Float,
            Double,
            Void,
            NoneType,
            Ptr[Int32],
            Ptr[Void],
            Array[Int32, 4],
            SampleModel,
            SampleUnion,
            SampleInterface,
        ],
        ids=[
            "int8",
            "uint8",
            "int16",
            "uint16",
            "int32",
            "uint32",
            "int64",
            "uint64",
            "float",
            "double",
            "void",
            "nonetype",
            "ptr_typed",
            "ptr_void",
            "array",
            "model",
            "union",
            "interface",
        ],
    )
    def test_different_types_valid(self, hint: type) -> None:
        """Verifies valid EcoOS types pass validation unchanged."""
        assert validate(hint) is hint

    @pytest.mark.parametrize(
        "hint",
        [
            str,
            int,
            float,
            list,
            dict,
            tuple,
            bytes,
            bool,
            42,
            "Int32",
            None,
            3.14,
            object(),
        ],
        ids=[
            "str",
            "int",
            "float",
            "list",
            "dict",
            "tuple",
            "bytes",
            "bool",
            "int_value",
            "string_value",
            "none_value",
            "float_value",
            "instance",
        ],
    )
    def test_different_types_invalid(self, hint: Any) -> None:
        """Verifies non-EcoOS types and non-type values are rejected."""
        with pytest.raises(TypeError, match="Invalid EcoOS data type"):
            validate(hint)

    def test_custom_python_class_invalid(self) -> None:
        """Verifies arbitrary Python classes are rejected."""

        class Plain:
            pass

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):
            validate(Plain)


class TestNormalize:
    """Tests for `normalize` function.

    Verifies that `Optional[T]` and `Union[T, None]` collapse to `T`,
    plain types pass through, and ambiguous unions raise `TypeError`.
    """

    @pytest.mark.parametrize(
        "hint",
        [
            Int8,
            Int32,
            Double,
            Void,
            Ptr[Int32],
            Array[Int32, 4],
            SampleModel,
            SampleUnion,
            SampleInterface,
        ],
        ids=[
            "int8",
            "int32",
            "double",
            "void",
            "ptr",
            "array",
            "model",
            "union",
            "interface",
        ],
    )
    def test_plain_type_unchanged(self, hint: type) -> None:
        """Verifies non-Union types pass through unchanged."""
        assert normalize(hint) is hint

    @pytest.mark.parametrize(
        ["hint", "expected"],
        [
            (Optional[Int32], Int32),
            (Union[Int32, None], Int32),
            (Optional[Ptr[Int32]], Ptr[Int32]),
            (Optional[Array[Int32, 3]], Array[Int32, 3]),
            (Int32 | None, Int32),
            (Ptr[Int32] | None, Ptr[Int32]),
            (Optional[SampleModel], SampleModel),
            (SampleUnion | None, SampleUnion),
        ],
        ids=[
            "optional_int32",
            "union_int32_none",
            "optional_ptr",
            "optional_array",
            "pep604_int32",
            "pep604_ptr",
            "optional_model",
            "pep604_union",
        ],
    )
    def test_optional_collapses_to_inner(self, hint: type, expected: type) -> None:
        """Verifies `Optional[T]` / `T | None` collapse to `T` across syntaxes."""
        assert normalize(hint) is expected

    @pytest.mark.parametrize(
        "hint",
        [
            Union[Int32, Int16],
            Union[Int32, Int16, None],
            Int32 | Int16,
            Int32 | Int16 | None,
        ],
        ids=[
            "union_two_types",
            "union_two_types_with_none",
            "pep604_two_types",
            "pep604_two_types_with_none",
        ],
    )
    def test_ambiguous_union_raises(self, hint: type) -> None:
        """Verifies unions with more than one non-None type raise `TypeError`."""
        with pytest.raises(TypeError, match="Invalid EcoOS union type"):
            normalize(hint)
