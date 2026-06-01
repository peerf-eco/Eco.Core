"""Client-side trace sink subscribing to both binary-search interfaces.

`EcoBinarySearchTraceSink` implements two outgoing views:
    - `Events` (`IEcoBinarySearchEvents`) — per-step trace lines as the search
      runs (`OnStart` / `OnStep` / `OnFinal`);
    - `Stats` (`IEcoBinarySearchStats`) — a short summary printed at the end,
      one line per metric (`OnComparisons` / `OnTiming`).
"""

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.iid import IID_IEcoConnectionPointContainer
from eco_python2acom.interfaces.connections import IEcoConnectionPoint, IEcoConnectionPointContainer
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import byref, cast
from examples.client.console import console
from examples.server.points.interfaces import (
    IEcoBinarySearch,
    IEcoBinarySearchEvents,
    IEcoBinarySearchStats,
    IID_IEcoBinarySearchEvents,
    IID_IEcoBinarySearchStats,
)


def render(array: Ptr[Int32], length: Int32, highlight: Int32 = -1) -> str:
    """Render the array as a coloured single-line string for the trace.

    Args:
        array: Pointer to the first element.
        length: Number of elements to render.
        highlight: Optional index drawn in green.

    Returns:
        A rendered array string.
    """
    parts = []
    for idx in range(length):
        value = array[idx]
        if idx == highlight:
            parts.append(f"[bold green]{value}[/bold green]")
        else:
            parts.append(f"[cyan]{value}[/cyan]")
    return "[" + ", ".join(parts) + "]"


@component()
class EcoBinarySearchTraceSink:
    """Client sink that traces events and prints the final search statistics."""

    cookie_events: UInt32
    cookie_stats: UInt32

    def __eco_new__(self) -> Int16:
        """Allocation phase — initialize both connection cookies."""
        self.cookie_events = 0
        self.cookie_stats = 0
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — reset both connection cookies."""
        self.cookie_events = 0
        self.cookie_stats = 0

    def Advise(self, search: Ptr[IEcoBinarySearch]) -> Int16:
        """Attach this sink to both the events and stats connection points.

        Args:
            search: Pointer to the binary-search component to subscribe to.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(search):
            return EcoErrorCode.POINTER

        container_ptr = Ptr[Void]()
        result = search.obj.QueryInterface(
            byref(IID_IEcoConnectionPointContainer), byref(container_ptr)
        )
        if result != 0 or not bool(container_ptr):
            return EcoErrorCode.NOINTERFACE
        container = cast(container_ptr, Ptr[IEcoConnectionPointContainer])
        sink = pointer(self, IEcoUnknown)

        # --------------------------- Events ---------------------------
        events = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchEvents), byref(events))
        if result != 0 or not bool(events):
            container.obj.Release()
            return EcoErrorCode.NOINTERFACE
        cookie = UInt32()
        result = events.obj.Advise(sink, byref(cookie))
        events.obj.Release()
        if result != 0:
            container.obj.Release()
            return result
        self.cookie_events = cookie.value

        # --------------------------- Stats ---------------------------
        stats = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchStats), byref(stats))
        if result != 0 or not bool(stats):
            container.obj.Release()
            return EcoErrorCode.NOINTERFACE
        cookie = UInt32()
        result = stats.obj.Advise(sink, byref(cookie))
        stats.obj.Release()
        if result != 0:
            container.obj.Release()
            return result
        self.cookie_stats = cookie.value

        container.obj.Release()
        return EcoErrorCode.SUCCESS

    def Unadvise(self, search: Ptr[IEcoBinarySearch]) -> Int16:
        """Detach this sink from both connection points.

        Args:
            search: Pointer to the binary-search component to unsubscribe from.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(search):
            return EcoErrorCode.POINTER

        container_ptr = Ptr[Void]()
        result = search.obj.QueryInterface(
            byref(IID_IEcoConnectionPointContainer), byref(container_ptr)
        )
        if result != 0 or not bool(container_ptr):
            return EcoErrorCode.NOINTERFACE
        container = cast(container_ptr, Ptr[IEcoConnectionPointContainer])

        # --------------------------- Events ---------------------------
        events = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchEvents), byref(events))
        if result == 0 and bool(events):
            events.obj.Unadvise(self.cookie_events)
            events.obj.Release()
        self.cookie_events = 0

        # --------------------------- Stats ---------------------------
        stats = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchStats), byref(stats))
        if result == 0 and bool(stats):
            stats.obj.Unadvise(self.cookie_stats)
            stats.obj.Release()
        self.cookie_stats = 0

        container.obj.Release()
        return EcoErrorCode.SUCCESS

    @view
    class Events(IEcoBinarySearchEvents):
        """`IEcoBinarySearchEvents` view — three coloured trace messages."""

        def OnStart(self, array: Ptr[Int32], length: Int32) -> Int16:
            """Print the initial array before the search begins."""
            console.info(f"[OnStart]  array = {render(array, length)}")
            return EcoErrorCode.SUCCESS

        def OnStep(
            self,
            array: Ptr[Int32],
            length: Int32,
            left: Int32,
            right: Int32,
            mid: Int32,
            value: Int32,
        ) -> Int16:
            """Print one probe step (current bounds, mid index, value at mid)."""
            console.print(
                f"  [OnStep]   [dim]left = {left:<3}  right = {right:<3}  mid = {mid:<3}[/dim]  "
                f"value = [yellow]{value}[/yellow]"
            )
            return EcoErrorCode.SUCCESS

        def OnFinal(self, array: Ptr[Int32], length: Int32, index: Int32) -> Int16:
            """Print the final array with the resolved index highlighted."""
            if index >= 0:
                console.success(
                    f"[OnFinal]  found at index [bold green]{index}[/bold green] "
                    f"---> {render(array, length, highlight=index)}"
                )
            else:
                console.error(f"[OnFinal]  not found ---> {render(array, length)}")
            return EcoErrorCode.SUCCESS

    @view
    class Stats(IEcoBinarySearchStats):
        """`IEcoBinarySearchStats` view — one summary line per metric."""

        def OnComparisons(self, count: Int32) -> Int16:
            """Print the number of comparisons the search performed."""
            console.info(f"[OnComparisons]  comparisons = [magenta]{count}[/magenta]")
            return EcoErrorCode.SUCCESS

        def OnTiming(self, elapsed: Int32) -> Int16:
            """Print the pure algorithm time in microseconds."""
            console.info(f"[OnTiming]       time = [magenta]{elapsed} µs[/magenta]")
            return EcoErrorCode.SUCCESS
