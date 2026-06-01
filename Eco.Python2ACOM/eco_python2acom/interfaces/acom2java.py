"""Interface for the ACOM2Java bridge.

The bridge is a native C component that embeds a JVM and exposes
Java-implemented ACOM components to the surrounding system.

Interfaces:
    `IEcoACOM2Java`: Registry of Java components living inside the bridge.

Reference:
    Based on `IEcoACOM2Java.h` from `Eco.ACOM2Java/SharedFiles`.
"""

from typing import Optional

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoACOM2Java
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoACOM2Java)
class IEcoACOM2Java(IEcoUnknown):
    """Registry of Java ACOM components managed by the bridge.

    The bridge loads a Java class by its classpath and class name, instantiates
    the corresponding instance and stores it under the given CID.
    """

    def RegisterComponent(self, classpath: CString, classname: CString, rcid: Ptr[UGUID]) -> Int16:
        """Register a Java component loaded from `classpath/classname`.

        Args:
            classpath: Java classpath to search for the class.
            classname: Fully qualified Java class name.
            rcid: Component ID under which the class is registered.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def UnRegisterComponent(self, rcid: Ptr[UGUID]) -> Int16:
        """Drop the class previously registered under `rcid`.

        Args:
            rcid: Component ID of the class to remove.

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
        """Create a Java component instance and hand back the requested interface.

        Args:
            rcid: Component ID of the Java component to instantiate.
            outer: Outer unknown for aggregation (can be NULL).
            riid: Requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        ...
