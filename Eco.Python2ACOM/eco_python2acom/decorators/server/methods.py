"""Method bundles used to populate server-side vtables.

Each bundle exposes a `build()` classmethod that returns a `{name: callable}`
mapping.
"""

from collections.abc import Callable
from typing import Any, Optional

from eco_python2acom.guids.iid import IID_IEcoUnknown
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof

# Holds Python instances whose vtable address is currently exposed to C, so
# refcount-based collection cannot pull them out from under the C caller.
_alive: dict[int, Any] = {}


# -----------------------------------------------------------------------------
# IEcoUnknown — non-delegating
# -----------------------------------------------------------------------------


class IEcoUnknownMethods:
    """Non-delegating `IEcoUnknown` triple: `QueryInterface`, `AddRef`, `Release`."""

    @staticmethod
    def build(primary: UGUID, offsets: dict[UGUID, int]) -> dict[str, Callable[..., Any]]:
        """Build the triple bound to the given primary IID and view-offset table."""

        def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
            """Query for another interface on this component.

            Args:
                iid: Pointer to the requested interface ID.
                out: Output pointer to receive the interface.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(iid) or not bool(out):
                return Int16(EcoErrorCode.POINTER)

            target = iid.obj
            offset = offsets.get(target)

            if offset is None and target == IID_IEcoUnknown:
                offset = offsets.get(primary)
            if offset is None:
                out.obj.value = 0
                return Int16(EcoErrorCode.NOINTERFACE)

            out.obj.value = addressof(self) + offset
            self.AddRef()
            return Int16(EcoErrorCode.SUCCESS)

        def AddRef(self) -> UInt32:
            """Increment the reference count.

            Returns:
                The new reference count.
            """
            self.refs += 1
            return self.refs

        def Release(self) -> UInt32:
            """Decrement the reference count.

            When the count reaches zero, the component is freed.

            Returns:
                The new reference count.
            """
            self.refs -= 1
            if self.refs == 0:
                self.__eco_del__()
                _alive.pop(addressof(self), None)
            return self.refs

        return {"QueryInterface": QueryInterface, "AddRef": AddRef, "Release": Release}


# -----------------------------------------------------------------------------
# IEcoUnknown — delegating
# -----------------------------------------------------------------------------


class DelegatingIEcoUnknownMethods:
    """Delegating `IEcoUnknown` triple that forwards every call to outer component."""

    @staticmethod
    def build() -> dict[str, Callable[..., Any]]:
        """Build the triple — every call is forwarded to `self.outer`."""

        def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
            """Query for another interface on this component.

            Args:
                iid: Pointer to the requested interface ID.
                out: Output pointer to receive the interface.

            Returns:
                0 on success, error code otherwise.
            """
            return self.outer.obj.QueryInterface(iid, out)

        def AddRef(self) -> UInt32:
            """Increment the reference count.

            Returns:
                The new reference count.
            """
            return self.outer.obj.AddRef()

        def Release(self) -> UInt32:
            """Decrement the reference count.

            When the count reaches zero, the component is freed.

            Returns:
                The new reference count.
            """
            return self.outer.obj.Release()

        return {"QueryInterface": QueryInterface, "AddRef": AddRef, "Release": Release}


# -----------------------------------------------------------------------------
# IEcoComponentFactory
# -----------------------------------------------------------------------------


class IEcoComponentFactoryMethods:
    """Method bundle implementing `IEcoComponentFactory`."""

    @staticmethod
    def build(cls: type) -> dict[str, Callable[..., Any]]:
        """Build the bundle that allocates instances of the given component class."""

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
            if not bool(iid) or not bool(out) or not bool(system):
                return Int16(EcoErrorCode.POINTER)

            if bool(outer) and iid.obj != IID_IEcoUnknown:
                return Int16(EcoErrorCode.NOAGGREGATION)

            instance = cls()
            result = instance.__eco_new__(system, outer)
            if result.value != 0:
                return result

            result = instance.__eco_init__(system)
            if result.value != 0:
                return result

            _alive[addressof(instance)] = instance

            result = instance.QueryInterface(iid, out)
            if result.value != 0:
                return result

            instance.Release()
            return Int16(EcoErrorCode.SUCCESS)

        def Init(self, system: Optional[Ptr[IEcoUnknown]], context: Ptr[Void]) -> Int16:
            """Initialize the factory with system context.

            Args:
                system: Pointer to system interface (can be NULL).
                context: Additional context (e.g., bus pointer).

            Returns:
                0 on success, error code otherwise.
            """
            return Int16(EcoErrorCode.SUCCESS)

        def get_Name(self) -> CString:
            """Get the component name.

            Returns:
                Pointer to null-terminated string.
            """
            return self.name

        def get_Version(self) -> CString:
            """Get the component version.

            Returns:
                Pointer to null-terminated string.
            """
            return self.version

        def get_Manufacturer(self) -> CString:
            """Get the component manufacturer.

            Returns:
                Pointer to null-terminated string.
            """
            return self.manufacturer

        return {
            "Alloc": Alloc,
            "Init": Init,
            "get_Name": get_Name,
            "get_Version": get_Version,
            "get_Manufacturer": get_Manufacturer,
        }


__all__ = [
    "IEcoUnknownMethods",
    "DelegatingIEcoUnknownMethods",
    "IEcoComponentFactoryMethods",
]
