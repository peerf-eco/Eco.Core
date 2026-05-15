"""`Eco.CalculatorInclusion` — inclusion calculator variant.

Implements both `IEcoCalculatorX` and `IEcoCalculatorY`, but delegates the
`IEcoCalculatorX` arithmetic to an included instance of `Eco.Calculator`.

Inclusion model:
    - own vtables for `IEcoCalculatorX` and `IEcoCalculatorY`;
    - own refcount, no aggregation;
    - the included `IEcoCalculatorX` is acquired in `__eco_init__` via
      `IEcoACOM2Python::QueryComponent` and released in `__eco_del__`.
"""

try:
    from interfaces import IEcoCalculatorX, IEcoCalculatorY, IID_IEcoCalculatorX
    from simple import CID_EcoCalculator
except ImportError:
    from examples.server.calculator.interfaces import (
        IEcoCalculatorX,
        IEcoCalculatorY,
        IID_IEcoCalculatorX,
    )
    from examples.server.calculator.simple import CID_EcoCalculator

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

    system: Ptr[IEcoSystem1]
    inner: Ptr[IEcoCalculatorX]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = Ptr[IEcoSystem1]()
        self.inner = Ptr[IEcoCalculatorX]()
        if not bool(system):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return EcoErrorCode.SUCCESS

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — acquire `IEcoCalculatorX` from the bridge."""
        if not bool(self.system):
            return EcoErrorCode.NOSYSTEM

        bus_ptr = Ptr[Void]()
        result = self.system.obj.QueryInterface(byref(IID_IEcoInterfaceBus1), byref(bus_ptr))
        if result != 0 or not bool(bus_ptr):
            return EcoErrorCode.NOBUS
        bus = cast(bus_ptr, Ptr[IEcoInterfaceBus1])

        bridge_ptr = Ptr[Void]()
        result = bus.obj.QueryInterface(byref(IID_IEcoACOM2Python), byref(bridge_ptr))
        if result != 0 or not bool(bridge_ptr):
            bus.obj.Release()
            return EcoErrorCode.NOPYTHONBRIDGE
        bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])

        inner_ptr = Ptr[Void]()
        result = bridge.obj.QueryComponent(
            byref(CID_EcoCalculator), None, byref(IID_IEcoCalculatorX), byref(inner_ptr)
        )
        bridge.obj.Release()
        bus.obj.Release()
        if result != 0 or not bool(inner_ptr):
            return result
        self.inner = cast(inner_ptr, Ptr[IEcoCalculatorX])

        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the included calculator and the cached system."""
        if bool(self.inner):
            self.inner.obj.Release()
        if bool(self.system):
            self.system.obj.Release()

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
