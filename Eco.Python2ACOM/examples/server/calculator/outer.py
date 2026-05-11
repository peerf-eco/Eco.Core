"""`Eco.CalculatorOuter` — outer aggregator calculator variant.

Implements `IEcoCalculatorY` directly and aggregates an `Eco.CalculatorInner`
to expose `IEcoCalculatorX`.

Aggregation model ("outer"):
    - own vtable for `IEcoCalculatorY`;
    - custom `QueryInterface` answers `IEcoCalculatorY/IEcoUnknown` locally and forwards anything
      else to the inner's non-delegating `IEcoUnknown`;
    - custom `Release` releases the inner first to break the controlling cycle;
    - the inner is acquired in `__eco_init__` via `IEcoACOM2Python::QueryComponent`
      with `outer=self` and released in `__eco_del__`.
"""

from typing import Optional

from inner import CID_EcoCalculatorInner
from interfaces import IEcoCalculatorY, IID_IEcoCalculatorY

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.guids.iid import IID_IEcoACOM2Python, IID_IEcoUnknown
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof, byref, cast, offsetof

CID_EcoCalculatorOuter = UGUID("872FEFD1-E331-4887-AD44-D1E7C232C2F0")


@component(cid=CID_EcoCalculatorOuter)
class EcoCalculatorOuter:
    """Aggregated calculator implementing `IEcoCalculatorY`."""

    system: Optional[Ptr[IEcoSystem1]]
    inner: Optional[Ptr[IEcoUnknown]]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = None
        self.inner = None
        if not bool(system):
            return Int16(EcoErrorCode.POINTER)

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result.value != 0 or not system_ptr.value:
            return Int16(EcoErrorCode.NOSYSTEM)
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return Int16(EcoErrorCode.SUCCESS)

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — aggregate the inner under our own identity."""
        if not bool(self.system):
            return Int16(EcoErrorCode.NOSYSTEM)

        bridge_ptr = Ptr[Void]()
        result = self.system.obj.QueryInterface(byref(IID_IEcoACOM2Python), byref(bridge_ptr))
        if result.value != 0 or not bridge_ptr.value:
            return Int16(EcoErrorCode.NOPYTHONBRIDGE)
        bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])

        # Pass *ourselves* as the outer so the inner's delegating triple loops
        # back through our `QueryInterface` / `AddRef` / `Release`.
        outer_self = cast(addressof(self), Ptr[IEcoUnknown])

        inner_ptr = Ptr[Void]()
        result = bridge.obj.QueryComponent(
            byref(CID_EcoCalculatorInner), outer_self, byref(IID_IEcoUnknown), byref(inner_ptr)
        )
        bridge.obj.Release()
        if result.value != 0 or not inner_ptr.value:
            return result
        self.inner = cast(inner_ptr, Ptr[IEcoUnknown])

        return Int16(EcoErrorCode.SUCCESS)

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the cached `IEcoSystem1` pointer."""
        if bool(self.system):
            self.system.obj.Release()
        self.system = None

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for another interface on this component.

        Answers `IEcoCalculatorY` / `IEcoUnknown` locally; any other IID is
        forwarded to the aggregated inner so callers can reach `IEcoCalculatorX`
        through it.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out):
            return Int16(EcoErrorCode.POINTER)

        target = iid.obj
        if target == IID_IEcoUnknown or target == IID_IEcoCalculatorY:
            out.obj.value = addressof(self) + offsetof(self, IEcoCalculatorY)
            self.AddRef()
            return Int16(EcoErrorCode.SUCCESS)

        if bool(self.inner):
            return self.inner.obj.QueryInterface(iid, out)

        out.obj.value = 0
        return Int16(EcoErrorCode.NOINTERFACE)

    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        self.refs += 1
        return self.refs

    def Release(self) -> UInt32:
        """Decrement the reference count.

        When the count reaches zero, release inner first to break the cycle.

        Returns:
            The new reference count.
        """
        self.refs -= 1
        if self.refs == 0:
            if bool(self.inner):
                if self.inner.obj.Release() != 0:
                    self.refs = 1
                else:
                    self.inner = None
            if self.refs == 0:
                self.__eco_del__()
        return self.refs

    @view
    class Y(IEcoCalculatorY):
        """`IEcoCalculatorY` view — local arithmetic."""

        def Multiplication(self, a: Int16, b: Int16) -> Int32:
            """Return the product of two integers."""
            return a * b

        def Division(self, a: Int16, b: Int16) -> Int16:
            """Return the integer quotient of two integers."""
            return a // b


@factory(component=EcoCalculatorOuter)
class EcoCalculatorOuterFactory(IEcoComponentFactory):
    """Factory for `Eco.CalculatorOuter`."""

    name: CString = b"Eco.CalculatorOuter"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoCalculatorOuterFactory)
