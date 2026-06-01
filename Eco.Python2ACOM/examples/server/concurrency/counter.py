"""`Eco.Counter` — a shared-state Python component for the concurrency example.

Implements `IEcoCounter`: a single shared integer with an unsynchronised
increment, a lock-guarded one, and a blocking one.
"""

import threading
from time import sleep

try:
    from interfaces import IEcoCounter
except ImportError:
    from examples.server.concurrency.interfaces import IEcoCounter

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast

CID_EcoCounter = UGUID("3F8B2D14-6A57-4C90-B2E8-1D5C7A9F4B60")


@component(cid=CID_EcoCounter)
class EcoCounter:
    """Shared-counter component implementing `IEcoCounter`."""

    system: Ptr[IEcoSystem1]
    value: Int32

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — resolve `IEcoSystem1` and set up the shared value with lock."""
        self.value = 0
        self.lock = threading.Lock()
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
    class Counter(IEcoCounter):
        """`IEcoCounter` view — unsynchronised, lock-guarded and blocking increments."""

        def Increment(self) -> Int16:
            """Read-modify-write the shared value with no lock."""
            current = self.value
            sleep(0)
            self.value = current + 1
            return EcoErrorCode.SUCCESS

        def IncrementSafe(self) -> Int16:
            """Read-modify-write the shared value with a lock."""
            with self.lock:
                current = self.value
                sleep(0)
                self.value = current + 1
            return EcoErrorCode.SUCCESS

        def IncrementBlock(self, milliseconds: UInt32) -> Int16:
            """Block and increment the shared value under a lock."""
            sleep(milliseconds / 1000.0)
            with self.lock:
                self.value += 1
            return EcoErrorCode.SUCCESS

        def GetValue(self) -> Int32:
            """Return the current value of the shared counter."""
            return self.value

        def Reset(self) -> Int16:
            """Reset the shared value back to zero."""
            with self.lock:
                self.value = 0
            return EcoErrorCode.SUCCESS


@factory(component=EcoCounter)
class EcoCounterFactory(IEcoComponentFactory):
    """Factory for `Eco.Counter`."""

    name: CString = b"Eco.Counter"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoCounterFactory)
