"""Unit tests for the `@factory` decorator and `export` helper.

Verifies that `@factory` correctly composes a singleton companion class for a
`@component`-decorated product: marker attributes, `__init__` wiring, the
synthesised `IEcoComponentFactory` view, the `Alloc` allocation path with all
its early-out branches, and the `export(...)` glue used by the Python loader.
"""

from typing import Any, Optional, Union

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.iid import IID_IEcoComponentFactory
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, offsetof

CID_DEFAULT = "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA"
IID_X = "BBBBBBBB-BBBB-BBBB-BBBB-BBBBBBBBBBBB"


@interface(iid=IID_X)
class IFooX(IEcoUnknown):
    def Foo(self) -> Int32:
        ...


def make_product(cid: Optional[Union[str, UGUID]] = CID_DEFAULT) -> type:
    """Build a minimal `@component` product for the factory under test."""

    @component(cid=cid)
    class Product:
        def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
            return 0

        def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
            return 0

        def __eco_del__(self) -> Void:
            return None

        @view
        class X(IFooX):
            def Foo(self) -> Int32:
                return 42

    return Product


def make_factory(product: type) -> type:
    """Build the smallest valid factory class for `product`."""

    @factory(component=product)
    class Factory(IEcoComponentFactory):
        name: CString = b"Test.Component"
        version: CString = b"0.0.0.0"
        manufacturer: CString = b"Test"

    return Factory


@pytest.fixture
def factory_cls() -> type:
    """Fixture building a factory class for a default minimal product."""
    return make_factory(make_product())


@pytest.mark.unit
class TestFactoryBasics:
    """Verifies marker attributes and the synthesised factory view."""

    def test_decoration_sets_markers(self) -> None:
        """Decorator sets `_eco_factory_` and `_eco_component_cls_`."""

        product = make_product()
        Factory = make_factory(product)

        assert Factory._eco_factory_ is True
        assert Factory._eco_component_cls_ is product

    def test_factory_view_is_indexed_by_iid_component_factory(self, factory_cls: type) -> None:
        """The synthesised view sits under `IID_IEcoComponentFactory`."""

        assert len(factory_cls._eco_views_) == 1
        assert IID_IEcoComponentFactory in factory_cls._eco_views_


@pytest.mark.unit
class TestFactoryDecorationValidation:
    """Verifies that `@factory` rejects malformed inputs."""

    def test_non_component_product_raises(self) -> None:
        """`@factory(component=X)` where `X` is not a component raises `TypeError`."""

        class NotAComponent:
            pass

        with pytest.raises(TypeError, match="is not a component"):
            factory(component=NotAComponent)

    def test_internal_helper_product_raises(self) -> None:
        """A product without a CID (internal helper) cannot have a factory."""

        helper = make_product(cid=None)

        with pytest.raises(TypeError, match="has no CID"):
            factory(component=helper)

    def test_factory_class_must_inherit_from_iecocomponentfactory(self) -> None:
        """A factory class that does not inherit from `IEcoComponentFactory` is rejected."""

        product = make_product()

        with pytest.raises(TypeError, match="must inherit from 'IEcoComponentFactory'"):

            @factory(component=product)
            class Factory:
                name: CString = b"Test"
                version: CString = b"0.0.0.0"
                manufacturer: CString = b"Test"


@pytest.mark.unit
class TestFactoryInit:
    """Verifies that `__init__` wires the vtable, metadata, and refcount."""

    def test_init_sets_refs_to_one(self, factory_cls: type) -> None:
        """A freshly built factory has `refs == 1` (singleton pinned alive)."""

        assert factory_cls().refs == 1

    def test_init_installs_factory_vtable(self, factory_cls: type) -> None:
        """The `_vtbl_IEcoComponentFactory` field is wired to the singleton vtable."""

        inst = factory_cls()

        vtbl_ptr = inst._vtbl_IEcoComponentFactory
        expected = factory_cls._eco_vtbls_[IID_IEcoComponentFactory]
        assert vtbl_ptr.value == addressof(expected)

    def test_init_seeds_metadata_fields(self, factory_cls: type) -> None:
        """`name`, `version`, `manufacturer` defaults are copied into the instance."""

        inst = factory_cls()

        assert inst.name == b"Test.Component"
        assert inst.version == b"0.0.0.0"
        assert inst.manufacturer == b"Test"


@pytest.mark.unit
class TestFactoryAlloc:
    """Verifies the `Alloc` allocation method exposed by the factory view."""

    @pytest.fixture
    def unknown(self) -> Ptr[IEcoUnknown]:
        """Fixture generating a non-null `Ptr[IEcoUnknown]` placeholder."""
        ptr: Ptr[IEcoUnknown] = Ptr[IEcoUnknown]()
        ptr.value = 0x123456789
        return ptr

    def alloc(
        self,
        inst: Any,
        system: Optional[Ptr[IEcoUnknown]],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Invoke the bound `Alloc` directly on the factory instance."""
        return inst._eco_views_[IID_IEcoComponentFactory].__dict__["Alloc"](
            inst, system, outer, iid, out
        )

    def test_alloc_success_returns_view_pointer_and_pins_instance(
        self, unknown: Ptr[IEcoUnknown]
    ) -> None:
        """`Alloc` builds a component, runs QI, leaves the instance pinned with `refs == 1`."""
        product = make_product()
        Factory = make_factory(product)
        inst = Factory()

        out = Ptr[Void]()
        iid = UGUID(IID_X)
        result = self.alloc(inst, unknown, None, pointer(iid), pointer(out))

        # The returned pointer is the offset of the `IFooX` view inside the new instance
        new_addr = out.value - offsetof(product, IFooX)
        assert result == EcoErrorCode.SUCCESS
        assert new_addr in ALIVE
        assert ALIVE[new_addr].refs == 1

        ALIVE[new_addr].Release()  # Cleanup

    def test_alloc_null_pointer_returns_pointer_error(
        self, factory_cls: type, unknown: Ptr[IEcoUnknown]
    ) -> None:
        """`Alloc` with any of `system`, `iid`, `out` null returns `POINTER`."""

        inst = factory_cls()
        iid = UGUID(IID_X)
        out = Ptr[Void]()

        assert self.alloc(inst, None, None, pointer(iid), pointer(out)) == EcoErrorCode.POINTER
        assert self.alloc(inst, unknown, None, Ptr[UGUID](), pointer(out)) == EcoErrorCode.POINTER
        assert (
            self.alloc(inst, unknown, None, pointer(iid), Ptr[Ptr[Void]]()) == EcoErrorCode.POINTER
        )

    def test_alloc_aggregation_requires_iid_iecounknown(
        self, factory_cls: type, unknown: Ptr[IEcoUnknown]
    ) -> None:
        """A non-null `outer` with a non-`IID_IEcoUnknown` `iid` returns `NOAGGREGATION`."""

        inst = factory_cls()
        iid = UGUID(IID_X)
        out = Ptr[Void]()

        result = self.alloc(inst, unknown, unknown, pointer(iid), pointer(out))

        assert result == EcoErrorCode.NOAGGREGATION

    def test_alloc_propagates_eco_new_failure(self, unknown: Ptr[IEcoUnknown]) -> None:
        """If `__eco_new__` returns non-zero, `Alloc` returns that code unchanged."""

        @component(cid=CID_DEFAULT)
        class Product:
            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0x1234

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 0

        Factory = make_factory(Product)
        inst = Factory()
        iid = UGUID(IID_X)
        out = Ptr[Void]()

        result = self.alloc(inst, unknown, None, pointer(iid), pointer(out))

        assert result == 0x1234

    def test_alloc_propagates_eco_init_failure(self, unknown: Ptr[IEcoUnknown]) -> None:
        """If `__eco_init__` returns non-zero, `Alloc` returns that code unchanged."""

        @component(cid=CID_DEFAULT)
        class Product:
            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0x5678

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 0

        Factory = make_factory(Product)
        inst = Factory()
        iid = UGUID(IID_X)
        out = Ptr[Void]()

        result = self.alloc(inst, unknown, None, pointer(iid), pointer(out))

        assert result == 0x5678

    def test_alloc_returns_nointerface_for_unknown_iid(
        self, factory_cls: type, unknown: Ptr[IEcoUnknown]
    ) -> None:
        """If the product does not implement the requested IID, `Alloc` returns `NOINTERFACE`."""

        inst = factory_cls()
        bogus = UGUID("DEADBEEF-DEAD-BEEF-DEAD-BEEFDEADBEEF")
        out = Ptr[Void]()

        result = self.alloc(inst, unknown, None, pointer(bogus), pointer(out))

        assert result == EcoErrorCode.NOINTERFACE


@pytest.mark.unit
class TestFactoryAccessors:
    """Verifies `Init` and the metadata getters."""

    def call(self, inst: Any, name: str) -> Any:
        """Invoke an `IEcoComponentFactory` method directly on the instance."""
        return inst._eco_views_[IID_IEcoComponentFactory].__dict__[name]

    def test_init_returns_success(self, factory_cls: type) -> None:
        """`Init(system, context)` is a no-op that returns `SUCCESS`."""

        inst = factory_cls()

        result = self.call(inst, "Init")(inst, None, Ptr[Void]())

        assert result == EcoErrorCode.SUCCESS

    def test_get_name_returns_metadata(self, factory_cls: type) -> None:
        """`get_Name` returns the `name` field set at `__init__`."""

        inst = factory_cls()

        assert self.call(inst, "get_Name")(inst) == b"Test.Component"

    def test_get_version_returns_metadata(self, factory_cls: type) -> None:
        """`get_Version` returns the `version` field set at `__init__`."""

        inst = factory_cls()

        assert self.call(inst, "get_Version")(inst) == b"0.0.0.0"

    def test_get_manufacturer_returns_metadata(self, factory_cls: type) -> None:
        """`get_Manufacturer` returns the `manufacturer` field set at `__init__`."""

        inst = factory_cls()

        assert self.call(inst, "get_Manufacturer")(inst) == b"Test"


@pytest.mark.unit
class TestFactoryExport:
    """Verifies `export(cls)` glue used by the Python loader."""

    def test_export_returns_singleton_and_getter(self, factory_cls: type) -> None:
        """`export` returns `(instance, getter)` and the instance is a real factory."""

        instance, get_component_factory = export(factory_cls)

        assert isinstance(instance, factory_cls)
        assert callable(get_component_factory)

    def test_get_component_factory_returns_typed_pointer(self, factory_cls: type) -> None:
        """The returned pointer is `Ptr[IEcoComponentFactory]` aimed at the singleton's view."""

        instance, get_component_factory = export(factory_cls)

        ptr = get_component_factory()

        assert ptr.value == pointer(instance, IEcoComponentFactory, shift=True).value

    def test_export_on_non_factory_raises(self) -> None:
        """`export` rejects classes that are not `@factory`-decorated."""

        class NotAFactory:
            pass

        with pytest.raises(TypeError, match="is not a factory"):
            export(NotAFactory)
