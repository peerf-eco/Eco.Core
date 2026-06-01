"""`EcoBinarySearchEnumConnectionPoints` — enumerator over connection points.

Holds its own `IEcoList1` populated with the container's connection points and
lets callers walk through them via `Next` / `Skip` / `Reset` / `Clone`.
"""

try:
    from utils import AcquireList
except ImportError:
    from examples.server.points.utils import AcquireList

from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.connections import IEcoConnectionPoint, IEcoEnumConnectionPoints
from eco_python2acom.interfaces.list import IEcoList1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, byref, cast


@component()
class EcoBinarySearchEnumConnectionPoints:
    """Snapshot enumerator over a points list, addressed by `IEcoEnumConnectionPoints`."""

    system: Ptr[IEcoSystem1]
    points: Ptr[IEcoList1]
    index: UInt32

    def __eco_new__(self, system: Ptr[IEcoUnknown], source: Ptr[IEcoList1]) -> Int16:
        """Allocation phase — resolve `IEcoSystem1`, acquire a fresh `IEcoList1`,
        and copy every connection point from `source` into it.

        Args:
            system: System unknown pointer handed in by the caller.
            source: Points list to snapshot.

        Returns:
            0 on success, error code otherwise.
        """
        self.system = Ptr[IEcoSystem1]()
        self.points = Ptr[IEcoList1]()
        self.index = 0
        if not bool(system) or not bool(source):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        result = AcquireList(self.system, pointer(self.points))
        if result != 0 or not bool(self.points):
            return result

        for idx in range(source.obj.Count()):
            slot_ptr = source.obj.Item(idx)
            if not bool(slot_ptr):
                continue
            point = cast(slot_ptr, Ptr[IEcoConnectionPoint])
            point.obj.AddRef()
            self.points.obj.Add(slot_ptr)

        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release every copied point and drop the list."""
        if bool(self.points):
            for idx in range(self.points.obj.Count()):
                slot_ptr = self.points.obj.Item(idx)
                if bool(slot_ptr):
                    slot = cast(slot_ptr, Ptr[IEcoConnectionPoint])
                    slot.obj.Release()
            self.points.obj.Clear()
            self.points.obj.Release()

        self.index = 0
        if bool(self.system):
            self.system.obj.Release()

    @view
    class Enum(IEcoEnumConnectionPoints):
        """`IEcoEnumConnectionPoints` view — cursor-based traversal of the points list."""

        def Next(
            self, count: UInt32, points: Ptr[Ptr[IEcoConnectionPoint]], fetched: Ptr[UInt32]
        ) -> Int16:
            """Yield up to `count` connection points from the current cursor.

            Args:
                count: Maximum number of points to fetch.
                points: Output array receiving the point pointers.
                fetched: Output for the actual number of points returned.

            Returns:
                0 on success (full count delivered), error code when fewer
                points were available.
            """
            if not bool(points) or (not bool(fetched) and count > 1):
                return EcoErrorCode.POINTER

            actual = 0
            while self.index < self.points.obj.Count() and actual < count:
                slot_ptr = self.points.obj.Item(self.index)
                if not bool(slot_ptr):
                    break
                point = cast(slot_ptr, Ptr[IEcoConnectionPoint])
                point.obj.AddRef()
                points[actual] = point
                actual += 1
                self.index += 1

            if bool(fetched):
                fetched.obj = UInt32(actual)
            return EcoErrorCode.FAIL if actual < count else EcoErrorCode.SUCCESS

        def Skip(self, count: UInt32) -> Int16:
            """Advance the cursor by `count` positions without yielding points.

            Args:
                count: Number of points to skip.

            Returns:
                0 on success (full count skipped), error code when the
                cursor hit the end of the list early.
            """
            actual = 0
            while self.index < self.points.obj.Count() and actual < count:
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

        def Clone(self, enum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
            """Create an independent enumerator over a copy of the same list.

            Args:
                enum: Output pointer that receives the cloned enumerator.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(enum):
                return EcoErrorCode.POINTER

            new_enum = EcoBinarySearchEnumConnectionPoints()
            result = new_enum.__eco_new__(cast(self.system, Ptr[IEcoUnknown]), self.points)
            if result != 0:
                return result
            ALIVE[addressof(new_enum)] = new_enum
            enum.obj = pointer(new_enum, IEcoEnumConnectionPoints, shift=True)
            return EcoErrorCode.SUCCESS
