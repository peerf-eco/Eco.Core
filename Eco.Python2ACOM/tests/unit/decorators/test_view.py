"""Unit tests for the `@view` decorator.

Verifies that `@view` correctly marks a class as a view inside an `@component`:
interface resolution from bases, method-implementation validation, marker
attributes, IEcoUnknown-method exemption, and error reporting for malformed
view declarations.
"""

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.server.view import view
from eco_python2acom.types.core import Int16, Int32, Void

EXAMPLE_IID_X = "ABCDEF12-3456-7890-ABCD-EF1234567890"
EXAMPLE_IID_Y = "12345678-1234-1234-1234-123456789012"


@interface(iid=EXAMPLE_IID_X)
class IFooX:
    def MethodA(self) -> Int16:
        ...

    def MethodB(self, value: Int32) -> Int32:
        ...


@interface(iid=EXAMPLE_IID_Y)
class IFooY:
    def MethodC(self) -> Void:
        ...


@pytest.mark.unit
class TestViewBasics:
    """Verifies basic view decoration: markers, identity, callable methods."""

    def test_view_decoration(self) -> None:
        """Decorator preserves class identity, sets markers, leaves methods callable."""

        class Impl(IFooX):
            def MethodA(self) -> Int16:
                return 7

            def MethodB(self, value: Int32) -> Int32:
                return value + 1

        decorated = view(Impl)

        assert decorated is Impl
        assert hasattr(Impl, "_eco_view_") and Impl._eco_view_ is True
        assert hasattr(Impl, "_eco_iface_") and Impl._eco_iface_ is IFooX

        instance = Impl()
        assert instance.MethodA() == 7
        assert instance.MethodB(10) == 11


@pytest.mark.unit
class TestViewInterfaceResolution:
    """Verifies how the decorator picks the interface from the class bases."""

    def test_resolves_single_interface_base(self) -> None:
        """A class inheriting from exactly one interface resolves it."""

        @view
        class Impl(IFooX):
            def MethodA(self) -> Int16:
                return 0

            def MethodB(self, value: Int32) -> Int32:
                return value

        assert Impl._eco_iface_ is IFooX

    def test_no_interface_base_raises(self) -> None:
        """A class without an interface base raises `TypeError`."""

        class NotAnInterface:
            pass

        with pytest.raises(TypeError, match="must inherit from exactly one interface"):

            @view
            class Impl(NotAnInterface):
                pass

    def test_two_interface_bases_raises(self) -> None:
        """A class inheriting from two interfaces raises `TypeError`."""

        with pytest.raises(TypeError, match="must inherit from exactly one interface"):

            @view
            class Impl(IFooX, IFooY):
                def MethodA(self) -> Int16:
                    return 0

                def MethodB(self, value: Int32) -> Int32:
                    return value

                def MethodC(self) -> Void:
                    return None

    def test_bare_object_base_raises(self) -> None:
        """A class with the default `object` base raises `TypeError`."""

        with pytest.raises(TypeError, match="must inherit from exactly one interface"):

            @view
            class Impl:
                pass


@pytest.mark.unit
class TestViewImplementationValidation:
    """Verifies that every business method of the interface is implemented."""

    def test_missing_method_raises(self) -> None:
        """A view that misses a business method raises `NotImplementedError`."""

        with pytest.raises(NotImplementedError, match="does not implement 'MethodB'"):

            @view
            class Impl(IFooX):
                def MethodA(self) -> Int16:
                    return 0

    def test_method_must_be_a_function(self) -> None:
        """A `staticmethod` placeholder does not satisfy the requirement."""

        with pytest.raises(NotImplementedError, match="does not implement 'MethodA'"):

            @view
            class Impl(IFooX):
                MethodA = staticmethod(lambda: 0)

                def MethodB(self, value: Int32) -> Int32:
                    return value

    def test_classmethod_does_not_satisfy(self) -> None:
        """A `classmethod` placeholder does not satisfy the requirement."""

        with pytest.raises(NotImplementedError):

            @view
            class Impl(IFooX):
                @classmethod
                def MethodA(cls) -> Int16:
                    return 0

                def MethodB(self, value: Int32) -> Int32:
                    return value

    def test_non_callable_attribute_does_not_satisfy(self) -> None:
        """An attribute that is not a function fails validation."""

        with pytest.raises(NotImplementedError):

            @view
            class Impl(IFooX):
                MethodA = 42

                def MethodB(self, value: Int32) -> Int32:
                    return value

    def test_inherited_method_does_not_satisfy(self) -> None:
        """A method inherited from a non-interface base does not satisfy."""

        class Base:
            def MethodA(self) -> Int16:
                return 0

        with pytest.raises(NotImplementedError, match="does not implement 'MethodA'"):

            @view
            class Impl(Base, IFooX):
                def MethodB(self, value: Int32) -> Int32:
                    return value


@pytest.mark.unit
class TestViewIEcoUnknownExemption:
    """Verifies that IEcoUnknown methods need not be implemented by the view."""

    def test_unknown_triple_not_required(self) -> None:
        """`QueryInterface`, `AddRef`, `Release` are supplied by `@component`."""

        @view
        class Impl(IFooX):
            def MethodA(self) -> Int16:
                return 0

            def MethodB(self, value: Int32) -> Int32:
                return value

        for name in ("QueryInterface", "AddRef", "Release"):
            assert name not in Impl.__dict__
