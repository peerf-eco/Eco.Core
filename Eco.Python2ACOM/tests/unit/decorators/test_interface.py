"""Unit tests for the `@interface` decorator.

Verifies that `@interface` correctly converts annotated method-stub classes
into ACOM interface types: IID assignment, VTbl method resolution,
inheritance, forward references, parameter/return type extraction, and error reporting.
"""

from typing import Any

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model, stub, union
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CStructure, Double, Int16, Int32, UInt8, Void
from eco_python2acom.types.function import Func
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import sizeof

EXAMPLE_IID = "ABCDEF12-3456-7890-ABCD-EF1234567890"


@pytest.mark.unit
class TestInterfaceBasics:
    """Verifies basic interface decoration and metadata."""

    def test_interface_basics(self) -> None:
        """Decorator produces a `CStructure` subclass with marker and UGUID IID."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Method(self) -> Int16:
                ...

        assert issubclass(IFoo, CStructure)
        assert IFoo._eco_interface_ is True
        assert isinstance(IFoo._iid_, UGUID)
        assert IFoo._iid_.to_string() == EXAMPLE_IID

    def test_iid_uguid_passed_through(self) -> None:
        """A pre-built `UGUID` passed as IID is stored on the class as-is."""

        guid = UGUID(EXAMPLE_IID)

        @interface(iid=guid)
        class IFoo:
            def Method(self) -> Int16:
                ...

        assert IFoo._iid_ is guid


@pytest.mark.unit
class TestInterfaceMethodResolution:
    """Verifies VTbl method extraction and ordering."""

    def test_vtbl_methods_extracted_from_annotations(self) -> None:
        """Methods are extracted into the hidden `_vtbl_` class."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def MethodA(self) -> Int16:
                ...

            def MethodB(self, x: Int32) -> Int32:
                ...

        own_slots = IFoo._vtbl_._fields_
        assert own_slots == [
            ("_func_MethodA", Func[Int16, [Ptr[IFoo]]]),
            ("_func_MethodB", Func[Int32, [Ptr[IFoo], Int32]]),
        ]

    def test_method_param_names_recorded(self) -> None:
        """Parameter names are stored on `_eco_method_params_` for keyword dispatch."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Add(self, left: Int32, right: Int32) -> Int32:
                ...

        assert IFoo._eco_method_params_["_func_Add"] == ["left", "right"]

    def test_method_return_type_recorded(self) -> None:
        """Return types are stored on `_eco_method_returns_` for result wrapping."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Compute(self) -> Double:
                ...

        assert IFoo._eco_method_returns_["_func_Compute"] is Double

    def test_dispatcher_installed_on_method(self) -> None:
        """Method stubs are replaced with dispatcher callables."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Method(self) -> Int16:
                ...

        assert callable(IFoo.Method)
        assert IFoo.Method.__name__ == "Method"


@pytest.mark.unit
class TestInterfaceInstanceLayout:
    """Verifies that an interface instance is a flat `{ vtbl* }` struct."""

    def test_interface_is_single_vtbl_pointer(self) -> None:
        """An interface instance is laid out as a single `vtbl*` pointer."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Method(self) -> Int16:
                ...

        assert IFoo._fields_ == [("vtbl", Ptr[IFoo._vtbl_])]
        assert sizeof(IFoo) == sizeof(Ptr[Void])


@pytest.mark.unit
class TestInterfaceVTblLayout:
    """Verifies VTbl byte layout matches the C-side ABI."""

    def test_vtbl_size_with_methods(self) -> None:
        """N inherited + M custom methods yield (N + M) pointer slots."""

        @interface(iid=EXAMPLE_IID)
        class Parent:
            def Method(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class Child(Parent):
            def A(self) -> Int16:
                ...

            def B(self, x: Int32) -> Int32:
                ...

            def C(self, x: Double, y: Double) -> Double:
                ...

        assert sizeof(Child._vtbl_) == (3 + 1) * sizeof(Ptr[Void])


@pytest.mark.unit
class TestInterfaceInheritance:
    """Verifies inheritance from interfaces."""

    def test_inheritance_from_non_structure_fails(self) -> None:
        """An interface inheriting from a non-Structure base raises `TypeError`."""

        class Plain:
            pass

        with pytest.raises(TypeError, match="invalid base"):

            @interface(iid=EXAMPLE_IID)
            class IBad(Plain):
                def Method(self) -> Int16:
                    ...

    def test_multiple_inheritance_fails(self) -> None:
        """Multiple inheritance is rejected."""

        @interface(iid=EXAMPLE_IID)
        class IA:
            def A(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IB:
            def B(self) -> Int16:
                ...

        with pytest.raises(TypeError, match="multiple inheritance"):

            @interface(iid=EXAMPLE_IID)
            class IBad(IA, IB):
                def Method(self) -> Int16:
                    ...

    def test_interface_can_extend_another_interface(self) -> None:
        """Derived interface vtable contains inherited and own slots."""

        @interface(iid=EXAMPLE_IID)
        class IBase:
            def Foo(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IDerived(IBase):
            def Bar(self) -> Int16:
                ...

        names = [name for name, _ in IDerived._vtbl_._fields_]
        assert "_func_Foo" in names
        assert "_func_Bar" in names
        assert sizeof(IDerived._vtbl_) > sizeof(IBase._vtbl_)


@pytest.mark.unit
class TestInterfaceForwardReferences:
    """Verifies forward references in method signatures resolve."""

    def test_forward_reference_to_later_interface(self) -> None:
        """Method signatures may reference interfaces declared later via `stub`."""

        IChild = stub("IChild")

        @interface(iid=EXAMPLE_IID)
        class IContainer:
            def GetChild(self, child: Ptr[Ptr[IChild]]) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IChild:
            def Method(self) -> Int16:
                ...

        assert IContainer._vtbl_._fields_[-1] == (
            "_func_GetChild",
            Func[Int16, [Ptr[IContainer], Ptr[Ptr[IChild]]]],
        )
        assert IChild._vtbl_._fields_[-1] == ("_func_Method", Func[Int16, [Ptr[IChild]]])

    def test_forward_reference_to_later_model(self) -> None:
        """An interface can forward-reference a model defined later via `stub`."""

        State = stub("State")

        @interface(iid=EXAMPLE_IID)
        class IContext:
            def SetState(self, state: Ptr[State]) -> Int16:
                ...

        @model
        class State:
            flags: Int32

        assert IContext._vtbl_._fields_[-1] == (
            "_func_SetState",
            Func[Int16, [Ptr[IContext], Ptr[State]]],
        )
        assert sizeof(State) == sizeof(Int32)

    def test_self_reference_via_pointer(self) -> None:
        """An interface can reference itself in its own signatures via `stub`."""

        IRecursive = stub("IRecursive")

        @interface(iid=EXAMPLE_IID)
        class IRecursive:
            def Clone(self, out: Ptr[Ptr[IRecursive]]) -> Int16:
                ...

        assert IRecursive._vtbl_._fields_[-1] == (
            "_func_Clone",
            Func[Int16, [Ptr[IRecursive], Ptr[Ptr[IRecursive]]]],
        )

    def test_string_annotation_for_param(self) -> None:
        """String form annotations are resolved correctly."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Method(self, x: "Int32") -> "Int32":
                ...

        assert IFoo._vtbl_._fields_[-1] == ("_func_Method", Func[Int32, [Ptr[IFoo], Int32]])


@pytest.mark.unit
class TestInterfaceInvalidAnnotations:
    """Verifies error reporting for unresolved or invalid method types."""

    def test_unresolved_forward_ref_raises(self) -> None:
        """Unresolved forward references raise `TypeError` on finalization."""

        with pytest.raises(TypeError, match="unresolved types"):

            @interface(iid=EXAMPLE_IID)
            class IBroken:
                def Method(self, x: "DoesNotExist") -> Int16:  # type: ignore
                    ...

    def test_non_eco_param_type_raises(self) -> None:
        """Non-EcoOS parameter types raise `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):

            @interface(iid=EXAMPLE_IID)
            class IBad:
                def Method(self, x: str) -> Int16:
                    ...

    def test_non_eco_return_type_raises(self) -> None:
        """Non-EcoOS return types raise `TypeError`."""

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):

            @interface(iid=EXAMPLE_IID)
            class IBad:
                def Method(self) -> list:
                    ...


@pytest.mark.unit
class TestInterfaceCompositeParameters:
    """Verifies interfaces with methods that accept/return composite types."""

    def test_method_with_pointer_to_model_param(self) -> None:
        """A method can accept a pointer to a model as a parameter."""

        @model
        class Point:
            x: Int32
            y: Int32

        @interface(iid=EXAMPLE_IID)
        class IShape:
            def GetOrigin(self, out: Ptr[Point]) -> Int16:
                ...

        assert IShape._vtbl_._fields_[-1] == (
            "_func_GetOrigin",
            Func[Int16, [Ptr[IShape], Ptr[Point]]],
        )

    def test_method_with_array_param(self) -> None:
        """A method can accept an array as a parameter."""

        @interface(iid=EXAMPLE_IID)
        class IBuffer:
            def Fill(self, data: Array[UInt8, 16]) -> Int16:
                ...

        assert IBuffer._vtbl_._fields_[-1] == (
            "_func_Fill",
            Func[Int16, [Ptr[IBuffer], Array[UInt8, 16]]],
        )

    def test_method_with_double_pointer_to_interface(self) -> None:
        """A method can accept a double pointer to another interface (out-param)."""

        @interface(iid=EXAMPLE_IID)
        class IChild:
            def Ping(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IFactory:
            def CreateChild(self, child: Ptr[Ptr[IChild]]) -> Int16:
                ...

        assert IFactory._vtbl_._fields_[-1] == (
            "_func_CreateChild",
            Func[Int16, [Ptr[IFactory], Ptr[Ptr[IChild]]]],
        )

    def test_method_with_model_by_value_param(self) -> None:
        """A method may accept a model by value."""

        @model
        class Point:
            x: Int32
            y: Int32

        @interface(iid=EXAMPLE_IID)
        class IShape:
            def SetOrigin(self, point: Point) -> Int16:
                ...

        assert IShape._vtbl_._fields_[-1] == (
            "_func_SetOrigin",
            Func[Int16, [Ptr[IShape], Point]],
        )

    def test_method_with_nested_union_pointer(self) -> None:
        """An interface can accept a pointer to a union containing nested models."""

        @model
        class Small:
            a: Int16

        @union
        class Payload:
            as_small: Small
            as_int: Int32

        @interface(iid=EXAMPLE_IID)
        class IHandler:
            def Handle(self, payload: Ptr[Payload]) -> Int16:
                ...

        assert IHandler._vtbl_._fields_[-1] == (
            "_func_Handle",
            Func[Int16, [Ptr[IHandler], Ptr[Payload]]],
        )


@pytest.mark.unit
class TestInterfaceBodyValidation:
    """Verifies decorator-time validation of the interface body."""

    @pytest.mark.parametrize(
        ["decorator", "match"],
        [
            (classmethod, "classmethod"),
            (staticmethod, "staticmethod"),
            (property, "property"),
        ],
        ids=["classmethod", "staticmethod", "property"],
    )
    def test_descriptor_rejected(self, decorator: Any, match: str) -> None:
        """`classmethod`, `staticmethod`, and `property` are rejected at decoration time."""

        with pytest.raises(TypeError, match=match):

            @interface(iid=EXAMPLE_IID)
            class IBad:
                @decorator
                def Method(self) -> Int16:
                    ...

    def test_missing_param_annotation_rejected(self) -> None:
        """A method parameter without a type annotation is rejected."""

        with pytest.raises(TypeError, match="has no type annotation"):

            @interface(iid=EXAMPLE_IID)
            class IBad:
                def Method(self, x: Int32, y) -> Int16:
                    ...

    def test_first_param_non_self_name_accepted(self) -> None:
        """The first parameter may have any name and will skip by position."""

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            def Method(this, x: Int32) -> Int16:
                ...

        assert IFoo._eco_method_params_["_func_Method"] == ["x"]

    def test_method_without_return_annotation_rejected(self) -> None:
        """A method without a return annotation is rejected at decoration time."""

        with pytest.raises(TypeError, match="has no return type annotation"):

            @interface(iid=EXAMPLE_IID)
            class IFoo:
                def Method(self, x: Int32):
                    ...

    def test_method_without_self_parameter_rejected(self) -> None:
        """A method with zero parameters is rejected at decoration time."""

        with pytest.raises(TypeError, match="must declare 'self'"):

            @interface(iid=EXAMPLE_IID)
            class IBad:
                def Method() -> Int16:
                    ...

    def test_non_function_attributes_ignored(self) -> None:
        """Non-function attributes in the class body are ignored by method resolution."""

        CONSTANT = 42

        @interface(iid=EXAMPLE_IID)
        class IFoo:
            marker = CONSTANT

            def Method(self) -> Int16:
                ...

        assert IFoo.marker == CONSTANT
        slot_names = [name for name, _ in IFoo._vtbl_._fields_]
        assert slot_names == ["_func_Method"]

    def test_redeclaring_ancestor_method_rejected(self) -> None:
        """Redeclaring a method already defined by an ancestor raises `TypeError`."""

        @interface(iid=EXAMPLE_IID)
        class IBase:
            def Method(self) -> Int16:
                ...

        with pytest.raises(TypeError, match="already defined"):

            @interface(iid=EXAMPLE_IID)
            class IChild(IBase):
                def Method(self) -> Int16:
                    ...
