"""Unit tests for the `@model` decorator.

Verifies that `@model` correctly converts annotated classes into C-compatible
structures: field resolution, byte layout/sizeof, inheritance,
forward references, `ClassVar` handling, `Optional` types, and error reporting
for invalid or unresolved annotations.
"""

from typing import ClassVar, Literal, Optional, Self

import pytest

from eco_python2acom.decorators.layout import model, stub, union
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import (
    CStructure,
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
from eco_python2acom.types.utils import sizeof


@pytest.mark.unit
class TestModelBasicFields:
    """Verifies basic field resolution and `CStructure` compliance."""

    def test_simple_model_is_cstructure(self) -> None:
        """A decorated model is a subclass of `CStructure`."""

        @model
        class Point:
            x: Int32
            y: Int32

        assert issubclass(Point, CStructure)

    def test_simple_model_field_names_and_types(self) -> None:
        """Field names and types are preserved as declared."""

        @model
        class Point:
            x: Int32
            y: Int32

        assert hasattr(Point, "_fields_")
        assert Point._fields_ == [("x", Int32), ("y", Int32)]

    def test_model_instance_field_access(self) -> None:
        """Instance fields are accessible and store assigned values."""

        @model
        class Point:
            x: Int32
            y: Int32

        p = Point(3, 4)
        assert p.x == 3
        assert p.y == 4

    def test_model_eco_model_marker(self) -> None:
        """Decorator sets the `_eco_model_` marker attribute."""

        @model
        class Foo:
            value: Int32

        assert Foo._eco_model_ is True

    @pytest.mark.parametrize(
        ["field_type", "size"],
        [
            (Int8, 1),
            (UInt8, 1),
            (Int16, 2),
            (UInt16, 2),
            (Int32, 4),
            (UInt32, 4),
            (Int64, 8),
            (UInt64, 8),
            (Float, 4),
            (Double, 8),
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
        ],
    )
    def test_single_field_sizeof(self, field_type: type, size: int) -> None:
        """Sizeof a single-field model matches the C type's natural size."""

        @model
        class Wrap:
            value: field_type  # type: ignore

        assert sizeof(Wrap) == size


@pytest.mark.unit
class TestModelLayout:
    """Verifies byte layout, size, and field offsets."""

    def test_mixed_field_layout_offsets(self) -> None:
        """Field offsets follow C natural alignment rules."""

        @model
        class Mixed:
            a: Int8
            b: Int32
            c: Int16

        assert Mixed.a.offset == 0
        assert Mixed.b.offset == 4
        assert Mixed.c.offset == 8
        assert sizeof(Mixed) == 12

    def test_pointer_field_size(self) -> None:
        """`Ptr[T]` fields take pointer-width bytes."""

        @model
        class WithPtr:
            data: Ptr[Int32]

        assert sizeof(WithPtr) == sizeof(Ptr[Void])


@pytest.mark.unit
class TestModelClassVar:
    """Verifies `ClassVar` annotations are excluded from `_fields_`."""

    def test_classvar_is_excluded(self) -> None:
        """`ClassVar` annotations do not appear in `_fields_`."""

        @model
        class WithClassVar:
            CONSTANT: ClassVar[int] = 42
            value: Int32

        names = [name for name, _ in WithClassVar._fields_]
        assert names == ["value"]
        assert WithClassVar.CONSTANT == 42

    def test_classvar_only_model(self) -> None:
        """A model with only `ClassVars` has no instance fields."""

        @model
        class OnlyClassVars:
            A: ClassVar[int] = 1
            B: ClassVar[str] = "x"

        assert OnlyClassVars._fields_ == []


@pytest.mark.unit
class TestModelOptional:
    """Verifies `Optional[T]` is normalized to T."""

    def test_optional_field_resolves_to_inner(self) -> None:
        """`Optional[T]` is treated as T after normalization."""

        @model
        class WithOptional:
            value: Optional[Int32]

        assert WithOptional._fields_ == [("value", Int32)]

    def test_union_none_resolves_to_inner(self) -> None:
        """`T | None` is treated as T after normalization."""

        @model
        class WithOptional:
            value: Int32 | None

        assert WithOptional._fields_ == [("value", Int32)]


@pytest.mark.unit
class TestModelInheritance:
    """Verifies models can inherit from other models."""

    def test_inheritance_from_model_allowed(self) -> None:
        """A `@model` can inherit from another `@model`."""

        @model
        class Base:
            a: Int32

        @model
        class Derived(Base):
            b: Int32

        # Derived only declares its own fields
        derived = Derived(1, 2)
        assert derived.a == 1
        assert derived.b == 2
        assert Derived._fields_ == [("b", Int32)]
        assert sizeof(Derived) >= sizeof(Base)

    def test_inheritance_from_non_structure_fails(self) -> None:
        """Inheritance from a non-Structure base raises `TypeError`."""

        class NotAStruct:
            pass

        with pytest.raises(TypeError, match="invalid base"):

            @model
            class Bad(NotAStruct):
                x: Int32

    def test_multiple_inheritance_fails(self) -> None:
        """Multiple inheritance raises TypeError."""

        @model
        class A:
            x: Int32

        @model
        class B:
            y: Int32

        with pytest.raises(TypeError, match="multiple inheritance"):

            @model
            class C(A, B):
                z: Int32


@pytest.mark.unit
class TestModelForwardReferences:
    """Verifies forward references and self-references resolve correctly."""

    def test_forward_reference_to_other_model(self) -> None:
        """A model can forward-reference another model defined later via `stub`."""

        Child = stub("Child")

        @model
        class Container:
            child: Ptr[Child]

        @model
        class Child:
            value: Int32

        child = Child(10)
        container = Container(child=Ptr[Child](child))
        assert container.child.obj.value == 10

    def test_self_reference_via_pointer(self) -> None:
        """A model can recursively reference itself via `Ptr` and `stub`."""

        Node = stub("Node")

        @model
        class Node:
            value: Int32
            next: Ptr[Node]

        assert sizeof(Node) == 2 * sizeof(Ptr[Void])
        types = dict(Node._fields_)
        assert "value" in types
        assert "next" in types

    def test_string_annotation_resolves(self) -> None:
        """String-form annotations resolve correctly."""

        @model
        class StrAnnotated:
            value: "Int32"

        assert StrAnnotated._fields_ == [("value", Int32)]


@pytest.mark.unit
class TestModelInvalidAnnotations:
    """Verifies error reporting for unresolved or invalid annotations."""

    def test_unresolved_forward_ref_raises(self) -> None:
        """Unresolved forward references raise `TypeError` on finalization."""

        with pytest.raises(TypeError, match="unresolved types"):

            @model
            class Broken:
                value: "DoesNotExist"  # type: ignore

    def test_non_eco_type_raises(self) -> None:
        """Annotations that aren't EcoOS data types raise `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):

            @model
            class Bad:
                value: str

    def test_non_class_annotation_raises(self) -> None:
        """Annotations that are not classes raise `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):

            @model
            class Bad:
                value: Literal[42]

    def test_union_with_multiple_types_raises(self) -> None:
        """A Union with more than one non-None type raises `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS union"):

            @model
            class BadUnion:
                value: Int32 | Int16


@pytest.mark.unit
class TestModelNestedStructures:
    """Verifies nesting of models, unions, pointers, and arrays as fields."""

    def test_nested_model_by_value(self) -> None:
        """A model can embed another model by value."""

        @model
        class Inner:
            x: Int32
            y: Int32

        @model
        class Outer:
            inner: Inner
            tag: Int32

        assert Outer._fields_ == [("inner", Inner), ("tag", Int32)]
        assert sizeof(Outer) == sizeof(Inner) + sizeof(Int32)

    def test_nested_union_by_value(self) -> None:
        """A model can embed a union by value."""

        @union
        class Value:
            as_int: Int32
            as_float: Float

        @model
        class Container:
            tag: Int32
            value: Value

        assert Container._fields_ == [("tag", Int32), ("value", Value)]
        assert sizeof(Container) == sizeof(Int32) + sizeof(Value)

    def test_model_with_pointer_to_nested_model(self) -> None:
        """A model can contain a pointer to another model."""

        @model
        class Child:
            value: Int32

        @model
        class Parent:
            child: Ptr[Child]

        assert Parent._fields_ == [("child", Ptr[Child])]
        assert sizeof(Parent) == sizeof(Ptr[Void])

    def test_model_with_pointer_to_union(self) -> None:
        """A model can contain a pointer to a union."""

        @union
        class Value:
            as_int: Int32
            as_float: Float

        @model
        class Holder:
            ref: Ptr[Value]

        assert Holder._fields_ == [("ref", Ptr[Value])]
        assert sizeof(Holder) == sizeof(Ptr[Void])

    def test_model_with_array_field(self) -> None:
        """A model can contain a fixed-size array field."""

        @model
        class Buffer:
            data: Array[Int32, 8]

        assert Buffer._fields_ == [("data", Array[Int32, 8])]
        assert sizeof(Buffer) == sizeof(Int32) * 8

    def test_model_with_array_of_models(self) -> None:
        """A model can contain an array of nested models."""

        @model
        class Point:
            x: Int32
            y: Int32

        @model
        class Polyline:
            points: Array[Point, 4]

        assert sizeof(Polyline) == sizeof(Point) * 4

    def test_model_with_array_of_unions(self) -> None:
        """A model can contain an array of unions."""

        @union
        class Slot:
            as_int: Int32
            as_float: Float

        @model
        class Bank:
            slots: Array[Slot, 3]

        assert sizeof(Bank) == sizeof(Slot) * 3

    def test_model_with_pointer_to_array(self) -> None:
        """A model can hold a pointer to an array."""

        @model
        class Holder:
            buf: Ptr[Array[Int32, 16]]

        assert sizeof(Holder) == sizeof(Ptr[Void])

    def test_deeply_nested_composition(self) -> None:
        """A model with union-of-pointer-to-model-with-array resolves correctly."""

        @model
        class Leaf:
            tag: Int32
            buf: Array[UInt8, 4]

        @union
        class Payload:
            as_leaf: Ptr[Leaf]
            as_int: Int64

        @model
        class Root:
            header: Int32
            payload: Payload

        names = [name for name, _ in Root._fields_]
        assert names == ["header", "payload"]
        assert sizeof(Root) >= sizeof(Int32) + sizeof(Int64)

    def test_mutually_recursive_models(self) -> None:
        """Two models can hold pointers to each other via `stub`."""

        Partner = stub("Partner")

        @model
        class Node:
            value: Int32
            peer: Ptr[Partner]

        @model
        class Partner:
            value: Int32
            peer: Ptr[Node]

        assert ("peer", Ptr[Partner]) in Node._fields_
        assert ("peer", Ptr[Node]) in Partner._fields_

    def test_model_with_array_of_child_pointers(self) -> None:
        """A model can form an array of self-pointers via `stub`."""

        TreeNode = stub("TreeNode")

        @model
        class TreeNode:
            value: Int32
            children: Array[Ptr[TreeNode], 4]

        assert sizeof(TreeNode) >= sizeof(Int32) + sizeof(Ptr[Void]) * 4


@pytest.mark.unit
class TestModelBodyValidation:
    """Verifies decorator-time validation of the model body."""

    def test_bare_assignment_rejected(self) -> None:
        """A bare attribute without annotation is rejected at decoration time."""

        with pytest.raises(TypeError, match="no type annotation"):

            @model
            class Bad:
                x: Int32
                MAGIC = 42

    def test_convenience_accessors_allowed(self) -> None:
        """Methods, classmethods, staticmethods, and properties are allowed."""

        @model
        class Model:
            value: Int32

            def doubled(self) -> int:
                return int(self.value) * 2

            @classmethod
            def zero(cls) -> Self:
                return cls(0)

            @staticmethod
            def identity(x: int) -> int:
                return x

            @property
            def squared(self) -> int:
                return int(self.value) ** 2

        instance = Model()
        instance.value = 10
        assert instance.doubled() == 20
        assert Model.zero().value == 0
        assert Model.identity(5) == 5
        assert Model(5).squared == 25
