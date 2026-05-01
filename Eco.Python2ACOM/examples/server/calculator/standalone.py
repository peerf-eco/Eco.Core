"""`Eco.CalculatorC` — Python implementation, standalone.

The component implements both `IEcoCalculatorX` and `IEcoCalculatorY` directly
and owns its reference count, with no participation in aggregation.

Aggregation model ("none"):
    - `QueryInterface` matches the IIDs of `IEcoCalculatorX`, `IEcoCalculatorY`, and `IEcoUnknown`;
    - `AddRef` and `Release` operate on the component's own reference counter;
"""

from typing import Optional

from eco_python2acom.decorators.component import component
from eco_python2acom.decorators.factory import factory
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from examples.server.calculator.interfaces import IEcoCalculatorX, IEcoCalculatorY

CID_EcoCalculatorC = UGUID("4828F655-2E45-40E7-8121-EBD220DC360E")


@component(cid=CID_EcoCalculatorC)
class EcoCalculatorC(IEcoCalculatorX, IEcoCalculatorY):
    """Standalone arithmetic component implementing `IEcoCalculatorX` and `IEcoCalculatorY`."""

    refs: UInt32
    system: Optional[Ptr[IEcoSystem1]]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase."""
        self.refs = 0
        self.system = None
        if not bool(system):
            return Int16(EcoErrorCode.POINTER)

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result.value != 0 or not system_ptr.value:
            return Int16(EcoErrorCode.NOSYSTEM)
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return Int16(EcoErrorCode.SUCCESS)

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase."""
        return Int16(EcoErrorCode.SUCCESS)

    def __eco_del__(self) -> Void:
        """Cleanup phase."""
        if bool(self.system):
            self.system.obj.Release()
        self.system = None

    def Addition(self, a: Int16, b: Int16) -> Int32:
        """Return the sum of two integers."""
        return a + b

    def Subtraction(self, a: Int16, b: Int16) -> Int16:
        """Return the difference of two integers."""
        return a - b

    def Multiplication(self, a: Int16, b: Int16) -> Int32:
        """Return the product of two integers."""
        return a * b

    def Division(self, a: Int16, b: Int16) -> Int16:
        """Return the integer quotient of two integers."""
        return a // b


@factory(component=EcoCalculatorC)
class EcoCalculatorCFactory(IEcoComponentFactory):
    """Factory for `Eco.CalculatorC`."""

    refs: UInt32 = 0
    name: CString = "Eco.CalculatorC"
    version: CString = "1.0.0.0"
    manufacturer: CString = "Eco"


def get_component_factory() -> EcoCalculatorCFactory:
    """Module entry point."""
    return EcoCalculatorCFactory()
