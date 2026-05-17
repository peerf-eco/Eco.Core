"""Unit tests for `eco_python2acom.types.array` module.

This module tests the generic `Array[T, N]` type including metaclass subscript
syntax, element access, iteration, conversion, and error handling.

Test Classes:
    TestArrayCreation: Tests for `Array[T, N]` type creation and instantiation.
    TestArrayElementAccess: Tests for `__getitem__` and `__setitem__`.
    TestArrayIteration: Tests for `__iter__` and `__len__`.
    TestArrayConversion: Tests for `__bytes__` and `__repr__`.
    TestArrayValidation: Tests for type and parameter validation errors.
    TestArrayCaching: Tests for metaclass type caching.
"""

import pytest

from eco_python2acom.types.array import Array
from eco_python2acom.types.core import Int32, UInt8, UInt16, Void


@pytest.mark.unit
class TestArrayCreation:
    """Tests for `Array[T, N]` type creation and instantiation.

    Verifies subscript syntax, zero-initialization, and value initialization.
    """

    def test_create_array_type(self) -> None:
        """Verifies `Array[T, N]` returns a valid array type."""
        ArrayType = Array[UInt8, 4]
        assert ArrayType._element_type_ is UInt8
        assert ArrayType._size_ == 4

    def test_init_empty(self) -> None:
        """Verifies empty array is zero-initialized."""
        arr = Array[UInt8, 4]()
        assert len(arr) == 4
        assert all(arr[i] == 0 for i in range(4))

    def test_init_with_values(self) -> None:
        """Verifies array initialized with values stores them correctly."""
        arr = Array[Int32, 3](1, 5, 15)
        assert arr[0] == 1
        assert arr[1] == 5
        assert arr[2] == 15

    def test_init_partial_values(self) -> None:
        """Verifies partial initialization zero-fills remaining elements."""
        arr = Array[UInt8, 4](1, 2)
        assert arr[0] == 1
        assert arr[1] == 2
        assert arr[2] == 0
        assert arr[3] == 0

    @pytest.mark.parametrize("size", [1, 10, 10000], ids=["one", "ten", "large"])
    def test_array_sizes(self, size: int) -> None:
        """Verifies `Array[T, N]` can be created with various sizes."""
        arr = Array[UInt8, size]()
        assert len(arr) == size


@pytest.mark.unit
class TestArrayElementAccess:
    """Tests for element access via `__getitem__` and `__setitem__`.

    Verifies index access, slice access, and assignment.
    """

    def test_get_by_index(self) -> None:
        """Verifies individual element access by index."""
        arr = Array[Int32, 3](1, 2, 3)
        assert arr[0] == 1
        assert arr[2] == 3

    def test_get_by_negative_index(self) -> None:
        """Verifies negative index access wraps from the end."""
        arr = Array[Int32, 3](1, 2, 3)
        assert arr[-1] == 3
        assert arr[-3] == 1

    def test_set_by_index(self) -> None:
        """Verifies element assignment by index."""
        arr = Array[Int32, 3](0, 0, 0)
        arr[0] = 42
        assert arr[0] == 42

    def test_get_slice(self) -> None:
        """Verifies slice access returns a list of elements."""
        arr = Array[Int32, 5](1, 2, 3, 4, 5)
        sliced = arr[1:4]
        assert len(sliced) == 3
        assert sliced == [2, 3, 4]

    def test_set_slice(self) -> None:
        """Verifies slice assignment updates multiple elements."""
        arr = Array[UInt8, 4](0, 0, 0, 0)
        arr[0:2] = [10, 20]
        assert arr[0] == 10
        assert arr[1] == 20

    def test_index_out_of_range(self) -> None:
        """Verifies out-of-bounds index raises `IndexError`."""
        arr = Array[UInt8, 3](1, 2, 3)
        with pytest.raises(IndexError):
            arr[len(arr) + 1]


@pytest.mark.unit
class TestArrayIteration:
    """Tests for `__iter__` and `__len__`.

    Verifies iteration order and length reporting.
    """

    def test_iter_yields_all_elements(self) -> None:
        """Verifies `__iter__` yields all elements in order."""
        arr = Array[Int32, 4](1, 2, 3, 4)
        values = list(arr)
        assert len(values) == 4
        assert values[0] == 1
        assert values[-1] == 4

    def test_iter_with_for_loop(self) -> None:
        """Verifies array works in a for-loop."""
        arr = Array[UInt8, 3](1, 2, 3)
        total = 0
        for elem in arr:
            total += elem
        assert total == (1 + 2 + 3)


@pytest.mark.unit
class TestArrayConversion:
    """Tests for `__bytes__` and `__repr__`.

    Verifies byte conversion and string representation.
    """

    def test_bytes_conversion(self) -> None:
        """Verifies `__bytes__` returns correct byte representation."""
        arr = Array[UInt8, 4](0x01, 0x02, 0x03, 0x04)
        assert bytes(arr) == b"\x01\x02\x03\x04"

    def test_bytes_empty_array(self) -> None:
        """Verifies `__bytes__` on zero-initialized array."""
        arr = Array[UInt8, 3]()
        assert bytes(arr) == b"\x00\x00\x00"

    def test_repr_contains_array_prefix(self) -> None:
        """Verifies `__repr__` starts with '<Array['."""
        arr = Array[Int32, 5]()
        assert repr(arr).startswith("<Array[")


@pytest.mark.unit
class TestArrayValidation:
    """Tests for Array type parameter validation.

    Verifies error handling for invalid subscript arguments.
    """

    @pytest.mark.parametrize(
        ["params", "err", "match"],
        [
            ((UInt8,), ValueError, "2 parameters"),
            ((UInt8, -1), ValueError, "positive integer"),
            ((UInt8, 0), ValueError, "positive integer"),
            ((UInt8, "3"), ValueError, "positive integer"),
            (("not_a_type", 3), TypeError, "must be a type"),
            ((int, 5), ValueError, "not create array"),
            ((Void, 4), ValueError, "not create array"),
        ],
        ids=[
            "single_param",
            "negative_size",
            "zero_size",
            "non_int_size",
            "non_type_element",
            "not_valid_elem_type",
            "void_type_element",
        ],
    )
    def test_invalid_subscript_params(
        self, params: tuple, err: type[Exception], match: str
    ) -> None:
        """Verifies invalid subscript parameters raise the expected exception."""
        with pytest.raises(err, match=match):
            Array[params]


@pytest.mark.unit
class TestArrayCaching:
    """Tests for metaclass type caching.

    Verifies that `Array[T, N]` returns the same type for identical parameters.
    """

    def test_same_params_return_same_type(self) -> None:
        """Verifies cache hit for identical parameters."""
        type1 = Array[UInt8, 4]
        type2 = Array[UInt8, 4]
        assert type1 is type2

    def test_different_params_return_different_types(self) -> None:
        """Verifies different parameters produce different types."""
        type1 = Array[UInt8, 4]
        type2 = Array[UInt16, 4]
        type3 = Array[UInt8, 8]
        assert type1 is not type2
        assert type1 is not type3
