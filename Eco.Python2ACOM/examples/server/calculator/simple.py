"""`Eco.Calculator` — standalone calculator variant.

Implements both `IEcoCalculatorX` and `IEcoCalculatorY` directly
and owns its reference count, with no participation in aggregation.

Aggregation model ("none"):
    - `QueryInterface` matches the IIDs of `IEcoCalculatorX`, `IEcoCalculatorY`, and `IEcoUnknown`;
    - `AddRef` and `Release` operate on the component's own reference counter;
"""

try:
    from interfaces import IEcoCalculatorX, IEcoCalculatorY
except ImportError:
    from examples.server.calculator.interfaces import IEcoCalculatorX, IEcoCalculatorY

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast

CID_EcoCalculator = UGUID("4828F655-2E45-40E7-8121-EBD220DC360E")


@component(cid=CID_EcoCalculator)
class EcoCalculator:
    """Standalone calculator implementing `IEcoCalculatorX` and `IEcoCalculatorY`."""

    system: Ptr[IEcoSystem1]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = Ptr[IEcoSystem1]()
        if not bool(system):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return EcoErrorCode.SUCCESS

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — nothing to do for this component."""
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the cached `IEcoSystem1` pointer."""
        if bool(self.system):
            self.system.obj.Release()

    @view
    class X(IEcoCalculatorX):
        """`IEcoCalculatorX` view — addition and subtraction."""

        def Addition(self, a: Int16, b: Int16) -> Int32:
            """Return the sum of two integers."""
            return a + b

        def Subtraction(self, a: Int16, b: Int16) -> Int16:
            """Return the difference of two integers."""
            return a - b

    @view
    class Y(IEcoCalculatorY):
        """`IEcoCalculatorY` view — multiplication and division."""

        def Multiplication(self, a: Int16, b: Int16) -> Int32:
            """Return the product of two integers."""
            return a * b

        def Division(self, a: Int16, b: Int16) -> Int16:
            """Return the integer quotient of two integers."""
            return a // b


@factory(component=EcoCalculator)
class EcoCalculatorFactory(IEcoComponentFactory):
    """Factory for `Eco.Calculator`."""

    name: CString = b"Eco.Calculator"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoCalculatorFactory)
