"""Interface for ACOM2Python bridge.

The bridge is a native C component that embeds `CPython` and exposes
Python-implemented ACOM components to the surrounding system.

Interfaces:
    `IEcoACOM2Python`: Registry of Python component factories living inside the bridge.

Reference:
    Based on `IEcoACOM2Python.h` from `Eco.ACOM2Python/SharedFiles`.
"""

from typing import Optional

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoACOM2Python
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoACOM2Python)
class IEcoACOM2Python(IEcoUnknown):
    """Registry of Python ACOM components managed by the bridge.

    The bridge imports a Python source file, calls its `get_component_factory`
    attribute, and stores the resulting factory under the given CID.
    """

    def RegisterComponent(self, pathName: CString, rcid: Ptr[UGUID]) -> Int16:
        """Register a Python component factory loaded from `pathName`.

        Args:
            pathName: Absolute filesystem path to the Python source file.
            rcid: Component ID under which the factory is registered.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def UnRegisterComponent(self, rcid: Ptr[UGUID]) -> Int16:
        """Drop the factory previously registered under `rcid`.

        Args:
            rcid: Component ID of the factory to remove.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def QueryComponent(
        self,
        rcid: Ptr[UGUID],
        outer: Optional[Ptr[IEcoUnknown]],
        riid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Create a Python component instance and hand back the requested interface.

        Args:
            rcid: Component ID of the Python component to instantiate.
            outer: Outer unknown for aggregation (can be NULL).
            riid: Requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """

        ...
