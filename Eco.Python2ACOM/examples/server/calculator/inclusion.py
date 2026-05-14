"""`Eco.CalculatorInclusion` — inclusion calculator variant.

Implements both `IEcoCalculatorX` and `IEcoCalculatorY`, but delegates the
`IEcoCalculatorX` arithmetic to an included instance of `Eco.Calculator`.

Inclusion model:
    - own vtables for `IEcoCalculatorX` and `IEcoCalculatorY`;
    - own refcount, no aggregation;
    - the included `IEcoCalculatorX` is acquired in `__eco_init__` via
      `IEcoACOM2Python::QueryComponent` and released in `__eco_del__`.
"""

from typing import Optional

from interfaces import IEcoCalculatorX, IEcoCalculatorY, IID_IEcoCalculatorX
from simple import CID_EcoCalculator

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.guids.iid import IID_IEcoACOM2Python, IID_IEcoInterfaceBus1
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.interface_bus import IEcoInterfaceBus1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast

CID_EcoCalculatorInclusion = UGUID("3A8E4467-7E82-475C-B4A3-719ED8397E61")


@component(cid=CID_EcoCalculatorInclusion)
class EcoCalculatorInclusion:
    """Calculator implementing `IEcoCalculatorY` directly and delegates `IEcoCalculatorX` to an included `Eco.Calculator`."""

    system: Optional[Ptr[IEcoSystem1]]
    inner: Optional[Ptr[IEcoCalculatorX]]

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
        """Initialisation phase — acquire `IEcoCalculatorX` from the bridge."""
        if not bool(self.system):
            return Int16(EcoErrorCode.NOSYSTEM)

        bus_ptr = Ptr[Void]()
        result = self.system.obj.QueryInterface(byref(IID_IEcoInterfaceBus1), byref(bus_ptr))
        if result.value != 0 or not bus_ptr.value:
            return Int16(EcoErrorCode.NOBUS)
        bus = cast(bus_ptr, Ptr[IEcoInterfaceBus1])

        bridge_ptr = Ptr[Void]()
        result = bus.obj.QueryInterface(byref(IID_IEcoACOM2Python), byref(bridge_ptr))
        if result.value != 0 or not bridge_ptr.value:
            bus.obj.Release()
            return Int16(EcoErrorCode.NOPYTHONBRIDGE)
        bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])

        inner_ptr = Ptr[Void]()
        result = bridge.obj.QueryComponent(
            byref(CID_EcoCalculator), None, byref(IID_IEcoCalculatorX), byref(inner_ptr)
        )
        bridge.obj.Release()
        bus.obj.Release()
        if result.value != 0 or not inner_ptr.value:
            return result
        self.inner = cast(inner_ptr, Ptr[IEcoCalculatorX])

        return Int16(EcoErrorCode.SUCCESS)

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the included calculator and the cached system."""
        if bool(self.inner):
            self.inner.obj.Release()
        self.inner = None
        if bool(self.system):
            self.system.obj.Release()
        self.system = None

    @view
    class X(IEcoCalculatorX):
        """`IEcoCalculatorX` view — delegates arithmetic to the included calculator."""

        def Addition(self, a: Int16, b: Int16) -> Int32:
            """Forward addition to the included calculator."""
            return self.inner.obj.Addition(a, b)

        def Subtraction(self, a: Int16, b: Int16) -> Int16:
            """Forward subtraction to the included calculator."""
            return self.inner.obj.Subtraction(a, b)

    @view
    class Y(IEcoCalculatorY):
        """`IEcoCalculatorY` view — arithmetic implemented locally."""

        def Multiplication(self, a: Int16, b: Int16) -> Int32:
            """Return the product of two integers."""
            return a * b

        def Division(self, a: Int16, b: Int16) -> Int16:
            """Return the integer quotient of two integers."""
            return a // b


@factory(component=EcoCalculatorInclusion)
class EcoCalculatorInclusionFactory(IEcoComponentFactory):
    """Factory for `Eco.CalculatorInclusion`."""

    name: CString = b"Eco.CalculatorInclusion"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoCalculatorInclusionFactory)
