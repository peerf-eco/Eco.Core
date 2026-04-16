"""Unit tests for the `@interface` decorator.

Verifies that `@interface` correctly converts annotated method-stub classes
into ACOM interface types: IID assignment, VTbl method resolution,
inheritance from IEcoUnknown, dispatcher installation, forward references,
parameter/return type extraction, and error reporting.
"""

from __future__ import annotations

from typing import Any

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model, union
from eco_python2acom.decorators.utils import finalize
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CFuncType, CStructure, Double, Int16, Int32, UInt8, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import sizeof

EXAMPLE_IID = "ABCDEF12-3456-7890-ABCD-EF1234567890"


class TestInterfaceBasics:
    """Verifies basic interface decoration and metadata."""

    def test_interface_basics(self) -> None:
        """Decorator produces a `CStructure` subclass with marker and UGUID IID."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        assert issubclass(IFoo, CStructure)
        assert IFoo._eco_interface_ is True
        assert isinstance(IFoo._iid_, UGUID)
        assert IFoo._iid_.to_string().upper() == EXAMPLE_IID

    def test_iid_uguid_passed_through(self) -> None:
        """A pre-built `UGUID` passed as IID is stored on the class as-is."""

        guid = UGUID(EXAMPLE_IID)

        @interface(iid=guid)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        assert IFoo._iid_ is guid


class TestInterfaceMethodResolution:
    """Verifies VTbl method extraction and ordering."""

    def test_vtbl_methods_extracted_from_annotations(self) -> None:
        """Methods are extracted from annotated method stubs."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def MethodA(self) -> Int16:
                ...

            def MethodB(self, x: Int32) -> Int32:
                ...

        assert IFoo._fields_ == [
            ("_func_MethodA", CFuncType(Int16, Ptr[Void])),
            ("_func_MethodB", CFuncType(Int32, Ptr[Void], Int32)),
        ]

    def test_method_param_names_recorded(self) -> None:
        """Parameter names are stored on `_eco_method_params_` for keyword dispatch."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Add(self, left: Int32, right: Int32) -> Int32:
                ...

        IFoo = finalize(IFoo)
        assert IFoo._eco_method_params_["_func_Add"] == ["left", "right"]

    def test_method_return_type_recorded(self) -> None:
        """Return types are stored on `_eco_method_returns_` for result wrapping."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Compute(self) -> Double:
                ...

        IFoo = finalize(IFoo)
        assert IFoo._eco_method_returns_["_func_Compute"] is Double

    def test_dispatcher_installed_on_method(self) -> None:
        """Method stubs are replaced with dispatcher callables."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        IFoo = finalize(IFoo)
        assert callable(IFoo.Method)
        assert IFoo.Method.__name__ == "Method"


class TestInterfaceVTblSize:
    """Verifies VTbl byte layout matches the C-side ABI."""

    def test_vtbl_size_with_three_methods(self) -> None:
        """Three methods (3 inherited + 0 custom) yield three pointer slots."""

        @interface(iid=EXAMPLE_IID)
        class IEmpty(IEcoUnknown):
            pass

        assert sizeof(IEmpty) == 3 * sizeof(Ptr[Void])

    def test_vtbl_size_with_extra_methods(self) -> None:
        """N inherited + M custom methods yield (N + M) pointer slots."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def A(self) -> Int16:
                ...

            def B(self, x: Int32) -> Int32:
                ...

            def C(self, x: Double, y: Double) -> Double:
                ...

        assert sizeof(IFoo) == (3 + 3) * sizeof(Ptr[Void])


class TestInterfaceInheritance:
    """Verifies inheritance from `IEcoUnknown` and other interfaces."""

    def test_must_inherit_from_iecounknown_or_compatible(self) -> None:
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
        class IA(IEcoUnknown):
            def A(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IB(IEcoUnknown):
            def B(self) -> Int16:
                ...

        with pytest.raises(TypeError, match="multiple inheritance"):

            @interface(iid=EXAMPLE_IID)
            class IBad(IA, IB):
                def Method(self) -> Int16:
                    ...

    def test_interface_can_extend_another_interface(self) -> None:
        """Interfaces can be derived, child contributes its own methods."""

        @interface(iid=EXAMPLE_IID)
        class IBase(IEcoUnknown):
            def Foo(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IDerived(IBase):
            def Bar(self) -> Int16:
                ...

        names = [name for name, _ in IDerived._fields_]
        assert "_func_Foo" not in names
        assert "_func_Bar" in names
        assert sizeof(IDerived) >= sizeof(IBase)


class TestInterfaceForwardReferences:
    """Verifies forward references in method signatures resolve."""

    def test_forward_reference_to_later_interface(self) -> None:
        """Method signatures may reference interfaces declared later."""

        @interface(iid=EXAMPLE_IID)
        class IContainer(IEcoUnknown):
            def GetChild(self, child: Ptr[Ptr[IChild]]) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IChild(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        assert IContainer._fields_ == [
            ("_func_GetChild", CFuncType(Int16, Ptr[Void], Ptr[Ptr[IChild]]))
        ]

    def test_forward_reference_to_later_model(self) -> None:
        """An interface can forward-reference a model defined later."""

        @interface(iid=EXAMPLE_IID)
        class IContext(IEcoUnknown):
            def SetState(self, state: State) -> Int16:
                ...

        @model
        class State:
            flags: Int32

        assert IContext._fields_ == [("_func_SetState", CFuncType(Int16, Ptr[Void], State))]

    def test_self_reference_via_pointer(self) -> None:
        """An interface can reference itself in its own signatures."""

        @interface(iid=EXAMPLE_IID)
        class IRecursive(IEcoUnknown):
            def Clone(self, out: Ptr[Ptr[IRecursive]]) -> Int16:
                ...

        assert IRecursive._fields_ == [
            ("_func_Clone", CFuncType(Int16, Ptr[Void], Ptr[Ptr[IRecursive]]))
        ]

    def test_string_annotation_for_param(self) -> None:
        """String form annotations are resolved correctly."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self, x: "Int32") -> "Int32":
                ...

        assert IFoo._fields_ == [("_func_Method", CFuncType(Int32, Ptr[Void], Int32))]


class TestInterfaceInvalidAnnotations:
    """Verifies error reporting for unresolved or invalid method types."""

    def test_unresolved_forward_ref_raises(self) -> None:
        """Unresolved forward references raise `TypeError` on finalization."""

        @interface(iid=EXAMPLE_IID)
        class IBroken(IEcoUnknown):
            def Method(self, x: "DoesNotExist") -> Int16:  # type: ignore
                ...

        with pytest.raises(TypeError, match="unresolved types"):
            IBroken = finalize(IBroken)

    def test_non_eco_param_type_raises(self) -> None:
        """Non-EcoOS parameter types raise `TypeError`."""

        @interface(iid=EXAMPLE_IID)
        class IBad(IEcoUnknown):
            def Method(self, x: str) -> Int16:
                ...

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):
            IBad = finalize(IBad)

    def test_non_eco_return_type_raises(self) -> None:
        """Non-EcoOS return types raise `TypeError`."""

        @interface(iid=EXAMPLE_IID)
        class IBad(IEcoUnknown):
            def Method(self) -> list:
                ...

        with pytest.raises(TypeError, match="Invalid EcoOS data type"):
            IBad = finalize(IBad)


class TestInterfaceDispatcherSemantics:
    """Verifies dispatcher behavior (positional/keyword/missing args)."""

    def test_dispatch_keyword_missing_arg_raises(self) -> None:
        """Calling with keyword args missing a required parameter raises."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Add(self, left: Int32, right: Int32) -> Int32:
                ...

        IFoo = finalize(IFoo)

        # Build a fake instance bypassing __init__ to avoid touching real types
        instance = IFoo.__new__(IFoo)

        with pytest.raises(TypeError, match="missing required argument"):
            IFoo.Add(instance, left=10)  # Missing 'right'

    def test_dispatcher_preserves_docstring(self) -> None:
        """Dispatcher inherits the original method's docstring."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Add(self, left: Int32, right: Int32) -> Int32:
                """Add two integers."""
                ...

        IFoo = finalize(IFoo)
        assert IFoo.Add.__doc__ == "Add two integers."


class TestInterfaceCompositeParameters:
    """Verifies interfaces with methods that accept/return composite types."""

    def test_method_with_pointer_to_model_param(self) -> None:
        """A method can accept a pointer to a model as a parameter."""

        @model
        class Point:
            x: Int32
            y: Int32

        @interface(iid=EXAMPLE_IID)
        class IShape(IEcoUnknown):
            def GetOrigin(self, out: Ptr[Point]) -> Int16:
                ...

        assert IShape._fields_ == [("_func_GetOrigin", CFuncType(Int16, Ptr[Void], Ptr[Point]))]

    def test_method_with_array_param(self) -> None:
        """A method can accept an array as a parameter."""

        @interface(iid=EXAMPLE_IID)
        class IBuffer(IEcoUnknown):
            def Fill(self, data: Array[UInt8, 16]) -> Int16:
                ...

        assert IBuffer._fields_ == [("_func_Fill", CFuncType(Int16, Ptr[Void], Array[UInt8, 16]))]

    def test_method_with_double_pointer_to_interface(self) -> None:
        """A method can accept a double pointer to another interface (out-param)."""

        @interface(iid=EXAMPLE_IID)
        class IChild(IEcoUnknown):
            def Ping(self) -> Int16:
                ...

        @interface(iid=EXAMPLE_IID)
        class IFactory(IEcoUnknown):
            def CreateChild(self, child: Ptr[Ptr[IChild]]) -> Int16:
                ...

        assert IFactory._fields_ == [
            ("_func_CreateChild", CFuncType(Int16, Ptr[Void], Ptr[Ptr[IChild]]))
        ]

    def test_method_with_model_by_value_param(self) -> None:
        """A method may accept a model by value."""

        @model
        class Point:
            x: Int32
            y: Int32

        @interface(iid=EXAMPLE_IID)
        class IShape(IEcoUnknown):
            def SetOrigin(self, point: Point) -> Int16:
                ...

        assert IShape._fields_ == [("_func_SetOrigin", CFuncType(Int16, Ptr[Void], Point))]

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
        class IHandler(IEcoUnknown):
            def Handle(self, payload: Ptr[Payload]) -> Int16:
                ...

        assert IHandler._fields_ == [("_func_Handle", CFuncType(Int16, Ptr[Void], Ptr[Payload]))]


class TestInterfaceLazyFinalization:
    """Verifies the lazy-finalization behavior."""

    def test_fields_access_finalizes(self) -> None:
        """Accessing `_fields_` marks the class as finalized."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        assert IFoo.__dict__.get("_eco_ready_") is False
        _ = IFoo._fields_
        assert IFoo.__dict__.get("_eco_ready_") is True

    def test_sizeof_finalizes(self) -> None:
        """`sizeof` access triggers finalization."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        assert IFoo.__dict__.get("_eco_ready_") is False
        _ = sizeof(IFoo)
        assert IFoo.__dict__.get("_eco_ready_") is True

    def test_subclassing_finalizes_base(self) -> None:
        """Subclassing an interface triggers finalization of the base."""

        @interface(iid=EXAMPLE_IID)
        class IBase(IEcoUnknown):
            def Foo(self) -> Int16:
                ...

        assert IBase.__dict__.get("_eco_ready_") is False

        @interface(iid=EXAMPLE_IID)
        class IDerived(IBase):
            def Bar(self) -> Int16:
                ...

        assert IBase.__dict__.get("_eco_ready_") is True


class TestInterfaceRepr:
    """Verifies `__repr__` priority (inherited vs default) for interfaces."""

    def test_inherited_repr_takes_priority(self) -> None:
        """An interface inheriting from `IEcoUnknown` reuses its `__repr__`."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self) -> Int16:
                ...

        instance = IFoo.__new__(IFoo)
        instance.ptr = Ptr[Void](0xCAFE)
        assert "<IFoo at" in repr(instance)

    def test_bare_interface_repr_includes_ptr(self) -> None:
        """An interface without `IEcoUnknown` uses default behaviour."""

        @interface(iid=EXAMPLE_IID)
        class IBare:
            def Method(self) -> Int16:
                ...

        instance = IBare.__new__(IBare)
        instance.ptr = Ptr[Void](0xCAFE)
        text = repr(instance)
        assert "IBare" in text
        assert "ptr=" in text


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
            class IBad(IEcoUnknown):
                @decorator
                def Method(self) -> Int16:
                    ...

    def test_missing_param_annotation_rejected(self) -> None:
        """A method parameter without a type annotation is rejected."""

        with pytest.raises(TypeError, match="has no type annotation"):

            @interface(iid=EXAMPLE_IID)
            class IBad(IEcoUnknown):
                def Method(self, x: Int32, y) -> Int16:
                    ...

    def test_param_named_cls_is_accepted(self) -> None:
        """A regular parameter may be named `cls` without being filtered out."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self, cls: Int32) -> Int16:
                ...

        IFoo = finalize(IFoo)
        assert IFoo._eco_method_params_["_func_Method"] == ["cls"]
        assert IFoo._fields_ == [("_func_Method", CFuncType(Int16, Ptr[Void], Int32))]

    def test_first_param_non_self_name_accepted(self) -> None:
        """The first parameter may have any name and will skip by position."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(this, x: Int32) -> Int16:
                ...

        IFoo = finalize(IFoo)
        assert IFoo._eco_method_params_["_func_Method"] == ["x"]

    def test_method_without_return_annotation_accepted(self) -> None:
        """A method without a return annotation is accepted and defaults to `Int16`."""

        @interface(iid=EXAMPLE_IID)
        class IFoo(IEcoUnknown):
            def Method(self, x: Int32):
                ...

        IFoo = finalize(IFoo)
        assert IFoo._eco_method_returns_["_func_Method"] is Int16
