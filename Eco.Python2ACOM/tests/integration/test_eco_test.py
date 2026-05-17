"""Integration tests for the `Eco.Test` component contract.

Every test in this module runs twice: once against the native
`Eco.Test.dll` and once against `EcoTest` — the pure-Python implementation
hosted by the `Eco.ACOM2Python` bridge. Both back-ends expose the same
`IEcoTest` interface and must produce identical results; the parameter id
(`[c]` / `[py]`) tells which one a given run targets.

Coverage spans the full surface: arithmetic and floating-point math,
pointer-based array operations, in-place sorting, string return, plain and
nested structures, unions and tagged variants, the user-supplied comparator
callback, reference counting, and `QueryInterface` dispatch.

The entire session is skipped if the runtime or `Eco.Test` library is
unavailable; the `[py]` variants are skipped if the bridge is not loaded.

Test Classes:
    TestEcoTestArithmetic: Tests for `Addition`, `Negation` (int32 operations).
    TestEcoTestFloatingPoint: Tests for `Multiplication` (double operations).
    TestEcoTestArrays: Tests for `SumArray`, `FillArray` (pointer + size).
    TestEcoTestBubbleSort: Tests for `SortArray` (in-place mutation).
    TestEcoTestStrings: Tests for `GetName` (C string return).
    TestEcoTestStructures: Tests for `EcoPoint` interop (make, distance, translate).
    TestEcoTestUnions: Tests for `EcoValue` union (pack / unpack by pointer).
    TestEcoTestNestedStructures: Tests for `EcoRect` (nested struct, points by pointer).
    TestEcoTestArrayOfStructures: Tests for `SumPoints` (array of `EcoPoint`).
    TestEcoTestVariant: Tests for `EcoVariant` (model containing a union by value).
    TestEcoTestRefCounting: Tests for `AddRef`/`Release` lifecycle.
    TestEcoTestQueryInterface: Tests for `QueryInterface` (IID dispatch, refcount behavior).
    TestEcoTestCallback: Tests for `SortArrayWith` (user-supplied comparator callback).
    TestEcoTestStringParam: Tests for `StringLength` (C string as in-parameter).
"""

import math

import pytest

from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import Double, Int32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from tests.integration.structures import (
    ECO_VALUE_DOUBLE,
    ECO_VALUE_INT,
    EcoCompareFunc,
    EcoPoint,
    EcoRect,
    EcoValue,
    EcoVariant,
    IEcoTest,
    IID_IEcoTest,
)

# Int32 boundary values
INT32_MIN = -2_147_483_648
INT32_MAX = 2_147_483_647


@pytest.mark.integration
class TestEcoTestArithmetic:
    """Tests for basic integer arithmetic methods."""

    @pytest.mark.parametrize(
        ["left", "right", "expected"],
        [
            (10, 20, 30),
            (-5, -3, -8),
            (42, 0, 42),
            (0, 0, 0),
            (-100, 100, 0),
            (1, -1, 0),
            (INT32_MAX, 0, INT32_MAX),
            (INT32_MIN, 0, INT32_MIN),
            (1234567, 7654321, 8888888),
            (-1000, 500, -500),
        ],
        ids=[
            "positive",
            "negative",
            "positive_zero",
            "zero_zero",
            "neg_pos_cancel",
            "pos_neg_cancel",
            "int32_max",
            "int32_min",
            "large_numbers",
            "mixed_signs",
        ],
    )
    def test_add(self, eco_test: Ptr[IEcoTest], left: int, right: int, expected: int) -> None:
        """Verifies `Addition` across typical int32 inputs."""
        result = eco_test.obj.Addition(left, right)
        assert result == expected

    @pytest.mark.parametrize(
        ["left", "right", "expected"],
        [
            (INT32_MAX, 1, INT32_MIN),
            (INT32_MIN, -1, INT32_MAX),
            (INT32_MAX, INT32_MAX, -2),
        ],
        ids=[
            "pos_overflow",
            "neg_overflow",
            "max_max",
        ],
    )
    def test_add_overflow_wraps(
        self, eco_test: Ptr[IEcoTest], left: int, right: int, expected: int
    ) -> None:
        """Verifies int32 two's-complement wraparound on overflow."""
        result = eco_test.obj.Addition(left, right)
        assert result == expected

    @pytest.mark.parametrize(
        ["value", "expected"],
        [
            (42, -42),
            (-7, 7),
            (0, 0),
            (1, -1),
            (-1, 1),
            (INT32_MAX, -INT32_MAX),
            (123456789, -123456789),
        ],
        ids=[
            "positive",
            "negative",
            "zero",
            "one",
            "neg_one",
            "int_max",
            "large_positive",
        ],
    )
    def test_negate(self, eco_test: Ptr[IEcoTest], value: int, expected: int) -> None:
        """Verifies `Negation` across typical int32 inputs."""
        result = eco_test.obj.Negation(value)
        assert result == expected


@pytest.mark.integration
class TestEcoTestFloatingPoint:
    """Tests for floating-point arithmetic."""

    @pytest.mark.parametrize(
        ["left", "right", "expected"],
        [
            (3.0, 4.0, 12.0),
            (123.456, 0.0, 0.0),
            (0.0, 123.456, 0.0),
            (-2.5, 4.0, -10.0),
            (-2.5, -4.0, 10.0),
            (0.1, 0.2, 0.02),
            (1.0, 1.0, 1.0),
            (1e10, 1e-10, 1.0),
            (1.5, 2.5, 3.75),
            (-1.0, 0.0, 0.0),
        ],
        ids=[
            "positive",
            "pos_zero",
            "zero_pos",
            "neg_pos",
            "neg_neg",
            "small_decimals",
            "one_one",
            "large_small",
            "fractional",
            "neg_zero",
        ],
    )
    def test_scalar_mul(
        self, eco_test: Ptr[IEcoTest], left: float, right: float, expected: float
    ) -> None:
        """Verifies `Multiplication` of two doubles."""
        result = eco_test.obj.Multiplication(left, right)
        assert math.isclose(result, expected, abs_tol=1e-9)

    def test_scalar_mul_large_values(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies multiplication of large double values."""
        result = eco_test.obj.Multiplication(1e150, 1e150)
        assert math.isclose(result, 1e300, rel_tol=1e-9)

    def test_scalar_mul_infinity(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies multiplication producing infinity via overflow."""
        result = eco_test.obj.Multiplication(1e200, 1e200)
        assert math.isinf(result)


@pytest.mark.integration
class TestEcoTestArrays:
    """Tests for array operations via pointers."""

    @pytest.mark.parametrize(
        ["values", "expected"],
        [
            ((1, 2, 3, 4, 5), 15),
            ((42,), 42),
            ((-10, 20, -30, 40), 20),
            ((0, 0, 0), 0),
            ((-1, -2, -3, -4), -10),
            ((100, 200, 300), 600),
            ((1, -1, 2, -2, 3, -3), 0),
        ],
        ids=[
            "positive",
            "single_element",
            "mixed_signs",
            "all_zero",
            "all_negative",
            "large_values",
            "alternating_signs",
        ],
    )
    def test_sum_array(
        self, eco_test: Ptr[IEcoTest], values: tuple[int, ...], expected: int
    ) -> None:
        """Verifies summation of an integer array."""
        n = len(values)
        arr = Array[Int32, n](*values)
        result = eco_test.obj.SumArray(arr, n)
        assert result == expected

    def test_sum_array_large(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies summation of a large array (1..1000)."""
        n = 1000
        arr = Array[Int32, n](*range(1, n + 1))
        result = eco_test.obj.SumArray(arr, n)
        assert result == n * (n + 1) // 2

    @pytest.mark.parametrize(
        ["size", "fill_value"],
        [
            (4, 99),
            (3, -1),
            (1, 0),
            (10, INT32_MAX),
            (10, INT32_MIN),
            (5, 1),
        ],
        ids=[
            "small_positive",
            "small_negative",
            "single_element",
            "large_positive",
            "large_negative",
            "medium",
        ],
    )
    def test_fill_array(self, eco_test: Ptr[IEcoTest], size: int, fill_value: int) -> None:
        """Verifies filling an array with a constant value."""
        arr = Array[Int32, size](*([0] * size))
        result = eco_test.obj.FillArray(arr, size, fill_value)
        assert result == EcoErrorCode.SUCCESS
        assert list(arr) == [fill_value] * size

    def test_fill_then_sum(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `FillArray` + `SumArray` pipeline."""
        arr = Array[Int32, 5](0, 0, 0, 0, 0)
        eco_test.obj.FillArray(arr, 5, 10)
        result = eco_test.obj.SumArray(arr, 5)
        assert result == 50

    def test_fill_overwrites_existing(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `FillArray` overwrites any prior content."""
        arr = Array[Int32, 4](1, 2, 3, 4)
        eco_test.obj.FillArray(arr, 4, 7)
        assert list(arr) == [7, 7, 7, 7]

    def test_sum_partial_array(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `SumArray` respects the size parameter (sums a prefix)."""
        arr = Array[Int32, 5](1, 2, 3, 4, 5)
        result = eco_test.obj.SumArray(arr, 3)
        assert result == 6  # 1 + 2 + 3


@pytest.mark.integration
class TestEcoTestBubbleSort:
    """Tests for in-place bubble sort."""

    @pytest.mark.parametrize(
        ["values", "expected"],
        [
            ((5, 3, 1, 4, 2), [1, 2, 3, 4, 5]),
            ((1, 2, 3, 4), [1, 2, 3, 4]),
            ((5, 4, 3, 2, 1), [1, 2, 3, 4, 5]),
            ((42,), [42]),
            ((3, 1, 4, 1, 5, 3), [1, 1, 3, 3, 4, 5]),
            ((10, -5, 0, -3), [-5, -3, 0, 10]),
            ((2, 1), [1, 2]),
            ((1, 1, 1, 1), [1, 1, 1, 1]),
            ((0, -1, 1, -2, 2), [-2, -1, 0, 1, 2]),
            ((INT32_MAX, INT32_MIN, 0), [INT32_MIN, 0, INT32_MAX]),
        ],
    )
    def test_sort(
        self, eco_test: Ptr[IEcoTest], values: tuple[int, ...], expected: list[int]
    ) -> None:
        """Verifies sorting of various input arrays."""
        n = len(values)
        arr = Array[Int32, n](*values)
        result = eco_test.obj.SortArray(arr, n)
        assert result == EcoErrorCode.SUCCESS
        assert list(arr) == expected

    def test_sort_large_reversed(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies sorting a reverse-ordered array of 100 elements."""
        values = list(range(100, 0, -1))
        arr = Array[Int32, 100](*values)
        eco_test.obj.SortArray(arr, 100)
        assert list(arr) == list(range(1, 101))

    def test_sort_idempotent(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies sorting twice yields the same result."""
        arr = Array[Int32, 6](3, 1, 4, 1, 5, 9)
        eco_test.obj.SortArray(arr, 6)
        first = list(arr)
        eco_test.obj.SortArray(arr, 6)
        assert list(arr) == first

    def test_sort_preserves_length(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies the array length is unchanged after sorting."""
        arr = Array[Int32, 7](5, 3, 8, 1, 9, 2, 7)
        eco_test.obj.SortArray(arr, 7)
        assert len(list(arr)) == 7


@pytest.mark.integration
class TestEcoTestStrings:
    """Tests for C string return values."""

    def test_get_name_value(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `GetName` returns 'EcoTest'."""
        name = eco_test.obj.GetName()
        assert name == b"EcoTest"

    def test_get_name_stable_across_calls(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `GetName` returns the same value on repeated calls."""
        first = eco_test.obj.GetName()
        second = eco_test.obj.GetName()
        assert first == second == b"EcoTest"


@pytest.mark.integration
class TestEcoTestStructures:
    """Tests for C structure interop via the EcoPoint type."""

    def test_make_point_basic(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `MakePoint` writes the given coordinates into the out-param."""
        point = EcoPoint()
        result = eco_test.obj.MakePoint(3, 4, byref(point))
        assert result == EcoErrorCode.SUCCESS
        assert (point.x, point.y) == (3, 4)

    @pytest.mark.parametrize(
        ["x", "y"],
        [
            (0, 0),
            (1, 2),
            (-5, 7),
            (INT32_MAX, INT32_MIN),
            (100, -100),
        ],
        ids=["origin", "positive", "mixed_signs", "boundary", "symmetric"],
    )
    def test_make_point_parametrized(self, eco_test: Ptr[IEcoTest], x: int, y: int) -> None:
        """Verifies MakePoint preserves arbitrary coordinate pairs."""
        point = EcoPoint()
        eco_test.obj.MakePoint(x, y, byref(point))
        assert (point.x, point.y) == (x, y)

    def test_make_point_overwrites_existing(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `MakePoint` overwrites any prior content of the out-param."""
        point = EcoPoint(999, -999)
        eco_test.obj.MakePoint(1, 2, byref(point))
        assert (point.x, point.y) == (1, 2)

    @pytest.mark.parametrize(
        ["point_a", "point_b", "expected"],
        [
            ((0, 0), (0, 0), 0),
            ((0, 0), (3, 4), 25),
            ((1, 1), (4, 5), 25),
            ((-1, -1), (1, 1), 8),
            ((10, 0), (0, 0), 100),
            ((0, 10), (0, -10), 400),
        ],
        ids=["same_origin", "3_4_5", "shifted", "across_origin", "x_only", "y_only"],
    )
    def test_calculate_distance(
        self,
        eco_test: Ptr[IEcoTest],
        point_a: tuple[int, int],
        point_b: tuple[int, int],
        expected: int,
    ) -> None:
        """Verifies `CalculateDistance` computes squared Euclidean distance."""
        pa = EcoPoint(*point_a)
        pb = EcoPoint(*point_b)
        result = eco_test.obj.CalculateDistance(byref(pa), byref(pb))
        assert result == expected

    def test_calculate_distance_symmetric(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies distance(a, b) == distance(b, a)."""
        point_a = EcoPoint(2, 5)
        point_b = EcoPoint(7, 1)
        dist_a_to_b = eco_test.obj.CalculateDistance(byref(point_a), byref(point_b))
        dist_b_to_a = eco_test.obj.CalculateDistance(byref(point_b), byref(point_a))
        assert dist_a_to_b == dist_b_to_a

    def test_calculate_distance_does_not_mutate_inputs(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `CalculateDistance` leaves its input points untouched."""
        point_a = EcoPoint(2, 5)
        point_b = EcoPoint(7, 1)
        eco_test.obj.CalculateDistance(byref(point_a), byref(point_b))
        assert (point_a.x, point_a.y) == (2, 5)
        assert (point_b.x, point_b.y) == (7, 1)

    @pytest.mark.parametrize(
        ["start", "delta", "expected"],
        [
            ((0, 0), (5, 5), (5, 5)),
            ((10, 20), (-3, -4), (7, 16)),
            ((1, 1), (0, 0), (1, 1)),
            ((-5, -5), (5, 5), (0, 0)),
        ],
        ids=["from_origin", "negative_delta", "no_op", "back_to_origin"],
    )
    def test_translate_in_place(
        self,
        eco_test: Ptr[IEcoTest],
        start: tuple[int, int],
        delta: tuple[int, int],
        expected: tuple[int, int],
    ) -> None:
        """Verifies `Translate` mutates the structure in place."""
        point = EcoPoint(*start)
        dx, dy = delta
        result = eco_test.obj.Translate(byref(point), dx, dy)
        assert result == EcoErrorCode.SUCCESS
        assert (point.x, point.y) == expected

    def test_translate_chained(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies sequential translations accumulate."""
        point = EcoPoint(0, 0)
        eco_test.obj.Translate(byref(point), 1, 2)
        eco_test.obj.Translate(byref(point), 3, 4)
        eco_test.obj.Translate(byref(point), -2, -1)
        assert (point.x, point.y) == (2, 5)

    def test_make_point_then_translate(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `MakePoint` + `Translate` pipeline."""
        point = EcoPoint()
        eco_test.obj.MakePoint(10, 10, byref(point))
        eco_test.obj.Translate(byref(point), 5, -3)
        assert (point.x, point.y) == (15, 7)

    def test_make_point_then_distance(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `MakePoint` + `CalculateDistance` pipeline."""
        point_a = EcoPoint()
        point_b = EcoPoint()
        eco_test.obj.MakePoint(0, 0, byref(point_a))
        eco_test.obj.MakePoint(6, 8, byref(point_b))
        result = eco_test.obj.CalculateDistance(byref(point_a), byref(point_b))
        assert result == 100

    def test_struct_equality(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies model `__eq__` works on structures produced by `MakePoint`."""
        point_a = EcoPoint()
        point_b = EcoPoint()
        eco_test.obj.MakePoint(7, 9, byref(point_a))
        eco_test.obj.MakePoint(7, 9, byref(point_b))
        assert point_a == point_b


@pytest.mark.integration
class TestEcoTestUnions:
    """Tests for `EcoValue` union — by value and by pointer."""

    @pytest.mark.parametrize(
        "value",
        [0, 1, -1, 42, INT32_MAX, INT32_MIN, 123456789],
        ids=["zero", "one", "neg_one", "positive", "int_max", "int_min", "large"],
    )
    def test_pack_int_roundtrip(self, eco_test: Ptr[IEcoTest], value: int) -> None:
        """`PackInt` + `UnpackInt` roundtrip preserves the int view."""
        out = EcoValue()
        result = eco_test.obj.PackInt(value, byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert eco_test.obj.UnpackInt(byref(out)) == value

    @pytest.mark.parametrize(
        "value",
        [0.0, 1.5, -2.75, 3.1415926535, 1e100, -1e-100],
        ids=["zero", "positive", "negative", "pi_like", "huge", "tiny"],
    )
    def test_pack_double_roundtrip(self, eco_test: Ptr[IEcoTest], value: float) -> None:
        """`PackDouble` + `UnpackDouble` roundtrip preserves the double view."""
        out = EcoValue()
        result = eco_test.obj.PackDouble(value, byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert math.isclose(eco_test.obj.UnpackDouble(byref(out)), value, rel_tol=0, abs_tol=0)

    def test_union_by_pointer_param(self, eco_test: Ptr[IEcoTest]) -> None:
        """Union passed by pointer is read correctly on the C side."""
        value = EcoValue()
        value.as_int = Int32(777)
        assert eco_test.obj.UnpackInt(byref(value)) == 777

    def test_reinterpret_int_halves(self, eco_test: Ptr[IEcoTest]) -> None:
        """`ReinterpretAsInt` returns XOR of both int halves of the union."""
        value = EcoValue()
        value.as_bytes[0] = Int32(0xA5A5A5A5 - 0x100000000)  # signed int32 pattern
        value.as_bytes[1] = Int32(0x5A5A5A5A)
        expected = value.as_bytes[0] ^ value.as_bytes[1]
        assert eco_test.obj.ReinterpretAsInt(byref(value)) == expected

    def test_int_and_double_views_share_storage(self, eco_test: Ptr[IEcoTest]) -> None:
        """Writing via `PackDouble` changes the int view (shared storage)."""
        out = EcoValue()
        eco_test.obj.PackInt(0, byref(out))
        assert eco_test.obj.UnpackInt(byref(out)) == 0

        eco_test.obj.PackDouble(1.1, byref(out))
        # 1.1 as IEEE-754 double has non-zero bits in both int halves
        assert eco_test.obj.UnpackInt(byref(out)) != 0


@pytest.mark.integration
class TestEcoTestNestedStructures:
    """Tests for `EcoRect` — a model containing two nested `EcoPoint`s."""

    def test_make_rect_from_points_by_pointer(self, eco_test: Ptr[IEcoTest]) -> None:
        """`MakeRect` accepts two `EcoPoint`s by pointer and writes into out-param."""
        rect = EcoRect()
        top_left = EcoPoint(1, 2)
        bottom_right = EcoPoint(10, 20)
        result = eco_test.obj.MakeRect(byref(top_left), byref(bottom_right), byref(rect))
        assert result == EcoErrorCode.SUCCESS
        assert rect.top_left == EcoPoint(1, 2)
        assert rect.bottom_right == EcoPoint(10, 20)

    @pytest.mark.parametrize(
        ["top_left", "bottom_right", "expected"],
        [
            ((0, 0), (10, 5), 50),
            ((0, 0), (0, 0), 0),
            ((-5, -5), (5, 5), 100),
            ((10, 10), (0, 0), 100),
            ((1, 1), (2, 2), 1),
        ],
        ids=["simple", "degenerate", "centered", "reversed", "unit"],
    )
    def test_rect_area(
        self,
        eco_test: Ptr[IEcoTest],
        top_left: tuple[int, int],
        bottom_right: tuple[int, int],
        expected: int,
    ) -> None:
        """`RectArea` returns |dx| * |dy|."""
        rect = EcoRect()
        eco_test.obj.MakeRect(EcoPoint(*top_left), EcoPoint(*bottom_right), byref(rect))
        assert eco_test.obj.RectArea(byref(rect)) == expected

    def test_rect_nested_field_access(self, eco_test: Ptr[IEcoTest]) -> None:
        """Nested `EcoPoint` fields on `EcoRect` are accessed through dotted path."""
        rect = EcoRect()
        eco_test.obj.MakeRect(EcoPoint(3, 4), EcoPoint(7, 8), byref(rect))
        assert (rect.top_left.x, rect.top_left.y) == (3, 4)
        assert (rect.bottom_right.x, rect.bottom_right.y) == (7, 8)


@pytest.mark.integration
class TestEcoTestArrayOfStructures:
    """Tests for `SumPoints` — array of `EcoPoint` passed as a pointer."""

    def test_sum_points_single(self, eco_test: Ptr[IEcoTest]) -> None:
        """A one-element array is summed correctly."""
        points = Array[EcoPoint, 1](EcoPoint(5, 7))
        out = EcoPoint()
        result = eco_test.obj.SumPoints(points, 1, byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert out == EcoPoint(5, 7)

    def test_sum_points_multiple(self, eco_test: Ptr[IEcoTest]) -> None:
        """Componentwise sum of several points."""
        points = Array[EcoPoint, 4](
            EcoPoint(1, 2),
            EcoPoint(3, 4),
            EcoPoint(-5, 6),
            EcoPoint(10, -20),
        )
        out = EcoPoint()
        eco_test.obj.SumPoints(points, 4, byref(out))
        assert out == EcoPoint(9, -8)

    def test_sum_points_respects_size(self, eco_test: Ptr[IEcoTest]) -> None:
        """`SumPoints` sums only the first `size` elements."""
        points = Array[EcoPoint, 3](EcoPoint(1, 1), EcoPoint(2, 2), EcoPoint(100, 100))
        out = EcoPoint()
        eco_test.obj.SumPoints(points, 2, byref(out))
        assert out == EcoPoint(3, 3)


@pytest.mark.integration
class TestEcoTestVariant:
    """Tests for `EcoVariant` — a model containing a union by value."""

    def test_make_variant_int(self, eco_test: Ptr[IEcoTest]) -> None:
        """`MakeVariant` tags and stores an int value."""
        value = EcoValue()
        value.as_int = Int32(42)
        variant = EcoVariant()
        result = eco_test.obj.MakeVariant(ECO_VALUE_INT, byref(value), byref(variant))
        assert result == EcoErrorCode.SUCCESS
        assert variant.tag == ECO_VALUE_INT
        assert variant.value.as_int == 42

    def test_make_variant_double(self, eco_test: Ptr[IEcoTest]) -> None:
        """`MakeVariant` tags and stores a double value."""
        value = EcoValue()
        value.as_double = Double(3.14)
        variant = EcoVariant()
        eco_test.obj.MakeVariant(ECO_VALUE_DOUBLE, byref(value), byref(variant))
        assert variant.tag == ECO_VALUE_DOUBLE
        assert math.isclose(variant.value.as_double, 3.14, abs_tol=1e-9)

    def test_get_variant_tag(self, eco_test: Ptr[IEcoTest]) -> None:
        """`GetVariantTag` returns the stored tag."""
        value = EcoValue()
        value.as_int = Int32()
        variant = EcoVariant()
        eco_test.obj.MakeVariant(ECO_VALUE_INT, byref(value), byref(variant))
        assert eco_test.obj.GetVariantTag(byref(variant)) == ECO_VALUE_INT

    def test_get_variant_value_roundtrip(self, eco_test: Ptr[IEcoTest]) -> None:
        """`GetVariantValue` copies the union half into an out-parameter."""
        value = EcoValue()
        value.as_int = Int32(999)
        variant = EcoVariant()
        eco_test.obj.MakeVariant(ECO_VALUE_INT, byref(value), byref(variant))

        out = EcoValue()
        result = eco_test.obj.GetVariantValue(byref(variant), byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert out.as_int == 999


@pytest.mark.integration
class TestEcoTestRefCounting:
    """Tests for `IEcoUnknown` reference counting."""

    def test_addref_increments(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `AddRef` increments the reference count."""
        ref1 = eco_test.obj.AddRef()
        ref2 = eco_test.obj.AddRef()
        assert ref2 == ref1 + 1

        # Balance the extra refs
        eco_test.obj.Release()
        eco_test.obj.Release()

    def test_release_decrements(self, eco_test: Ptr[IEcoTest]) -> None:
        """Verifies `Release` decrements the reference count."""
        eco_test.obj.AddRef()
        ref_before = eco_test.obj.AddRef()
        ref_after = eco_test.obj.Release()
        assert ref_after == ref_before - 1

        # Balance the extra ref
        eco_test.obj.Release()

    @pytest.mark.parametrize("n", [1, 3, 5, 10])
    def test_addref_release_balanced(self, eco_test: Ptr[IEcoTest], n: int) -> None:
        """Verifies N `AddRef` calls balanced by N `Release` calls return to baseline."""
        baseline = eco_test.obj.AddRef()
        eco_test.obj.Release()

        for _ in range(n):
            eco_test.obj.AddRef()
        last = None
        for _ in range(n):
            last = eco_test.obj.Release()

        assert last is not None
        assert last == baseline - 1


@pytest.mark.integration
class TestEcoTestQueryInterface:
    """Tests for `QueryInterface` — IID dispatch and refcount behavior."""

    def test_query_self_iid_returns_component(self, eco_test: Ptr[IEcoTest]) -> None:
        """`QueryInterface(IID_IEcoTest)` returns a usable `IEcoTest` pointer."""
        out = Ptr[Void]()
        result = eco_test.obj.QueryInterface(byref(IID_IEcoTest), byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert out.value

        queried = cast(out, Ptr[IEcoTest])
        # The queried pointer must be usable as an `IEcoTest`.
        assert queried.obj.Addition(2, 3) == 5

        # Balance the refcount bump from QueryInterface
        queried.obj.Release()

    def test_query_iunknown_iid_returns_component(self, eco_test: Ptr[IEcoTest]) -> None:
        """`QueryInterface(IID_IEcoUnknown)` returns a usable `IEcoUnknown` pointer."""
        out = Ptr[Void]()
        result = eco_test.obj.QueryInterface(byref(IID_IEcoUnknown), byref(out))
        assert result == EcoErrorCode.SUCCESS
        assert out.value

        unknown = cast(out, Ptr[IEcoUnknown])
        # Balance the refcount bump from QueryInterface
        unknown.obj.Release()

    def test_query_unknown_iid_returns_error(self, eco_test: Ptr[IEcoTest]) -> None:
        """`QueryInterface` with an unrelated IID returns `NOINTERFACE`."""
        bogus_iid = UGUID("DEADBEEF-DEAD-BEEF-DEAD-BEEFDEADBEEF")
        out = Ptr[Void]()
        result = eco_test.obj.QueryInterface(byref(bogus_iid), byref(out))
        assert result & 0xFFFF == EcoErrorCode.NOINTERFACE
        assert not out.value

    def test_query_interface_increments_refcount(self, eco_test: Ptr[IEcoTest]) -> None:
        """A successful `QueryInterface` bumps the reference count."""
        before = eco_test.obj.AddRef()
        eco_test.obj.Release()

        out = Ptr[Void]()
        eco_test.obj.QueryInterface(byref(IID_IEcoTest), byref(out))

        after = eco_test.obj.AddRef()
        eco_test.obj.Release()

        assert after == before + 1

        # Balance the ref from QueryInterface
        cast(out, Ptr[IEcoTest]).obj.Release()


@pytest.mark.integration
class TestEcoTestCallback:
    """Tests for `SortArrayWith` — user-supplied comparator callback."""

    def test_sort_ascending_via_callback(self, eco_test: Ptr[IEcoTest]) -> None:
        """A Python comparator can drive sort order ascending."""

        def ascending(a: int, b: int) -> int:
            return a - b

        cmp = EcoCompareFunc(ascending)
        arr = Array[Int32, 5](5, 3, 1, 4, 2)
        result = eco_test.obj.SortArrayWith(arr, 5, cmp)
        assert result == EcoErrorCode.SUCCESS
        assert list(arr) == [1, 2, 3, 4, 5]

    def test_sort_descending_via_callback(self, eco_test: Ptr[IEcoTest]) -> None:
        """A Python comparator can drive sort order descending."""

        def descending(a: int, b: int) -> int:
            return b - a

        cmp = EcoCompareFunc(descending)
        arr = Array[Int32, 5](5, 3, 1, 4, 2)
        eco_test.obj.SortArrayWith(arr, 5, cmp)
        assert list(arr) == [5, 4, 3, 2, 1]

    def test_callback_is_invoked(self, eco_test: Ptr[IEcoTest]) -> None:
        """The comparator callback is actually invoked from C-side."""
        calls: list[tuple[int, int]] = []

        def tracking(a: int, b: int) -> int:
            calls.append((a, b))
            return a - b

        cmp = EcoCompareFunc(tracking)
        arr = Array[Int32, 4](4, 3, 2, 1)
        eco_test.obj.SortArrayWith(arr, 4, cmp)
        assert len(calls) > 0
        assert list(arr) == [1, 2, 3, 4]


@pytest.mark.integration
class TestEcoTestStringParam:
    """Tests for `StringLength` — C string passed as an in-parameter."""

    @pytest.mark.parametrize(
        ["string", "expected"],
        [
            (b"", 0),
            (b"a", 1),
            (b"hello", 5),
            (b"EcoTest", 7),
            (b"x" * 100, 100),
            (b"abc\0xyz", 3),
        ],
        ids=["empty", "one_char", "short", "word", "long", "trail"],
    )
    def test_string_length(self, eco_test: Ptr[IEcoTest], string: bytes, expected: int) -> None:
        """`StringLength` returns the number of bytes before the NULL terminator."""
        result = eco_test.obj.StringLength(string)
        assert result == expected
