"""`Eco.BinarySearch` — connection-point demonstration component.

Implements `IEcoBinarySearch` and `IEcoConnectionPointContainer`. The container
exposes one connection point keyed by `IID_IEcoBinarySearchEvents`. Sinks
attached via `Advise` receive `OnStart`/`OnStep`/`OnFinal` notifications as
the search progresses.

Notes:
    - `EcoBinarySearch` owns one `EcoBinarySearchConnectionPoint` helper;
    - `EcoBinarySearchConnectionPoint` keeps every connection entry inside
      an `IEcoList1` instance obtained from the bus and creates
      `EcoBinarySearchEnumConnections` instances on demand.
"""

from collections.abc import Callable

try:
    from connection_point import EcoBinarySearchConnectionPoint
    from interfaces import IEcoBinarySearch, IEcoBinarySearchEvents, IID_IEcoBinarySearchEvents
except ImportError:
    from examples.server.points.connection_point import EcoBinarySearchConnectionPoint
    from examples.server.points.interfaces import (
        IEcoBinarySearch,
        IEcoBinarySearchEvents,
        IID_IEcoBinarySearchEvents,
    )

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
    """Binary search component with one outgoing connection point."""

    system: Ptr[IEcoSystem1]
    point: Ptr[IEcoConnectionPoint]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = Ptr[IEcoSystem1]()
        self.point = Ptr[IEcoConnectionPoint]()
        if not bool(system):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return EcoErrorCode.SUCCESS

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — spin up the owned connection-point."""
        container = pointer(self, IEcoConnectionPointContainer, shift=True)
        point = EcoBinarySearchConnectionPoint()
        result = point.__eco_new__(system, container, pointer(IID_IEcoBinarySearchEvents))
        if result != 0:
            return result

        ALIVE[addressof(point)] = point
        self.point = pointer(point, IEcoConnectionPoint, shift=True)
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the owned connection-point and system pointer."""
        if bool(self.point):
            self.point.obj.Release()
        if bool(self.system):
            self.system.obj.Release()

    def _fire_on_start(self, array: Ptr[Int32], length: Int32) -> None:
        """Notify every attached sink that the search is about to start.

        Args:
            array: Pointer to the first element of the array under search.
            length: Number of elements in the array.
        """
        self._fire(lambda events: events.OnStart(array, length))

    def _fire_on_step(self, left: Int32, right: Int32, mid: Int32, value: Int32) -> None:
        """Notify every attached sink about a single probe inside the loop.

        Args:
            left: Current left bound (inclusive).
            right: Current right bound (inclusive).
            mid: Index being probed on this step.
            value: Array value at `mid`.
        """
        self._fire(lambda events: events.OnStep(left, right, mid, value))

    def _fire_on_final(self, array: Ptr[Int32], length: Int32, index: Int32) -> None:
        """Notify every attached sink about the resolved index after the loop.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.
            index: Resolved index, or `-1` when nothing was found.
        """
        self._fire(lambda events: events.OnFinal(array, length, index))

    def _fire(self, call: Callable[[IEcoBinarySearchEvents], Int16]) -> None:
        """Walk the connection-point enumerator and invoke `call(events)` for each sink.

        Args:
            call: Callable that receives the resolved
                `IEcoBinarySearchEvents` view and performs the notification.
        """
        if not bool(self.point):
            return

        enum = Ptr[IEcoEnumConnections]()
        result = self.point.obj.EnumConnections(pointer(enum))
        if result != 0 or not bool(enum):
            return

        try:
            data = EcoConnectionData()
            fetched = UInt32()
            while enum.obj.Next(1, byref(data), byref(fetched)) == 0:
                unknown = data.ptr
                if not bool(unknown):
                    break
                events_ptr = Ptr[Void]()
                result = unknown.obj.QueryInterface(
                    byref(IID_IEcoBinarySearchEvents), byref(events_ptr)
                )
                if result == 0 and bool(events_ptr):
                    events = cast(events_ptr, Ptr[IEcoBinarySearchEvents])
                    call(events.obj)
                    events.obj.Release()
                unknown.obj.Release()
        finally:
            enum.obj.Release()

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

            left, right, found = 0, length - 1, -1
            while left <= right:
                mid = (left + right) // 2
                value = array[mid]
                self._fire_on_step(left, right, mid, value)
                if value == target:
                    found = mid
                    break
                if value < target:
                    left = mid + 1
                else:
                    right = mid - 1

            self._fire_on_final(array, length, found)
            return found

    @view
    class Container(IEcoConnectionPointContainer):
        """`IEcoConnectionPointContainer` view — `FindConnectionPoint`/`EnumConnectionPoints`."""

        def FindConnectionPoint(
            self, iid: Ptr[UGUID], point: Ptr[Ptr[IEcoConnectionPoint]]
        ) -> Int16:
            """Return the single connection point when asked for events IID.

            Args:
                iid: IID of the requested outgoing interface.
                point: Output pointer receiving the connection point interface.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(iid) or not bool(point) or not bool(self.point):
                return EcoErrorCode.POINTER
            if iid.obj != IID_IEcoBinarySearchEvents:
                return EcoErrorCode.NOINTERFACE

            self.point.obj.AddRef()
            point.obj = self.point
            return EcoErrorCode.SUCCESS

        def EnumConnectionPoints(self, enum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
            """Not implemented — this demo exposes a single connection point only."""
            return EcoErrorCode.NOTIMPL


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
