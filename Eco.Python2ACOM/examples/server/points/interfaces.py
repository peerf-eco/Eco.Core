"""ACOM interfaces for the `Eco.BinarySearch` connection-point example.

Interfaces:
    - `IEcoBinarySearch` — exposes the `BinarySearch` operation that scans
      a sorted array of `Int32` values and returns the index of the target.
    - `IEcoBinarySearchEvents` — outgoing notifications fired during the
      search: `OnStart` once before the loop, `OnStep` for every probe, and
      `OnFinal` once at the end with the resolved index.
    - `IEcoBinarySearchStats` — outgoing notifications fired fired once at the
      end with summary telemetry: comparison count and elapsed time.

The component exposes `IEcoBinarySearchEvents` and `IEcoBinarySearchStats` as
two separate connection points, so a single sink may subscribe to both.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, Int32
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

IID_IEcoBinarySearch = UGUID("0F4D3A1B-7C82-49B5-9E04-2A1D6E3F8C71")
IID_IEcoBinarySearchEvents = UGUID("B2A85F36-1ED4-4F71-8C29-9B7E0D4A6C53")
IID_IEcoBinarySearchStats = UGUID("7C3E9A14-5B62-4D8F-A1E7-3F9C2B8D6E45")


@interface(iid=IID_IEcoBinarySearch)
class IEcoBinarySearch(IEcoUnknown):
    """Binary search over a sorted array of `Int32` values."""

    def BinarySearch(self, array: Ptr[Int32], length: Int32, target: Int32) -> Int32:
        """Search `target` inside `array`.

        Args:
            array: Pointer to the first element of the sorted array.
            length: Number of elements in the array.
            target: Value to look for.

        Returns:
            Zero-based index of the match, or `-1` when `target` is not found.
        """
        ...


@interface(iid=IID_IEcoBinarySearchEvents)
class IEcoBinarySearchEvents(IEcoUnknown):
    """Outgoing notifications fired during a `BinarySearch` call."""

    def OnStart(self, array: Ptr[Int32], length: Int32) -> Int16:
        """Fired once before the first probe with a copy of the input array.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def OnStep(
        self, array: Ptr[Int32], length: Int32, left: Int32, right: Int32, mid: Int32, value: Int32
    ) -> Int16:
        """Fired for every probe inside the binary-search loop.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.
            left: Current left bound (inclusive).
            right: Current right bound (inclusive).
            mid: Index being probed on this step.
            value: Array value at `mid`.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def OnFinal(self, array: Ptr[Int32], length: Int32, index: Int32) -> Int16:
        """Fired once after the loop with the resolved index.

        Args:
            array: Pointer to the first element of the array.
            length: Number of elements in the array.
            index: Resolved index, or `-1` when nothing was found.

        Returns:
            0 on success, error code otherwise.
        """
        ...


@interface(iid=IID_IEcoBinarySearchStats)
class IEcoBinarySearchStats(IEcoUnknown):
    """Outgoing summary telemetry fired once when a `BinarySearch` completes."""

    def OnComparisons(self, count: Int32) -> Int16:
        """Fired with the number of comparisons (probes) the search performed.

        Args:
            count: How many array elements were probed during the search.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def OnTiming(self, elapsed: Int32) -> Int16:
        """Fired with the pure algorithm time in microseconds.

        Args:
            elapsed: Elapsed search time in microseconds.

        Returns:
            0 on success, error code otherwise.
        """
        ...
