"""System interface definition.

The `IEcoSystem1` interface is the main entry point provided to
EcoOS user-mode applications. It is identified by a GID (Generation ID)
rather than a traditional IID.

Reference:
    Based on `IEcoSystem1.h` from `Eco.Core1/SharedFiles`.
"""

from __future__ import annotations

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.core import Void
from eco_python2acom.types.pointer import Ptr


@interface(iid=GID_IEcoSystem)
class IEcoSystem1(IEcoUnknown):
    """System interface - main entry point for EcoOS applications.

    Used by client code (e.g. `EcoMain`) to access the interface bus
    and other system services. The interface is identified by `GID_IEcoSystem`.
    """

    def set_data(self, ptr: Ptr[Void]) -> Void:
        """Store arbitrary user data on the system object.

        Args:
            ptr: Pointer to user data.
        """
        ...

    def get_data(self) -> Ptr[Void]:
        """Retrieve previously stored user data.

        Returns:
            Pointer to user data.
        """
        ...
