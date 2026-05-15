"""Shared helpers for the connection-point example."""

from eco_python2acom.guids.iid import IID_IEcoInterfaceBus1, IID_IEcoList1
from eco_python2acom.interfaces.interface_bus import IEcoInterfaceBus1
from eco_python2acom.interfaces.list import IEcoList1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.types.core import Int16, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast

# EcoList1 CID = {53884AFC-93C4-48EC-AA92-9C8D3A562281}
CID_EcoList1 = UGUID("53884AFC-93C4-48EC-AA92-9C8D3A562281")


def AcquireList(system: Ptr[IEcoSystem1], out: Ptr[Ptr[IEcoList1]]) -> Int16:
    """Walk system ---> bus ---> `IEcoList1` and place the resolved pointer in `out`.

    Args:
        system: Resolved `IEcoSystem1` pointer cached by the caller.
        out: Output slot that receives the new `IEcoList1` pointer on success.

    Returns:
        0 on success, error code otherwise.
    """
    if not bool(system):
        return EcoErrorCode.POINTER

    bus_ptr = Ptr[Void]()
    result = system.obj.QueryInterface(byref(IID_IEcoInterfaceBus1), byref(bus_ptr))
    if result != 0 or not bool(bus_ptr):
        return EcoErrorCode.NOBUS
    bus = cast(bus_ptr, Ptr[IEcoInterfaceBus1])

    list_ptr = Ptr[Void]()
    result = bus.obj.QueryComponent(
        byref(CID_EcoList1), None, byref(IID_IEcoList1), byref(list_ptr)
    )
    bus.obj.Release()
    if result != 0 or not bool(list_ptr):
        return result

    out.obj = cast(list_ptr, Ptr[IEcoList1])
    return result
