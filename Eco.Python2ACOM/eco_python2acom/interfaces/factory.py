"""Component factory ACOM interface.

Defines `IEcoComponentFactory`, the interface each component library
exposes via `GetIEcoComponentFactoryPtr()` to allocate and initialize
component instances.
"""

from typing import Optional

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoComponentFactory)
class IEcoComponentFactory(IEcoUnknown):
    """Factory interface for creating ACOM component instances.

    Each component exports `GetIEcoComponentFactoryPtr()` that returns
    a pointer to this interface.
    """

    def Alloc(
        self,
        system: Optional[Ptr[IEcoUnknown]],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Allocate a new component instance.

        Args:
            system: Pointer to system interface (can be NULL).
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Init(self, system: Optional[Ptr[IEcoUnknown]], context: Ptr[Void]) -> Int16:
        """Initialize the factory with system context.

        Args:
            system: Pointer to system interface (can be NULL).
            context: Additional context (e.g., bus pointer).

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_Name(self) -> CString:
        """Get the component name.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    def get_Version(self) -> CString:
        """Get the component version.

        Returns:
            Pointer to null-terminated string.
        """
        ...

    def get_Manufacturer(self) -> CString:
        """Get the component manufacturer.

        Returns:
            Pointer to null-terminated string.
        """
        ...
