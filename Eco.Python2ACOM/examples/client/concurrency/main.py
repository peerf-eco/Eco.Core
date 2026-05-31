"""Concurrency example — a shared-state Python component under threaded load.

The host:
    1. Boots `EcoSystem` and reaches the Python bridge through the bus.
    2. Registers `Eco.Counter` (server-side example) with the bridge.
    3. Queries `IEcoCounter`.
    4. Hammers the same instance from many threads with the racy and the
       lock-guarded increments, comparing the result.
    5. Fires several blocking increments via `asyncio`.
    6. Releases the interface pointer.
"""

import asyncio
import sys
import threading
from pathlib import Path
from time import perf_counter

from eco_python2acom.guids.iid import IID_IEcoACOM2Python
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.types.core import Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from examples.client.console import console
from examples.server.concurrency.counter import CID_EcoCounter
from examples.server.concurrency.interfaces import IEcoCounter, IID_IEcoCounter

SERVER_PATH = str(
    Path(__file__).resolve().parent.parent.parent / "server" / "concurrency" / "counter.py"
)

# Threaded run: how many threads and how many increments each performs
THREAD_COUNT = 8
INCREMENTS = 1000
EXPECTED = THREAD_COUNT * INCREMENTS

# Asyncio run: how many blocking increments, and how long each one blocks
TASK_COUNT = 25
BLOCK_MILLISECONDS = 1000

# Column width — keep every result line aligned
WIDTH = 20


def show(label: str, score: str, ok: bool, verdict: str) -> None:
    """Print one aligned result line: label, score, status mark and verdict.

    Args:
        label: Method name shown in the first column.
        score: The run's headline figure.
        ok: Whether the run met its expectation.
        verdict: Short phrase explaining the outcome.
    """
    name = f"[{label}]".ljust(WIDTH)
    figure = score.ljust(WIDTH)
    mark = "[green]✓[/green]" if ok else "[red]✗[/red]"
    console.print(f"  [yellow]{name}[/yellow] [cyan]{figure}[/cyan] {mark} ({verdict})\n")


def hammer(counter: Ptr[IEcoCounter], method: str) -> int:
    """Run the increment `method` from `THREAD_COUNT` threads, `INCREMENTS` each.

    Args:
        counter: `IEcoCounter` component pointer.
        method: Name of the increment method to call.

    Returns:
        The counter's value after all threads finish.
    """
    counter.obj.Reset()
    call = getattr(counter.obj, method)

    def task() -> None:
        for _ in range(INCREMENTS):
            call()

    threads = [threading.Thread(target=task) for _ in range(THREAD_COUNT)]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    return counter.obj.GetValue()


async def hammer_async(counter: Ptr[IEcoCounter]) -> tuple[float, int]:
    """Dispatch `TASK_COUNT` blocking increments on the executor and await them.

    Args:
        counter: `IEcoCounter` component pointer.

    Returns:
        Elapsed seconds and total count for the whole batch.
    """
    counter.obj.Reset()
    loop = asyncio.get_running_loop()
    start = perf_counter()
    await asyncio.gather(
        *(
            loop.run_in_executor(None, counter.obj.IncrementBlock, BLOCK_MILLISECONDS)
            for _ in range(TASK_COUNT)
        )
    )
    return perf_counter() - start, counter.obj.GetValue()


def main() -> int:
    """Run the racy vs lock-guarded counter under concurrent threaded load."""
    console.header("Eco Counter Example")
    console.info(f"CID:         {CID_EcoCounter}")
    console.info(f"IEcoCounter: {IID_IEcoCounter}")
    console.info(f"{THREAD_COUNT} threads x {INCREMENTS} increments = {EXPECTED} expected\n")

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

            result = bridge.obj.RegisterComponent(SERVER_PATH.encode(), byref(CID_EcoCounter))
            if result != 0:
                console.error(f"Failed to register component (code = {result})")
                bridge.obj.Release()
                return -3

            # ------------------------- Component --------------------------
            counter_ptr = Ptr[Void]()
            result = bridge.obj.QueryComponent(
                byref(CID_EcoCounter), None, byref(IID_IEcoCounter), byref(counter_ptr)
            )
            if result != 0 or not bool(counter_ptr):
                console.error(f"Failed to query component (code = {result})")
                bridge.obj.UnRegisterComponent(byref(CID_EcoCounter))
                bridge.obj.Release()
                return -4
            counter = cast(counter_ptr, Ptr[IEcoCounter])
            console.success(f"Got {counter}\n")

            # ---------------------------- Runs ----------------------------
            console.info("Running unguarded 'Increment'...")
            total = hammer(counter, "Increment")
            show("Increment", f"{total} / {EXPECTED}", total == EXPECTED, "race")

            console.info("Running lock-guarded 'IncrementSafe'...")
            total = hammer(counter, "IncrementSafe")
            show("IncrementSafe", f"{total} / {EXPECTED}", total == EXPECTED, "exact")

            console.info("Running asyncio 'IncrementBlock'...")
            elapsed, total = asyncio.run(hammer_async(counter))
            serial = TASK_COUNT * BLOCK_MILLISECONDS / 1000.0
            show(
                "IncrementBlock",
                f"{elapsed:.1f}s / {serial:.0f}s",
                total == TASK_COUNT,
                f"~{serial / elapsed:.0f}x faster",
            )

            # -------------------------- Cleanup ---------------------------
            counter.obj.Release()
            console.success(f"Released {counter}")
            bridge.obj.UnRegisterComponent(byref(CID_EcoCounter))
            bridge.obj.Release()

        console.info("Released EcoSystem")
        return 0

    except Exception as err:
        console.error(str(err))
        return -1


if __name__ == "__main__":
    sys.exit(main())
