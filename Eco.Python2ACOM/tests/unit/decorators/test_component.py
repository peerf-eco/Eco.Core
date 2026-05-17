"""Unit tests for the `@component` decorator.

Verifies that `@component` correctly composes a Python class with nested
`@view` declarations into an ACOM component: marker attributes, CID handling,
field layout, lifecycle-hook validation, vtable installation, the generated
`IEcoUnknown` triple, aggregation, and the internal-helper variant.
"""

from typing import Optional, Union

import pytest

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, CStructure, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof

CID_DEFAULT = "11111111-1111-1111-1111-111111111111"
IID_X = "ABCDEF12-3456-7890-ABCD-EF1234567890"
IID_Y = "12345678-1234-1234-1234-123456789012"


@interface(iid=IID_X)
class IFooX(IEcoUnknown):
    def Foo(self) -> Int32:
        ...


@interface(iid=IID_Y)
class IFooY(IEcoUnknown):
    def Bar(self) -> Int32:
        ...


def minimal_component(
    cid: Optional[Union[str, UGUID]] = CID_DEFAULT, aggregatable: bool = False
) -> type:
    """Build the smallest valid component with a single view, for reuse across tests."""

    @component(cid=cid, aggregatable=aggregatable)
    class Component:
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

    return Component


@pytest.mark.unit
class TestComponentBasics:
    """Verifies marker attributes and class shape after decoration."""

    def test_decoration_sets_markers_and_base(self) -> None:
        """Decorator sets `_eco_component_`, `_eco_aggregatable_`, `_eco_views_`, `_eco_vtbls_`."""

        Component = minimal_component()

        assert issubclass(Component, CStructure)
        assert Component._eco_component_ is True
        assert Component._eco_aggregatable_ is False
        assert isinstance(Component._eco_views_, dict) and len(Component._eco_views_) == 1
        assert isinstance(Component._eco_vtbls_, dict) and len(Component._eco_vtbls_) == 1
        assert hasattr(Component, "logger")


@pytest.mark.unit
class TestComponentCID:
    """Verifies the three accepted forms of the `cid` parameter."""

    def test_cid_from_string(self) -> None:
        """A string CID is converted into a `UGUID` and stored on `_cid_`."""

        Component = minimal_component(cid=CID_DEFAULT)

        assert isinstance(Component._cid_, UGUID)
        assert Component._cid_.to_string() == CID_DEFAULT

    def test_cid_from_uguid(self) -> None:
        """A pre-built `UGUID` is stored on `_cid_` unchanged."""

        guid = UGUID(CID_DEFAULT)
        Component = minimal_component(cid=guid)

        assert Component._cid_ is guid

    def test_cid_none_marks_internal_helper(self) -> None:
        """`cid = None` produces an internal helper with `_cid_ = None`."""

        @component(cid=None)
        class Helper:
            def __eco_new__(self, *args, **kwargs) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 0

        assert Helper._cid_ is None


@pytest.mark.unit
class TestComponentLifecycleValidation:
    """Verifies that required lifecycle hooks are enforced."""

    def test_missing_eco_new_raises(self) -> None:
        """A component without `__eco_new__` raises `NotImplementedError`."""

        with pytest.raises(NotImplementedError, match="must define '__eco_new__'"):

            @component(cid=CID_DEFAULT)
            class Component:
                def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_del__(self) -> Void:
                    return None

                @view
                class X(IFooX):
                    def Foo(self) -> Int32:
                        return 0

    def test_missing_eco_del_raises(self) -> None:
        """A component without `__eco_del__` raises `NotImplementedError`."""

        with pytest.raises(NotImplementedError, match="must define '__eco_del__'"):

            @component(cid=CID_DEFAULT)
            class Component:
                def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                @view
                class X(IFooX):
                    def Foo(self) -> Int32:
                        return 0

    def test_missing_eco_init_raises_for_full_component(self) -> None:
        """A component with a `cid` must also define `__eco_init__`."""

        with pytest.raises(NotImplementedError, match="must define '__eco_init__'"):

            @component(cid=CID_DEFAULT)
            class Component:
                def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_del__(self) -> Void:
                    return None

                @view
                class X(IFooX):
                    def Foo(self) -> Int32:
                        return 0

    def test_internal_helper_skips_eco_init(self) -> None:
        """An internal helper (no `cid`) does not require `__eco_init__`."""

        @component(cid=None)
        class Helper:
            def __eco_new__(self, *args, **kwargs) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 0

        assert Helper._eco_component_ is True


@pytest.mark.unit
class TestComponentViews:
    """Verifies how views are collected and indexed."""

    def test_no_views_raises(self) -> None:
        """A component without any `@view` declarations is rejected."""

        with pytest.raises(TypeError, match="must declare at least one '@view'"):

            @component(cid=CID_DEFAULT)
            class Component:
                def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_del__(self) -> Void:
                    return None

    def test_two_views_for_same_interface_raises(self) -> None:
        """Two views targeting the same interface are rejected."""

        with pytest.raises(ValueError, match="two views for interface"):

            @component(cid=CID_DEFAULT)
            class Component:
                def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                    return 0

                def __eco_del__(self) -> Void:
                    return None

                @view
                class X1(IFooX):
                    def Foo(self) -> Int32:
                        return 1

                @view
                class X2(IFooX):
                    def Foo(self) -> Int32:
                        return 2

    def test_eco_views_indexed_by_iid(self) -> None:
        """`_eco_views_` maps each interface IID to its view class."""

        @component(cid=CID_DEFAULT)
        class Component:
            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

            @view
            class Y(IFooY):
                def Bar(self) -> Int32:
                    return 2

        assert UGUID(IID_X) in Component._eco_views_
        assert UGUID(IID_Y) in Component._eco_views_


@pytest.mark.unit
class TestComponentLayout:
    """Verifies the order and types of fields produced by the decorator."""

    def test_layout_vtables_then_refs_then_user_fields(self) -> None:
        """Layout is: vtable pointers per view, then `refs: UInt32`, then user fields."""

        @component(cid=CID_DEFAULT)
        class Component:
            user_int: Int32

            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

            @view
            class Y(IFooY):
                def Bar(self) -> Int32:
                    return 2

        names = [name for name, _ in Component._fields_]
        assert names == ["_vtbl_IFooX", "_vtbl_IFooY", "refs", "user_int"]

    def test_refs_is_uint32(self) -> None:
        """The auto-generated `refs` field has type `UInt32`."""

        Component = minimal_component()
        refs_field = next(type for name, type in Component._fields_ if name == "refs")

        assert refs_field is UInt32

    def test_no_outer_when_not_aggregatable(self) -> None:
        """A non-aggregatable component has no `outer` field."""

        Component = minimal_component(aggregatable=False)
        names = [name for name, _ in Component._fields_]

        assert "outer" not in names

    def test_outer_present_when_aggregatable(self) -> None:
        """An aggregatable component has an `outer: Ptr[IEcoUnknown]` field after `refs`."""

        Component = minimal_component(aggregatable=True)
        names = [name for name, _ in Component._fields_]

        assert "outer" in names
        assert names.index("outer") == names.index("refs") + 1


@pytest.mark.unit
class TestComponentInstanceInitialisation:
    """Verifies what the generated `__eco_new__` does to a fresh instance."""

    def test_vtables_pointed_at_singletons_and_refs_is_one(self) -> None:
        """After `__eco_new__`, every `_vtbl_*` matches the cached singleton and `refs == 1`."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)

        for iid, vtbl_singleton in Component._eco_vtbls_.items():
            iface = Component._eco_views_[iid]._eco_iface_
            field_value = getattr(inst, f"_vtbl_{iface.__name__}")
            assert field_value.value == addressof(vtbl_singleton)

        assert inst.refs == 1

    def test_user_eco_new_runs_after_wiring(self) -> None:
        """User code in `__eco_new__` runs after vtables and refs are set."""

        @component(cid=CID_DEFAULT)
        class Component:
            user_int: Int32

            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                self.user_int = 777
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

        inst = Component()
        inst.__eco_new__(None, None)

        assert inst.refs == 1
        assert inst.user_int == 777


@pytest.mark.unit
class TestComponentIUnknownTriple:
    """Verifies the generated `QueryInterface`, `AddRef` and `Release`."""

    def test_addref_increments_and_release_decrements(self) -> None:
        """`AddRef` increments `refs`, `Release` decrements it; both return new value."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)

        assert inst.AddRef() == 2
        assert inst.AddRef() == 3
        assert inst.Release() == 2
        assert inst.Release() == 1

    def test_query_interface_returns_offset_for_known_iid(self) -> None:
        """`QueryInterface` for a registered IID writes the matching vtable offset."""

        @component(cid=CID_DEFAULT)
        class Component:
            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

            @view
            class Y(IFooY):
                def Bar(self) -> Int32:
                    return 2

        inst = Component()
        inst.__eco_new__(None, None)
        out = Ptr[Void]()
        iid = UGUID(IID_Y)

        result = inst.QueryInterface(pointer(iid), pointer(out))

        assert result == EcoErrorCode.SUCCESS
        assert out.value == pointer(inst, IFooY, shift=True).value

    def test_query_interface_iunknown_returns_primary_view(self) -> None:
        """`QueryInterface(IID_IEcoUnknown)` returns the offset of the first registered view."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        out = Ptr[Void]()

        result = inst.QueryInterface(pointer(IID_IEcoUnknown), pointer(out))

        assert result == EcoErrorCode.SUCCESS
        assert out.value == pointer(inst, IFooX, shift=True).value

    def test_query_interface_unknown_iid_returns_nointerface(self) -> None:
        """`QueryInterface` for an unregistered IID returns `NOINTERFACE` and nulls the out."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        unknown = UGUID("DEADBEEF-DEAD-BEEF-DEAD-BEEFDEADBEEF")
        out = Ptr[Void]()

        result = inst.QueryInterface(pointer(unknown), pointer(out))

        assert result == EcoErrorCode.NOINTERFACE
        assert out.value is None

    def test_query_interface_increments_refcount_on_success(self) -> None:
        """A successful `QueryInterface` bumps the refcount by one."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        baseline = inst.refs
        out = Ptr[Void]()
        iid = UGUID(IID_X)

        inst.QueryInterface(pointer(iid), pointer(out))

        assert inst.refs == baseline + 1

    def test_query_interface_does_not_bump_refcount_on_failure(self) -> None:
        """A failed `QueryInterface` leaves the refcount unchanged."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        baseline = inst.refs
        bogus = UGUID("DEADBEEF-DEAD-BEEF-DEAD-BEEFDEADBEEF")
        out = Ptr[Void]()

        inst.QueryInterface(pointer(bogus), pointer(out))

        assert inst.refs == baseline

    def test_query_interface_null_iid_returns_pointer(self) -> None:
        """`QueryInterface(NULL, out)` returns `POINTER`."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        out = Ptr[Void]()

        result = inst.QueryInterface(Ptr[UGUID](), pointer(out))

        assert result == EcoErrorCode.POINTER

    def test_query_interface_null_out_returns_pointer(self) -> None:
        """`QueryInterface(iid, NULL)` returns `POINTER`."""

        Component = minimal_component()
        inst = Component()
        inst.__eco_new__(None, None)
        iid = UGUID(IID_X)

        result = inst.QueryInterface(pointer(iid), Ptr[Ptr[Void]]())

        assert result == EcoErrorCode.POINTER


@pytest.mark.unit
class TestComponentReleaseAtZero:
    """Verifies that `Release` at zero runs `__eco_del__` and clears `ALIVE`."""

    def test_release_at_zero_calls_eco_del_and_evicts_from_alive(self) -> None:
        """`Release` at `refs == 0` invokes `__eco_del__` and removes the instance from `ALIVE`."""

        del_calls: list[bool] = []

        @component(cid=CID_DEFAULT)
        class Component:
            def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
                return 0

            def __eco_del__(self) -> Void:
                del_calls.append(True)
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

        inst = Component()
        inst.__eco_new__(None, None)
        key = addressof(inst)
        ALIVE[key] = inst

        result = inst.Release()

        assert result == EcoErrorCode.SUCCESS
        assert del_calls == [True]
        assert key not in ALIVE


@pytest.mark.unit
class TestComponentAggregation:
    """Verifies aggregation wiring: `outer` field, self-loop, NDU view."""

    def test_aggregatable_marker(self) -> None:
        """`@component(aggregatable=True)` sets the `_eco_aggregatable_` marker."""

        Component = minimal_component(aggregatable=True)

        assert Component._eco_aggregatable_ is True

    def test_outer_self_loop_when_no_external_outer(self) -> None:
        """Without an external outer, `self.outer` points back at the NDU view slot."""

        Component = minimal_component(aggregatable=True)
        inst = Component()
        inst.__eco_new__(None, None)
        ndu_addr = pointer(inst, IEcoUnknown, shift=True)

        assert inst.outer.value == ndu_addr.value

    def test_external_outer_is_stored_unchanged(self) -> None:
        """An external outer pointer passed to `__eco_new__` is stored on `self.outer`."""

        Component = minimal_component(aggregatable=True)
        inst = Component()
        external = Ptr[IEcoUnknown]()
        external.value = 0xDEAD1000  # Arbitrary non-null address
        inst.__eco_new__(None, external)

        assert inst.outer.value == external.value

    def test_aggregatable_adds_ndu_view_under_iunknown_iid(self) -> None:
        """Aggregation registers an internal NDU view under `IID_IEcoUnknown`."""

        Component = minimal_component(aggregatable=True)

        assert IID_IEcoUnknown in Component._eco_views_


@pytest.mark.unit
class TestComponentInternalHelper:
    """Verifies the internal-helper variant (no `cid`)."""

    def test_internal_eco_new_accepts_arbitrary_args(self) -> None:
        """Internal `__eco_new__` is wrapped with `*args, **kwargs` and forwarded as-is."""

        captured: dict = {}

        @component(cid=None)
        class Helper:
            def __eco_new__(self, label: CString, count: Int32 = 0) -> Int16:
                captured["label"] = label
                captured["count"] = count
                return 0

            def __eco_del__(self) -> Void:
                return None

            @view
            class X(IFooX):
                def Foo(self) -> Int32:
                    return 1

        inst = Helper()
        inst.__eco_new__(b"hello", count=5)

        assert captured == {"label": b"hello", "count": 5}
        assert inst.refs == 1
