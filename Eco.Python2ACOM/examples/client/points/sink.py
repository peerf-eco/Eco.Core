"""Client-side sink for `IEcoBinarySearchEvents`.

Each callback renders a single line, so the running search produces
a live trace in the terminal.
"""

from time import sleep

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
    IID_IEcoBinarySearchEvents,
)

SLEEP_SECONDS = 2.5


def print_array(array: Ptr[Int32], length: Int32, highlight: Int32 = -1) -> str:
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
class EcoBinarySearchSink:
    """Client sink that prints `OnStart`/`OnStep`/`OnFinal` events as they arrive."""

    cookie: UInt32

    def __eco_new__(self) -> Int16:
        """Allocation phase — initialize the connection cookie."""
        self.cookie = 0
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — reset the connection cookie."""
        self.cookie = 0

    def Advise(self, search: Ptr[IEcoBinarySearch]) -> Int16:
        """Attach this sink to the binary-search connection point.

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

        point = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchEvents), byref(point))
        container.obj.Release()
        if result != 0 or not bool(point):
            return result

        sink, cookie = pointer(self, IEcoUnknown), UInt32()
        result = point.obj.Advise(sink, byref(cookie))
        point.obj.Release()
        if result != 0:
            return result
        self.cookie = cookie.value
        return result

    def Unadvise(self, search: Ptr[IEcoBinarySearch]) -> Int16:
        """Detach this sink from the binary-search connection point.

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

        point = Ptr[IEcoConnectionPoint]()
        result = container.obj.FindConnectionPoint(byref(IID_IEcoBinarySearchEvents), byref(point))
        container.obj.Release()
        if result != 0 or not bool(point):
            return result

        result = point.obj.Unadvise(self.cookie)
        point.obj.Release()
        self.cookie = 0
        return result

    @view
    class Events(IEcoBinarySearchEvents):
        """`IEcoBinarySearchEvents` view — three coloured trace messages."""

        def OnStart(self, array: Ptr[Int32], length: Int32) -> Int16:
            """Print the initial array before the search begins."""
            sleep(SLEEP_SECONDS)
            console.info(f"[OnStart]  array = {print_array(array, length)}")
            return EcoErrorCode.SUCCESS

        def OnStep(self, left: Int32, right: Int32, mid: Int32, value: Int32) -> Int16:
            """Print one probe step (current bounds, mid index, value at mid)."""
            sleep(SLEEP_SECONDS)
            console.print(
                f"  [OnStep]   [dim]left = {left:<3}  right = {right:<3}  mid = {mid:<3}[/dim]  "
                f"value = [yellow]{value}[/yellow]"
            )
            return EcoErrorCode.SUCCESS

        def OnFinal(self, array: Ptr[Int32], length: Int32, index: Int32) -> Int16:
            """Print the final array with the resolved index highlighted."""
            sleep(SLEEP_SECONDS)
            if index >= 0:
                console.success(
                    f"[OnFinal]  found at index [bold green]{index}[/bold green] "
                    f"---> {print_array(array, length, highlight=index)}"
                )
            else:
                console.error(f"[OnFinal]  not found ---> {print_array(array, length)}")
            return EcoErrorCode.SUCCESS
