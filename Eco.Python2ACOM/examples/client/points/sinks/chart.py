"""Client-side chart sink for the binary-search events interface.

`EcoBinarySearchChartSink` implements one outgoing view:
    - `Events` (`IEcoBinarySearchEvents`) — the search drawn as a vertical
      ASCII bar chart, with the shrinking `[left, right]` window animated
      in-place (`OnStart` / `OnStep` / `OnFinal`).
"""

from time import sleep

from rich.panel import Panel

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

# Delay in seconds between steps
DELAY = 2.0

# Chart dimensions
HEIGHT = 8

# Column width in characters
WIDTH = 4

# Eighth-block glyphs for smooth bar tops, from empty to full
BLOCKS = " ▁▂▃▄▅▆▇█"

# Sub-row shades per value step
SHADES = len(BLOCKS) - 1


def style(idx: Int32, left: Int32, right: Int32, mid: Int32, found: Int32) -> str:
    """Pick the colour for column `idx` from its role in the current step."""
    if idx == found:
        return "bold green"
    if idx == mid:
        return "bold yellow"
    if left <= idx <= right:
        return "cyan"
    return "grey50"


def render(
    array: Ptr[Int32],
    length: Int32,
    left: Int32 = 0,
    right: Int32 = -1,
    mid: Int32 = -1,
    found: Int32 = -1,
) -> Panel:
    """Render the array as a smooth vertical bar chart inside a bordered panel.

    Args:
        array: Pointer to the first element.
        length: Number of elements to render.
        left: Current left bound (inclusive).
        right: Current right bound (inclusive).
        mid: Index probed this step, drawn in yellow.
        found: Resolved index, drawn in green.

    Returns:
        A `Panel` wrapping the multi-line chart.
    """
    if right < 0:
        right = length - 1

    values = [array[idx] for idx in range(length)]
    peak = max(values) if values else 1
    shades = [round(value / peak * HEIGHT * SHADES) for value in values]

    def cell(idx: int, glyph: str) -> str:
        color = style(idx, left, right, mid, found)
        return f"[{color}]{glyph:^{WIDTH}}[/{color}]"

    rows = []
    for level in range(HEIGHT, 0, -1):
        cells = []
        for idx in range(length):
            remaining = shades[idx] - (level - 1) * SHADES
            glyph = BLOCKS[min(SHADES, max(0, remaining))]
            cells.append(cell(idx, glyph))
        rows.append("".join(cells))

    rows.append("".join(cell(idx, str(values[idx])) for idx in range(length)))

    markers = []
    for idx in range(length):
        if idx == found:
            markers.append(cell(idx, "▲"))
        elif idx == mid:
            markers.append(cell(idx, "▲"))
        elif idx == left == right:
            markers.append(cell(idx, "▮"))
        elif idx == left:
            markers.append(cell(idx, "L"))
        elif idx == right:
            markers.append(cell(idx, "R"))
        else:
            markers.append(" " * WIDTH)
    rows.append("".join(markers))

    return Panel.fit("\n".join(rows), border_style="blue", padding=(1, 2))


@component()
class EcoBinarySearchChartSink:
    """Client sink that animates `OnStart`/`OnStep`/`OnFinal` as a bar chart."""

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
        """`IEcoBinarySearchEvents` view — the search drawn as an animated chart."""

        def OnStart(self, array: Ptr[Int32], length: Int32) -> Int16:
            """Open the live region and draw the initial bar chart."""
            sleep(DELAY)
            console.live(render(array, length), start=True)
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
            """Update the live region with the current window and probed index."""
            sleep(DELAY)
            console.live(render(array, length, left=left, right=right, mid=mid))
            return EcoErrorCode.SUCCESS

        def OnFinal(self, array: Ptr[Int32], length: Int32, index: Int32) -> Int16:
            """Draw the final chart, then close the live region."""
            sleep(DELAY)
            console.live(render(array, length, found=index), stop=True)
            return EcoErrorCode.SUCCESS
