"""Shared helpers for the `Eco.NewProject` pattern example."""

from eco_python2acom.guids.iid import (
    IID_IEcoInterfaceBus1,
    IID_IEcoInterfaceBus1MemExt,
    IID_IEcoMemoryAllocator1,
)
from eco_python2acom.interfaces.interface_bus import IEcoInterfaceBus1, IEcoInterfaceBus1MemExt
from eco_python2acom.interfaces.memory_manager import IEcoMemoryAllocator1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.types.core import Int16, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast


def AcquireAllocator(system: Ptr[IEcoSystem1], out: Ptr[Ptr[IEcoMemoryAllocator1]]) -> Int16:
    """Walks system ---> bus ---> memory extension ---> `IEcoMemoryAllocator1` and
    place the resolved pointer in `out`.

    Args:
        system: Resolved `IEcoSystem1` pointer cached by the caller.
        out: Output slot that receives the new `IEcoMemoryAllocator1` pointer on success.

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

    mem_ext_ptr = Ptr[Void]()
    result = bus.obj.QueryInterface(byref(IID_IEcoInterfaceBus1MemExt), byref(mem_ext_ptr))
    if result != 0 or not bool(mem_ext_ptr):
        bus.obj.Release()
        return EcoErrorCode.NOINTERFACE
    mem_ext = cast(mem_ext_ptr, Ptr[IEcoInterfaceBus1MemExt])

    manager_cid = mem_ext.obj.get_Manager()
    mem_ext.obj.Release()
    if not bool(manager_cid):
        bus.obj.Release()
        return EcoErrorCode.NOINTERFACE

    alloc_ptr = Ptr[Void]()
    result = bus.obj.QueryComponent(
        manager_cid, None, byref(IID_IEcoMemoryAllocator1), byref(alloc_ptr)
    )
    bus.obj.Release()
    if result != 0 or not bool(alloc_ptr):
        return result

    out.obj = cast(alloc_ptr, Ptr[IEcoMemoryAllocator1])
    return EcoErrorCode.SUCCESS
