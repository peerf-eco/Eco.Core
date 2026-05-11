"""`Eco.CalculatorInner` — aggregatable calculator variant.

Implements only `IEcoCalculatorX`. Designed to be aggregated by an outer
component.

Aggregation model ("inner"):
    - `outer == NULL` → standalone mode, `self.outer` loops back to the
      component's own non-delegating `IEcoUnknown` (NDU);
    - `outer != NULL` → aggregated mode, `self.outer = outer`, and the factory
      hands the caller a pointer to NDU directly.
"""

from typing import Optional

from interfaces import IEcoCalculatorX

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

CID_EcoCalculatorInner = UGUID("AE202E54-3CE5-4550-8996-03BD70C62565")


@component(cid=CID_EcoCalculatorInner, aggregatable=True)
class EcoCalculatorInner:
    """Aggregatable calculator implementing `IEcoCalculatorX`."""

    system: Optional[Ptr[IEcoSystem1]]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
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
        """Initialisation phase — nothing to do for this component."""
        return Int16(EcoErrorCode.SUCCESS)

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the cached `IEcoSystem1` pointer."""
        if bool(self.system):
            self.system.obj.Release()
        self.system = None

    @view
    class X(IEcoCalculatorX):
        """`IEcoCalculatorX` view — addition and subtraction."""

        def Addition(self, a: Int16, b: Int16) -> Int32:
            """Return the sum of two integers."""
            return a + b

        def Subtraction(self, a: Int16, b: Int16) -> Int16:
            """Return the difference of two integers."""
            return a - b


@factory(component=EcoCalculatorInner)
class EcoCalculatorInnerFactory(IEcoComponentFactory):
    """Factory for `Eco.CalculatorInner`."""

    name: CString = b"Eco.CalculatorInner"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoCalculatorInnerFactory)
