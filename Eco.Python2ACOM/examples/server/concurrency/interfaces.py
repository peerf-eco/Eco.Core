"""ACOM interface for the `Eco.Counter` concurrency example.

This module defines a shared-counter interface with three increment variants:
    - `Increment` updates the shared value without any synchronisation.
    - `IncrementSafe` performs the same update while holding a lock.
    - `IncrementBlock` blocks for a while and then increments under the lock.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, Int32, UInt32
from eco_python2acom.types.guid import UGUID

IID_IEcoCounter = UGUID("9A3C7E21-4B68-4F0D-8E15-2C7A9D4B6E83")


@interface(iid=IID_IEcoCounter)
class IEcoCounter(IEcoUnknown):
    """`IEcoCounter` — a shared integer with racy and lock-guarded increments."""

    def Increment(self) -> Int16:
        """Increment the shared value without synchronisation."""
        ...

    def IncrementSafe(self) -> Int16:
        """Increment the shared value under a lock."""
        ...

    def IncrementBlock(self, milliseconds: UInt32) -> Int16:
        """Block and increment the shared value under a lock."""
        ...

    def GetValue(self) -> Int32:
        """Return the current value of the shared counter."""
        ...

    def Reset(self) -> Int16:
        """Reset the shared value back to zero."""
        ...
