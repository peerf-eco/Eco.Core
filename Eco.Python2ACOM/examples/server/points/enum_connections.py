"""`EcoBinarySearchEnumConnections` — enumerator helper.

Holds its own `IEcoList1` populated with shallow copies of every source entry
and lets callers walk through them via `Next` / `Skip` / `Reset` / `Clone`.
"""

try:
    from utils import AcquireList
except ImportError:
    from examples.server.points.utils import AcquireList

from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.connections import EcoConnectionData, IEcoEnumConnections
from eco_python2acom.interfaces.list import IEcoList1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, byref, cast


@component()
class EcoBinarySearchEnumConnections:
    """Snapshot enumerator over a sink list, addressed by `IEcoEnumConnections`."""

    system: Ptr[IEcoSystem1]
    sinks: Ptr[IEcoList1]
    index: UInt32

    def __eco_new__(self, system: Ptr[IEcoUnknown], source: Ptr[IEcoList1]) -> Int16:
        """Allocation phase — resolve `IEcoSystem1`, acquire a fresh `IEcoList1`,
        and copy every entry from `source` into it.

        Args:
            system: System unknown pointer handed in by the caller.
            source: Sink list to snapshot.

        Returns:
            0 on success, error code otherwise.
        """
        self.system = Ptr[IEcoSystem1]()
        self.sinks = Ptr[IEcoList1]()
        self.index = 0
        if not bool(system) or not bool(source):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        self.sinks = Ptr[IEcoList1]()
        result = AcquireList(self.system, pointer(self.sinks))
        if result != 0 or not bool(self.sinks):
            return result

        for idx in range(source.obj.Count()):
            slot_ptr = source.obj.Item(idx)
            if not bool(slot_ptr):
                continue
            slot = cast(slot_ptr, Ptr[EcoConnectionData])
            data = EcoConnectionData()
            data.ptr = slot.obj.ptr
            data.cookie = slot.obj.cookie
            data.ptr.obj.AddRef()
            ALIVE[addressof(data)] = data
            self.sinks.obj.Add(pointer(data, Void))

        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release every copied sink and drop the cached references."""
        if bool(self.sinks):
            for idx in range(self.sinks.obj.Count()):
                slot_ptr = self.sinks.obj.Item(idx)
                if bool(slot_ptr):
                    slot = cast(slot_ptr, Ptr[EcoConnectionData])
                    slot.obj.ptr.obj.Release()
                    ALIVE.pop(slot_ptr.value, None)
            self.sinks.obj.Clear()
            self.sinks.obj.Release()

        self.index = 0
        if bool(self.system):
            self.system.obj.Release()

    @view
    class Enum(IEcoEnumConnections):
        """`IEcoEnumConnections` view — cursor-based traversal of the sink list."""

        def Next(self, count: UInt32, data: Ptr[EcoConnectionData], fetched: Ptr[UInt32]) -> Int16:
            """Yield up to `count` connection-data entries from the current cursor.

            Args:
                count: Maximum number of entries to fetch.
                data: Output array receiving the entries.
                fetched: Output for the actual number of entries returned.

            Returns:
                0 on success (full count delivered), error code when fewer
                entries were available.
            """
            if not bool(data) or (not bool(fetched) and count > 1):
                return EcoErrorCode.POINTER

            actual = 0
            while self.index < self.sinks.obj.Count() and actual < count:
                slot_ptr = self.sinks.obj.Item(self.index)
                if not bool(slot_ptr):
                    break
                slot = cast(slot_ptr, Ptr[EcoConnectionData])
                slot.obj.ptr.obj.AddRef()
                data[actual].ptr = slot.obj.ptr
                data[actual].cookie = slot.obj.cookie
                actual += 1
                self.index += 1

            if bool(fetched):
                fetched.obj = UInt32(actual)
            return EcoErrorCode.FAIL if actual < count else EcoErrorCode.SUCCESS

        def Skip(self, count: UInt32) -> Int16:
            """Advance the cursor by `count` positions without yielding entries.

            Args:
                count: Number of entries to skip.

            Returns:
                0 on success (full count skipped), error code when the
                cursor hit the end of the list early.
            """
            actual = 0
            while self.index < self.sinks.obj.Count() and actual < count:
                actual += 1
                self.index += 1

            return EcoErrorCode.FAIL if actual < count else EcoErrorCode.SUCCESS

        def Reset(self) -> Int16:
            """Move the cursor back to the start of the list.

            Returns:
                0 on success.
            """
            self.index = 0
            return EcoErrorCode.SUCCESS

        def Clone(self, enum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
            """Create an independent enumerator over a copy of the same list.

            Args:
                enum: Output pointer that receives the cloned enumerator.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(enum):
                return EcoErrorCode.POINTER

            new_enum = EcoBinarySearchEnumConnections()
            result = new_enum.__eco_new__(cast(self.system, Ptr[IEcoUnknown]), self.sinks)
            if result != 0:
                return result
            ALIVE[addressof(new_enum)] = new_enum
            enum.obj = pointer(new_enum, IEcoEnumConnections, shift=True)
            return EcoErrorCode.SUCCESS
