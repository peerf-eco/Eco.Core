"""`Eco.BinarySearch` — connection-point demonstration component.

Implements `IEcoBinarySearch` (the search) and `IEcoConnectionPointContainer`
(two outgoing connection points), held in an `IEcoList1`:
    - `IEcoBinarySearchEvents` — per-step trace fired as the search runs.
    - `IEcoBinarySearchStats` — summary telemetry fired once at the end.

`FindConnectionPoint` resolves a point by IID. `EnumConnectionPoints` snapshots
the list into an `EcoBinarySearchEnumConnectionPoints` enumerator.
"""

from collections.abc import Callable
from time import perf_counter

try:
    from connection_point import EcoBinarySearchConnectionPoint
    from enum_points import EcoBinarySearchEnumConnectionPoints
    from interfaces import (
        IEcoBinarySearch,
        IEcoBinarySearchEvents,
        IEcoBinarySearchStats,
        IID_IEcoBinarySearchEvents,
        IID_IEcoBinarySearchStats,
    )
    from utils import AcquireList
except ImportError:
    from examples.server.points.connection_point import EcoBinarySearchConnectionPoint
    from examples.server.points.enum_points import EcoBinarySearchEnumConnectionPoints
    from examples.server.points.interfaces import (
        IEcoBinarySearch,
        IEcoBinarySearchEvents,
        IEcoBinarySearchStats,
        IID_IEcoBinarySearchEvents,
        IID_IEcoBinarySearchStats,
    )
    from examples.server.points.utils import AcquireList

from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.connections import (
    EcoConnectionData,
    IEcoConnectionPoint,
    IEcoConnectionPointContainer,
    IEcoEnumConnectionPoints,
    IEcoEnumConnections,
)
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.list import IEcoList1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, byref, cast

CID_EcoBinarySearch = UGUID("6E1F4C5A-9D38-4B27-B1A4-8C6D2E5F7A39")


# =============================================================================
# EcoBinarySearch — main component
# =============================================================================


@component(cid=CID_EcoBinarySearch)
class EcoBinarySearch:
    """Binary search component with two outgoing connection points."""

    system: Ptr[IEcoSystem1]
    points: Ptr[IEcoList1]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = Ptr[IEcoSystem1]()
        self.points = Ptr[IEcoList1]()
        if not bool(system):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return EcoErrorCode.SUCCESS

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — build the points list and create both points."""
        result = AcquireList(self.system, pointer(self.points))
        if result != 0 or not bool(self.points):
            return result

        container = pointer(self, IEcoConnectionPointContainer, shift=True)
        for iid in (IID_IEcoBinarySearchEvents, IID_IEcoBinarySearchStats):
            point = EcoBinarySearchConnectionPoint()
            result = point.__eco_new__(system, container, pointer(iid))
            if result != 0:
                return result
            ALIVE[addressof(point)] = point
            point_ptr = pointer(point, IEcoConnectionPoint, shift=True)
            self.points.obj.Add(cast(point_ptr, Ptr[Void]))

        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release every owned point, the list and system pointer."""
        if bool(self.points):
            for idx in range(self.points.obj.Count()):
                slot_ptr = self.points.obj.Item(idx)
                if bool(slot_ptr):
                    slot = cast(slot_ptr, Ptr[IEcoConnectionPoint])
                    slot.obj.Release()
            self.points.obj.Clear()
            self.points.obj.Release()
        if bool(self.system):
            self.system.obj.Release()

    def _fire_on_start(self, array: Ptr[Int32], length: Int32) -> None:
        """Notify every events sink that the search is about to start.

        Args:
            array: Pointer to the first element of the array under search.
            length: Number of elements in the array.
        """
        self._fire(
            IID_IEcoBinarySearchEvents,
            IEcoBinarySearchEvents,
            lambda events: events.OnStart(array, length),
        )

    def _fire_on_step(
        self, array: Ptr[Int32], length: Int32, left: Int32, right: Int32, mid: Int32, value: Int32
    ) -> None:
        """Notify every events sink about a single probe inside the loop.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.
            left: Current left bound (inclusive).
            right: Current right bound (inclusive).
            mid: Index being probed on this step.
            value: Array value at `mid`.
        """
        self._fire(
            IID_IEcoBinarySearchEvents,
            IEcoBinarySearchEvents,
            lambda events: events.OnStep(array, length, left, right, mid, value),
        )

    def _fire_on_final(self, array: Ptr[Int32], length: Int32, index: Int32) -> None:
        """Notify every events sink about the resolved index after the loop.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.
            index: Resolved index, or `-1` when nothing was found.
        """
        self._fire(
            IID_IEcoBinarySearchEvents,
            IEcoBinarySearchEvents,
            lambda events: events.OnFinal(array, length, index),
        )

    def _fire_stats(self, steps: Int32, elapsed: Int32) -> None:
        """Notify every stats sink with the search's summary telemetry.

        Args:
            steps: Number of loop iterations (comparisons) performed.
            elapsed: Pure algorithm time in microseconds.
        """
        self._fire(
            IID_IEcoBinarySearchStats,
            IEcoBinarySearchStats,
            lambda stats: stats.OnComparisons(steps),
        )
        self._fire(
            IID_IEcoBinarySearchStats,
            IEcoBinarySearchStats,
            lambda stats: stats.OnTiming(elapsed),
        )

    def _fire(self, iid: UGUID, iface: type, call: Callable[[object], Int16]) -> None:
        """Walk the sinks of the point serving `iid` and invoke `call` on each.

        Args:
            iid: Outgoing interface the target point serves.
            iface: Interface class to reinterpret each resolved sink as.
            call: Callable receiving the resolved sink view (queried for `iid`)
                that performs the notification.
        """
        container = pointer(self, IEcoConnectionPointContainer, shift=True)
        point = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(iid), byref(point))
        if result != 0 or not bool(point):
            return

        try:
            enum = Ptr[IEcoEnumConnections]()
            result = point.obj.EnumConnections(byref(enum))
            if result != 0 or not bool(enum):
                return

            try:
                data = EcoConnectionData()
                fetched = UInt32()
                while enum.obj.Next(1, byref(data), byref(fetched)) == 0:
                    unknown = data.ptr
                    if not bool(unknown):
                        break
                    sink_ptr = Ptr[Void]()
                    result = unknown.obj.QueryInterface(byref(iid), byref(sink_ptr))
                    if result == 0 and bool(sink_ptr):
                        sink = cast(sink_ptr, Ptr[iface])
                        call(sink.obj)
                        sink.obj.Release()
                    unknown.obj.Release()
            finally:
                enum.obj.Release()
        finally:
            point.obj.Release()

    @view
    class Search(IEcoBinarySearch):
        """`IEcoBinarySearch` view — performs the search and emits events."""

        def BinarySearch(self, array: Ptr[Int32], length: Int32, target: Int32) -> Int32:
            """Run a classic iterative binary search over `array`.

            Args:
                array: Pointer to the first element of the sorted array.
                length: Number of elements in the array.
                target: Value to look for.

            Returns:
                Zero-based index of the match, or `-1` when not found.
            """
            self._fire_on_start(array, length)

            # Accumulate only the algorithm's own compute time
            left, right, found, steps, elapsed = 0, length - 1, -1, 0, 0.0
            while left <= right:
                tick = perf_counter()
                steps += 1
                mid = (left + right) // 2
                value = array[mid]
                done = value == target
                elapsed += perf_counter() - tick

                self._fire_on_step(array, length, left, right, mid, value)
                if done:
                    found = mid
                    break

                tick = perf_counter()
                if value < target:
                    left = mid + 1
                else:
                    right = mid - 1
                elapsed += perf_counter() - tick

            self._fire_on_final(array, length, found)
            self._fire_stats(steps, round(elapsed * 1_000_000))
            return found

    @view
    class Container(IEcoConnectionPointContainer):
        """`IEcoConnectionPointContainer` view — `FindConnectionPoint`/`EnumConnectionPoints`."""

        def FindConnectionPoint(
            self, iid: Ptr[UGUID], point: Ptr[Ptr[IEcoConnectionPoint]]
        ) -> Int16:
            """Return the connection point serving the requested outgoing IID.

            Args:
                iid: IID of the requested outgoing interface.
                point: Output pointer receiving the connection point interface.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(iid) or not bool(point) or not bool(self.points):
                return EcoErrorCode.POINTER

            probe = UGUID()
            for idx in range(self.points.obj.Count()):
                slot_ptr = self.points.obj.Item(idx)
                if not bool(slot_ptr):
                    continue
                candidate = cast(slot_ptr, Ptr[IEcoConnectionPoint])
                candidate.obj.GetConnectionInterface(byref(probe))
                if probe == iid.obj:
                    candidate.obj.AddRef()
                    point.obj = candidate
                    return EcoErrorCode.SUCCESS

            return EcoErrorCode.NOINTERFACE

        def EnumConnectionPoints(self, enum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
            """Hand back an enumerator with a snapshot of the container's points.

            Args:
                enum: Output pointer receiving the new enumerator.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(enum) or not bool(self.points):
                return EcoErrorCode.POINTER

            new_enum = EcoBinarySearchEnumConnectionPoints()
            result = new_enum.__eco_new__(cast(self.system, Ptr[IEcoUnknown]), self.points)
            if result != 0:
                return result
            ALIVE[addressof(new_enum)] = new_enum
            enum.obj = pointer(new_enum, IEcoEnumConnectionPoints, shift=True)
            return EcoErrorCode.SUCCESS


# =============================================================================
# Factory
# =============================================================================


@factory(component=EcoBinarySearch)
class EcoBinarySearchFactory(IEcoComponentFactory):
    """Factory for `Eco.BinarySearch`."""

    name: CString = b"Eco.BinarySearch"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoBinarySearchFactory)
