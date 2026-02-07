"""IEcoSystem1 interface definition.

The IEcoSystem1 interface is the main entry point provided to
EcoOS user-mode applications. It is identified by a GID (Generation ID)
rather than a traditional IID.

Reference:
    Based on IEcoSystem1.h from Eco.Core1/SharedFiles.
"""

from __future__ import annotations

from ctypes import _Pointer
from typing import TYPE_CHECKING

from eco_python2acom.core.types import Void, VoidPtr
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.interfaces.guids.gid import GID_IEcoSystem


@interface(iid=GID_IEcoSystem)
class IEcoSystem1(IEcoUnknown):
    """System interface - main entry point for EcoOS applications.

    Used by client code (e.g. EcoMain) to access the interface bus
    and other system services.

    The interface is identified by ``GID_IEcoSystem`` (a Generation ID),
    not a regular IID.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def set_Data(self, pv: VoidPtr) -> Void:
        """Store arbitrary user data on the system object.

        Args:
            pv: Pointer to user data.
        """
        ...

    @method
    def get_Data(self) -> VoidPtr:
        """Retrieve previously stored user data.

        Returns:
            Pointer to user data.
        """
        ...


if TYPE_CHECKING:
    IEcoSystem1Ptr = _Pointer[IEcoSystem1]
else:
    IEcoSystem1Ptr = IEcoSystem1._interface_ptr_
