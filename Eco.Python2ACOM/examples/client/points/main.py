"""Binary-search connection-point example (many-to-many).

The host:
    1. Boots `EcoSystem` and reaches the Python bridge through the bus.
    2. Registers `Eco.BinarySearch` (server-side example) with the bridge.
    3. Queries `IEcoBinarySearch`.
    4. Creates two sinks — `EcoBinarySearchTraceSink`, which subscribes to both
       outgoing interfaces (events + stats), and `EcoBinarySearchChartSink`,
       which subscribes only to events — and attaches them.
    5. Runs `BinarySearch` once; events flow to both sinks, the final stats flow
       only to the trace sink (many points -> many sinks).
"""

import sys
from pathlib import Path

from eco_python2acom.guids.iid import IID_IEcoACOM2Python
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import Int32, Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from examples.client.console import console
from examples.client.points.sinks.chart import EcoBinarySearchChartSink
from examples.client.points.sinks.trace import EcoBinarySearchTraceSink
from examples.server.points.interfaces import (
    IEcoBinarySearch,
    IID_IEcoBinarySearch,
    IID_IEcoBinarySearchEvents,
    IID_IEcoBinarySearchStats,
)
from examples.server.points.search import CID_EcoBinarySearch

SERVER_PATH = str(Path(__file__).resolve().parent.parent.parent / "server" / "points" / "search.py")


def main() -> int:
    """Run the binary-search connection-point example."""
    console.header("Eco BinarySearch Example")
    console.info(f"CID:                    {CID_EcoBinarySearch}")
    console.info(f"IEcoBinarySearch:       {IID_IEcoBinarySearch}")
    console.info(f"IEcoBinarySearchEvents: {IID_IEcoBinarySearchEvents}")
    console.info(f"IEcoBinarySearchStats:  {IID_IEcoBinarySearchStats}\n")

    try:
        console.info("Initializing EcoSystem...")

        with EcoSystem(lib_dir="data") as eco:
            console.success("EcoSystem initialized\n")

            # --------------------------- Bridge ---------------------------
            bridge_ptr = Ptr[Void]()
            result = eco.bus.obj.QueryInterface(byref(IID_IEcoACOM2Python), byref(bridge_ptr))
            if result != 0 or not bool(bridge_ptr):
                console.error(f"Failed to get bridge (code = {result})")
                return -2
            bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])
            console.success(f"Got {bridge}")

            result = bridge.obj.RegisterComponent(SERVER_PATH.encode(), byref(CID_EcoBinarySearch))
            if result != 0:
                console.error(f"Failed to register component (code = {result})")
                bridge.obj.Release()
                return -3

            # ------------------------- Component --------------------------
            search_ptr = Ptr[Void]()
            result = bridge.obj.QueryComponent(
                byref(CID_EcoBinarySearch), None, byref(IID_IEcoBinarySearch), byref(search_ptr)
            )
            if result != 0 or not bool(search_ptr):
                console.error(f"Failed to query component (code = {result})")
                bridge.obj.Release()
                return -4
            search = cast(search_ptr, Ptr[IEcoBinarySearch])
            console.success(f"Got {search}\n")

            # ---------------------------- Sinks ---------------------------
            trace_sink = EcoBinarySearchTraceSink()
            trace_sink.__eco_new__()
            chart_sink = EcoBinarySearchChartSink()
            chart_sink.__eco_new__()

            for label, sink in (("chart", chart_sink), ("trace", trace_sink)):
                result = sink.Advise(search)
                if result != 0:
                    console.error(f"Advise ({label}) failed (code = {result})")
                    trace_sink.Unadvise(search)
                    chart_sink.Unadvise(search)
                    search.obj.Release()
                    bridge.obj.Release()
                    return -5
                console.success(f"Advise ({label}) sink attached")

            # --------------------------- Search ---------------------------
            data = Array[Int32, 10](1, 3, 5, 7, 9, 11, 13, 15, 17, 19)
            target = 13
            console.info(f"Searching for <{target}> in the array...")
            found = search.obj.BinarySearch(data, 10, target)
            console.info(f"Binary search returned index = <{found}>")

            # -------------------------- Cleanup ---------------------------
            trace_sink.Unadvise(search)
            chart_sink.Unadvise(search)
            console.success("Unadvised both sinks\n")
            search.obj.Release()
            bridge.obj.UnRegisterComponent(byref(CID_EcoBinarySearch))
            bridge.obj.Release()

        console.info("Released EcoSystem")
        return 0

    except Exception as err:
        console.error(str(err))
        return -1


if __name__ == "__main__":
    sys.exit(main())
