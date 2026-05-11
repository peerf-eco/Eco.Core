"""Non-delegating `IEcoUnknown` types used to label the NDU vtable field.

`NonDelegatingUnknown` is ABI-compatible with `IEcoUnknown`; the distinct name
gives the corresponding layout field a unique label.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoUnknown)
class NonDelegatingUnknown:
    """ABI-compatible duplicate of `IEcoUnknown` — distinct name labels the NDU vtable field."""

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        ...

    def AddRef(self) -> UInt32:
        ...

    def Release(self) -> UInt32:
        ...


@view
class NonDelegatingUnknownView(NonDelegatingUnknown):
    """Non-delegating `IEcoUnknown` view."""


__all__ = ["NonDelegatingUnknown", "NonDelegatingUnknownView"]
