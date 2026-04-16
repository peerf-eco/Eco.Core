"""Unit tests for the `@union` decorator.

Verifies that `@union` correctly converts annotated classes into C-compatible
unions: field resolution, byte layout/sizeof, inheritance, forward references,
ClassVar handling, Optional types, and error reporting for invalid annotations.
"""

from __future__ import annotations

from typing import ClassVar, Optional

import pytest

from eco_python2acom.decorators.layout import model, union
from eco_python2acom.decorators.utils import finalize
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CUnion, Double, Float, Int8, Int16, Int32, Int64, UInt8, Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import sizeof


class TestUnionBasicFields:
    """Verifies basic field resolution and `CUnion` compliance."""

    def test_simple_union_is_cunion(self) -> None:
        """A decorated union is a subclass of `CUnion`."""

        @union
        class Value:
            as_int: Int32
            as_float: Float

        assert issubclass(Value, CUnion)

    def test_field_names_and_types_preserved(self) -> None:
        """Field names and types are preserved as declared."""

        @union
        class Value:
            as_int: Int32
            as_float: Float

        assert Value._fields_ == [("as_int", Int32), ("as_float", Float)]

    def test_eco_union_marker(self) -> None:
        """Decorator sets the `_eco_union_` marker attribute."""

        @union
        class Union:
            value: Int32

        assert Union._eco_union_ is True

    def test_instance_field_access_via_first_field(self) -> None:
        """Instances allow access via any declared field."""

        @union
        class Value:
            as_int: Int8
            as_float: Float

        val = Value(42)
        assert val.as_int == 42


class TestUnionLayout:
    """Verifies byte layout — union size equals the largest member."""

    def test_size_equals_largest_member(self) -> None:
        """Union size equals max of all member sizes."""

        @union
        class Union:
            small: Int8
            big: Int64

        assert sizeof(Union) == sizeof(Int64)

    @pytest.mark.parametrize(
        ["field_types", "expected"],
        [
            ((Int8, Int8), 1),
            ((Int8, Int16), 2),
            ((Int16, Int32), 4),
            ((Int32, Int64), 8),
            ((Float, Double), 8),
        ],
        ids=["int8_int8", "int8_int16", "int16_int32", "int32_int64", "float_double"],
    )
    def test_two_field_sizes(self, field_types: tuple[type, type], expected: int) -> None:
        """Size equals the larger of the two member types."""
        a, b = field_types

        @union
        class Union:
            x: a  # type: ignore
            y: b  # type: ignore

        assert sizeof(Union) == expected

    def test_pointer_field_size(self) -> None:
        """Union with a pointer field is at least pointer-sized."""

        @union
        class Union:
            as_ptr: Ptr[Int32]
            as_int: Int32

        assert sizeof(Union) == sizeof(Ptr[Void])

    def test_array_field_size(self) -> None:
        """Union with an array field is sized by the largest member."""

        @union
        class Union:
            as_bytes: Array[UInt8, 8]
            as_int: Int32

        assert sizeof(Union) == sizeof(UInt8) * 8

    def test_union_of_pointers_to_models(self) -> None:
        """A union can carry alternative pointers to different models."""

        @model
        class Left:
            a: Int32

        @model
        class Right:
            b: Double

        @union
        class Either:
            as_left: Ptr[Left]
            as_right: Ptr[Right]

        assert sizeof(Either) == sizeof(Ptr[Void])

    def test_empty_union_instantiable(self) -> None:
        """A union with no fields is still instantiable."""

        @union
        class Empty:
            pass

        instance = Empty()
        assert isinstance(instance, CUnion)


class TestUnionClassVar:
    """Verifies `ClassVar` annotations are excluded from `_fields_`."""

    def test_classvar_is_excluded(self) -> None:
        """`ClassVar` annotations do not appear in `_fields_`."""

        @union
        class Union:
            CONSTANT: ClassVar[int] = 42
            value: Int32

        names = [name for name, _ in Union._fields_]
        assert names == ["value"]
        assert Union.CONSTANT == 42

    def test_classvar_only_union(self) -> None:
        """A union with only `ClassVar`s has no instance fields."""

        @union
        class OnlyClassVars:
            A: ClassVar[int] = 1
            B: ClassVar[str] = "x"

        assert OnlyClassVars._fields_ == []


class TestUnionOptional:
    """Verifies `Optional[T]` is normalized to `T`."""

    def test_optional_field_resolves_to_inner(self) -> None:
        """`Optional[T]` is treated as `T` after normalization."""

        @union
        class Union:
            value: Optional[Int32]

        assert Union._fields_ == [("value", Int32)]


class TestUnionInheritance:
    """Verifies union inheritance rules."""

    def test_inheritance_from_union_allowed(self) -> None:
        """A `@union` can inherit from another `@union`."""

        @union
        class Base:
            a: Int32

        @union
        class Derived(Base):
            b: Int16

        assert Derived._fields_ == [("b", Int16)]

    def test_inheritance_from_non_union_fails(self) -> None:
        """Inheritance from a non-Union base raises `TypeError`."""

        class NotAUnion:
            pass

        with pytest.raises(TypeError, match="invalid base"):

            @union
            class Bad(NotAUnion):
                x: Int32

    def test_multiple_inheritance_fails(self) -> None:
        """Multiple inheritance raises `TypeError`."""

        @union
        class A:
            x: Int32

        @union
        class B:
            y: Int32

        with pytest.raises(TypeError, match="multiple inheritance"):

            @union
            class C(A, B):
                z: Int32


class TestUnionForwardReferences:
    """Verifies forward references resolve correctly."""

    def test_string_annotation_resolves(self) -> None:
        """String-form annotations resolve correctly."""

        @union
        class Union:
            value: "Int32"

        assert Union._fields_ == [("value", Int32)]


class TestUnionInvalidAnnotations:
    """Verifies error reporting for unresolved or invalid annotations."""

    def test_unresolved_forward_ref_raises(self) -> None:
        """Unresolved forward references raise `TypeError` on finalization."""

        @union
        class Broken:
            value: "DoesNotExist"  # type: ignore

        with pytest.raises(TypeError, match="unresolved types"):
            Broken = finalize(Broken)

    def test_non_eco_type_raises(self) -> None:
        """Annotations that aren't EcoOS data types raise `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):

            @union
            class Bad:
                value: str

            Bad = finalize(Bad)

    def test_union_annotation_multiple_types_raises(self) -> None:
        """A typing union with more than one non-None type raises `TypeError`."""

        @union
        class BadUnion:
            value: Int32 | Int16

        with pytest.raises(TypeError, match="Invalid EcoOS union"):
            BadUnion = finalize(BadUnion)


class TestUnionRepr:
    """Verifies generated `__repr__` method."""

    def test_repr_includes_class_and_fields(self) -> None:
        """`__repr__` output includes the class name and field values."""

        @union
        class Union:
            value: Int32

        inst = Union(value=7)
        text = repr(inst)
        assert "Union" in text
        assert "value=" in text


class TestUnionLazyFinalization:
    """Verifies lazy-finalization behavior."""

    def test_fields_resolved_on_access(self) -> None:
        """`_fields_` access triggers finalization."""

        @union
        class Lazy:
            value: Int32

        assert Lazy.__dict__.get("_eco_ready_") is False
        _ = Lazy._fields_
        assert Lazy.__dict__.get("_eco_ready_") is True

    def test_fields_resolved_on_instance(self) -> None:
        """Instantiation triggers finalization."""

        @union
        class Lazy:
            value: Int32

        assert Lazy.__dict__.get("_eco_ready_") is False
        _ = Lazy()
        assert Lazy.__dict__.get("_eco_ready_") is True

    def test_fields_resolved_on_sizeof(self) -> None:
        """`sizeof` access triggers finalization."""

        @union
        class Lazy:
            value: Int32

        assert Lazy.__dict__.get("_eco_ready_") is False
        _ = sizeof(Lazy)
        assert Lazy.__dict__.get("_eco_ready_") is True


class TestUnionBodyValidation:
    """Verifies decorator-time validation of the union body."""

    def test_bare_assignment_rejected(self) -> None:
        """A bare attribute without annotation is rejected at decoration time."""

        with pytest.raises(TypeError, match="no type annotation"):

            @union
            class Bad:
                as_int: Int32
                MAGIC = 42

    def test_convenience_accessors_allowed(self) -> None:
        """Methods, classmethods, staticmethods, and properties are allowed."""

        @union
        class Value:
            as_int: Int32
            as_float: Float

            def as_hex(self) -> str:
                return hex(int(self.as_int))

            @classmethod
            def zero(cls) -> "Value":
                return cls()

            @staticmethod
            def label() -> str:
                return "value"

            @property
            def int_view(self) -> int:
                return int(self.as_int)

        instance = Value()
        instance.as_int = Int32(0x1F)
        assert instance.as_hex() == "0x1f"
        assert Value.zero().as_int == 0
        assert Value.label() == "value"
        assert instance.int_view == 0x1F
