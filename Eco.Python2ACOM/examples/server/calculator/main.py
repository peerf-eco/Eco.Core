"""`Eco.Calculator` — Python implementation.

The component implements both `IEcoCalculatorX` and `IEcoCalculatorY` directly
and owns its reference count, with no participation in aggregation.

Aggregation model ("none"):
    - `QueryInterface` matches the IIDs of `IEcoCalculatorX`, `IEcoCalculatorY`, and `IEcoUnknown`;
    - `AddRef` and `Release` operate on the component's own reference counter;
"""

from eco_python2acom.decorators.component import component
from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.types.core import Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof
from examples.server.calculator.interfaces import (
    IEcoCalculatorX,
    IEcoCalculatorY,
    IID_IEcoCalculatorX,
    IID_IEcoCalculatorY,
)

CID_EcoCalculator = UGUID("4828F655-2E45-40E7-8121-EBD220DC360E")


@component(cid=CID_EcoCalculator)
class EcoCalculator(IEcoCalculatorX, IEcoCalculatorY):
    """Standalone arithmetic component implementing `IEcoCalculatorX` and `IEcoCalculatorY`."""

    def __init__(self) -> Void:
        """Initialize the component and its reference count."""
        self.refs: UInt32 = 0

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for another interface on this component.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out):
            return Int16(EcoErrorCode.POINTER)

        if iid.obj in (IID_IEcoCalculatorX, IID_IEcoCalculatorY, IID_IEcoUnknown):
            out.obj.value = addressof(self)
            self.AddRef()
            return Int16(EcoErrorCode.SUCCESS)

        out.obj.value = 0
        return Int16(EcoErrorCode.NOINTERFACE)

    def AddRef(self) -> UInt32:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        self.refs += 1
        return UInt32(self.refs)

    def Release(self) -> UInt32:
        """Decrement the reference count.

        When the count reaches zero, the component is freed.

        Returns:
            The new reference count.
        """
        self.refs -= 1
        if self.refs == 0:
            pass
        return UInt32(self.refs)

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
