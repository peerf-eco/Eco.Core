"""Unit tests for `eco_python2acom.types.pointer` module.

This module tests the generic `Ptr[T]` smart pointer type including `Ptr[Void]`,
typed pointers, NULL handling, equality, and caching.

Test Classes:
    TestPtrVoid: Tests for `Ptr[Void]` behavior.
    TestPtrTyped: Tests for typed pointers like `Ptr[Int32]`.
    TestPtrNull: Tests for NULL pointer behavior.
    TestPtrRepr: Tests for `__repr__` output.
    TestPtrEquality: Tests for `__eq__` (address-based).
    TestPtrCaching: Tests for metaclass type caching.
    TestPtrValidation: Tests for invalid type parameters.
"""

import pytest

from eco_python2acom.types.core import Int32, UInt8, Void
from eco_python2acom.types.pointer import Ptr


@pytest.mark.unit
class TestPtrVoid:
    """Tests for `Ptr[Void]` (void pointer).

    Verifies NULL creation, address assignment, bool behavior, and absence of `.obj`.
    """

    def test_null_pointer(self) -> None:
        """Verifies default `Ptr[Void]` is NULL."""
        ptr = Ptr[Void]()
        assert ptr.value is None

    def test_pointer_with_address(self) -> None:
        """Verifies `Ptr[Void]` stores an integer address."""
        ptr = Ptr[Void](0xDEADBEEF)
        assert ptr.value == 0xDEADBEEF

    def test_set_value(self) -> None:
        """Verifies `.value` can be reassigned."""
        ptr = Ptr[Void]()
        ptr.value = 0x12345678
        assert ptr.value == 0x12345678

    def test_set_value_to_none(self) -> None:
        """Verifies `.value` can be set back to None (NULL)."""
        ptr = Ptr[Void](0x1234)
        ptr.value = None
        assert ptr.value is None

    def test_bool_null_is_false(self) -> None:
        """Verifies NULL `Ptr[Void]` is false."""
        ptr = Ptr[Void]()
        assert not bool(ptr)

    def test_bool_non_null_is_true(self) -> None:
        """Verifies non-NULL `Ptr[Void]` is true."""
        ptr = Ptr[Void](0x1)
        assert bool(ptr)

    def test_no_obj_attribute(self) -> None:
        """Verifies `Ptr[Void]` has no `.obj` (void has no size)."""
        ptr = Ptr[Void](0x1234)
        assert not hasattr(ptr, "obj")


@pytest.mark.unit
class TestPtrTyped:
    """Tests for typed pointers like `Ptr[Int32]`.

    Verifies instantiation, dereferencing via `.obj`, value wrapping, and bool behavior.
    """

    def test_null_dereference_raises(self) -> None:
        """Verifies dereferencing NULL `Ptr[Int32]` raises `ValueError`."""
        with pytest.raises(ValueError, match="pointer access"):
            _ = Ptr[Int32]().obj

    def test_no_value_attribute(self) -> None:
        """Verifies typed pointer has no `.value`."""
        ptr = Ptr[Int32](42)
        assert not hasattr(ptr, "value")

    def test_bool_null_is_false(self) -> None:
        """Verifies NULL `Ptr[Int32]` is false."""
        ptr = Ptr[Int32]()
        assert not bool(ptr)

    def test_bool_non_null_is_true(self) -> None:
        """Verifies non-NULL `Ptr[Int32]` is true."""
        ptr = Ptr[Int32](42)
        assert bool(ptr)

    def test_create_from_value(self) -> None:
        """Verifies `Ptr[Int32]` wraps a value and provides `.obj`."""
        value = Int32(42)
        ptr = Ptr[Int32](value)
        assert ptr.obj.value == 42

    def test_create_from_int(self) -> None:
        """Verifies `Ptr[Int32]` auto-wraps a plain int."""
        ptr = Ptr[Int32](42)
        assert ptr.obj.value == 42

    def test_modify_obj(self) -> None:
        """Verifies `.obj` value can be modified."""
        ptr = Ptr[Int32](10)
        ptr.obj = Int32(99)
        assert ptr.obj.value == 99

    def test_invalid_value_raises(self) -> None:
        """Verifies unconvertible value raises `ValueError`."""
        with pytest.raises(ValueError, match="not initialize"):
            Ptr[Int32]("not_an_int")


@pytest.mark.unit
class TestPtrNull:
    """Tests for NULL pointer behavior."""

    @pytest.mark.parametrize("ptr_type", [Int32, Void], ids=["int32", "void"])
    def test_null_pointers_are_equal(self, ptr_type) -> None:
        """NULL pointers of the same type should be equal."""
        ptr1 = Ptr[ptr_type]()
        ptr2 = Ptr[ptr_type]()
        assert ptr1 == ptr2

    @pytest.mark.parametrize(
        ["ptr_type", "value"], [(Int32, 42), (Void, 0x1234)], ids=["int32", "void"]
    )
    def test_null_not_equal_to_non_null(self, ptr_type, value) -> None:
        """NULL pointer should not equal non-NULL pointer."""
        null_ptr = Ptr[ptr_type]()
        non_null_ptr = Ptr[ptr_type](value)
        assert null_ptr != non_null_ptr


@pytest.mark.unit
class TestPtrRepr:
    """Tests for Ptr `__repr__` output.

    Verifies representation format for NULL and non-NULL pointers.
    """

    def test_void_null_repr(self) -> None:
        """Verifies `Ptr[Void]` NULL repr format."""
        ptr = Ptr[Void]()
        assert repr(ptr) == "<Ptr[Void] NULL>"

    def test_void_non_null_repr(self) -> None:
        """Verifies `Ptr[Void]` non-NULL repr includes hex address."""
        ptr = Ptr[Void](0xABCD)
        repr_str = repr(ptr)
        assert "Ptr[Void]" in repr_str
        assert "0x" in repr_str

    def test_typed_null_repr(self) -> None:
        """Verifies typed pointer NULL repr format."""
        ptr = Ptr[Int32]()
        assert repr(ptr) == "<Ptr[Int32] NULL>"

    def test_typed_non_null_repr(self) -> None:
        """Verifies typed pointer non-NULL repr includes hex address."""
        ptr = Ptr[Int32](42)
        repr_str = repr(ptr)
        assert "Ptr[Int32]" in repr_str
        assert "0x" in repr_str


@pytest.mark.unit
class TestPtrEquality:
    """Tests for pointer equality.

    Verifies `__eq__` for both `Ptr[Void]` and typed pointers.
    """

    def test_void_pointers_equal_same_address(self) -> None:
        """Verifies `Ptr[Void]` equality by address."""
        ptr1 = Ptr[Void](0x1234)
        ptr2 = Ptr[Void](0x1234)
        assert ptr1 == ptr2

    def test_void_pointers_different_address(self) -> None:
        """Verifies `Ptr[Void]` inequality for different addresses."""
        ptr1 = Ptr[Void](0x1234)
        ptr2 = Ptr[Void](0x5678)
        assert ptr1 != ptr2

    def test_typed_pointers_equal_same_object(self) -> None:
        """Verifies typed pointers to the same object are equal."""
        value = Int32(42)
        ptr1 = Ptr[Int32](value)
        ptr2 = Ptr[Int32](value)
        assert ptr1 == ptr2

    def test_typed_pointers_different_objects_not_equal(self) -> None:
        """Verifies typed pointers to different objects are not equal."""
        ptr1 = Ptr[Int32](42)
        ptr2 = Ptr[Int32](42)
        assert ptr1 != ptr2

    @pytest.mark.parametrize("ptr_type", [Void, Int32], ids=["void", "int32"])
    @pytest.mark.parametrize("other", ["pointer", 0x1234, None], ids=["str", "int", "none"])
    def test_pointer_not_equal_to_other_types(self, ptr_type, other: object) -> None:
        """Pointers should not be equal to objects of other types."""
        ptr = Ptr[ptr_type](0x1234)
        assert ptr != other


@pytest.mark.unit
class TestPtrCaching:
    """Tests for metaclass type caching.

    Verifies that Ptr[T] returns the same type for identical parameters.
    """

    def test_same_type_returns_same_class(self) -> None:
        """Verifies cache hit for identical type parameter."""
        type1 = Ptr[Int32]
        type2 = Ptr[Int32]
        assert type1 is type2

    def test_void_cached(self) -> None:
        """Verifies `Ptr[Void]` is cached."""
        type1 = Ptr[Void]
        type2 = Ptr[Void]
        assert type1 is type2

    def test_different_types_return_different_classes(self) -> None:
        """Verifies different type parameters produce different classes."""
        assert Ptr[Int32] is not Ptr[UInt8]
        assert Ptr[Int32] is not Ptr[Void]


@pytest.mark.unit
class TestPtrValidation:
    """Tests for invalid `Ptr` type parameters.

    Verifies error handling for non-pointable types.
    """

    def test_non_type_raises(self) -> None:
        """Verifies non-type parameter raises `TypeError`."""
        with pytest.raises(TypeError, match="must be a type"):
            Ptr["not_a_type"]  # type: ignore

    def test_int_raises(self) -> None:
        """Verifies plain Python type raises `ValueError`."""
        with pytest.raises(ValueError, match="not create pointer"):
            Ptr[int]
