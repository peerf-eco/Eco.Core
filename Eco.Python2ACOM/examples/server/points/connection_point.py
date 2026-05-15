"""`EcoBinarySearchConnectionPoint` — connection-point helper.

Stores attached sinks inside an `IEcoList1` instance and creates fresh
`EcoBinarySearchEnumConnections` enumerators on demand.
"""

try:
    from enum_connections import EcoBinarySearchEnumConnections
    from utils import AcquireList
except ImportError:
    from examples.server.points.enum_connections import EcoBinarySearchEnumConnections
    from examples.server.points.utils import AcquireList

from eco_python2acom.decorators.server.alive import ALIVE
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.connections import (
    EcoConnectionData,
    IEcoConnectionPoint,
    IEcoConnectionPointContainer,
    IEcoEnumConnections,
)
from eco_python2acom.interfaces.list import IEcoList1
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr, pointer
from eco_python2acom.types.utils import addressof, byref, cast


@component()
class EcoBinarySearchConnectionPoint:
    """Connection point keyed by a single outgoing interface IID."""

    system: Ptr[IEcoSystem1]
    container: Ptr[IEcoConnectionPointContainer]
    piid: Ptr[UGUID]
    next_cookie: UInt32
    sink_list: Ptr[IEcoList1]

    def __eco_new__(
        self,
        system: Ptr[IEcoUnknown],
        container: Ptr[IEcoConnectionPointContainer],
        riid: Ptr[UGUID],
    ) -> Int16:
        """Allocation phase — resolve `IEcoSystem1` and seed the sink list.

        Args:
            system: System unknown pointer handed in by the caller.
            container: Owning connection-point container interface.
            riid: IID of the outgoing interface served by this point.

        Returns:
            0 on success, error code otherwise.
        """
        self.system = Ptr[IEcoSystem1]()
        self.sink_list = Ptr[IEcoList1]()
        self.container = Ptr[IEcoConnectionPointContainer]()
        self.piid = Ptr[UGUID]()
        self.next_cookie = 0
        if not bool(system) or not bool(container):
            return EcoErrorCode.POINTER

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result != 0 or not bool(system_ptr):
            return EcoErrorCode.NOSYSTEM
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        self.container = container
        self.piid = riid
        result = AcquireList(self.system, pointer(self.sink_list))
        if result != 0 or not bool(self.sink_list):
            return result
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — release every attached sink and drop cached references."""
        if bool(self.sink_list):
            for idx in range(self.sink_list.obj.Count()):
                slot_ptr = self.sink_list.obj.Item(idx)
                if bool(slot_ptr):
                    slot = cast(slot_ptr, Ptr[EcoConnectionData])
                    slot.obj.ptr.obj.Release()
                    ALIVE.pop(slot_ptr.value, None)
            self.sink_list.obj.Clear()
            self.sink_list.obj.Release()

        self.next_cookie = 0
        if bool(self.system):
            self.system.obj.Release()

    @view
    class Point(IEcoConnectionPoint):
        """`IEcoConnectionPoint` view — `Advise`/`Unadvise`/`EnumConnections`/..."""

        def GetConnectionInterface(self, iid: Ptr[UGUID]) -> Int16:
            """Return the IID of the outgoing interface served by this point.

            Args:
                iid: Output slot receiving the IID.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(iid):
                return EcoErrorCode.POINTER

            iid.obj = self.piid.obj
            return EcoErrorCode.SUCCESS

        def GetConnectionPointContainer(
            self, container: Ptr[Ptr[IEcoConnectionPointContainer]]
        ) -> Int16:
            """Return the owning connection-point container.

            Args:
                container: Output pointer receiving the container interface.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(container):
                return EcoErrorCode.POINTER

            container.obj = self.container
            self.container.obj.AddRef()
            return EcoErrorCode.SUCCESS

        def Advise(self, sink: Ptr[IEcoUnknown], cookie: Ptr[UInt32]) -> Int16:
            """Attach `sink` to this connection point and return its cookie.

            Args:
                sink: Subscriber's `IEcoUnknown` pointer.
                cookie: Output slot receiving the cookie that identifies the
                    new connection.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(sink) or not bool(cookie):
                return EcoErrorCode.POINTER

            events_ptr = Ptr[Void]()
            result = sink.obj.QueryInterface(self.piid, byref(events_ptr))
            if result != 0 or not bool(events_ptr):
                return EcoErrorCode.NOINTERFACE

            self.next_cookie += 1
            data = EcoConnectionData()
            data.ptr = cast(events_ptr, Ptr[IEcoUnknown])
            data.cookie = self.next_cookie
            ALIVE[addressof(data)] = data
            self.sink_list.obj.Add(pointer(data, Void))

            cookie.obj = UInt32(self.next_cookie)
            return EcoErrorCode.SUCCESS

        def Unadvise(self, cookie: UInt32) -> Int16:
            """Detach the sink previously attached under `cookie`.

            Args:
                cookie: Cookie returned by an earlier `Advise` call.

            Returns:
                0 on success, error code otherwise.
            """
            for idx in range(self.sink_list.obj.Count()):
                slot_ptr = self.sink_list.obj.Item(idx)
                if not bool(slot_ptr):
                    continue
                slot = cast(slot_ptr, Ptr[EcoConnectionData])
                if slot.obj.cookie == cookie:
                    self.sink_list.obj.RemoveAt(idx)
                    slot.obj.ptr.obj.Release()
                    ALIVE.pop(slot_ptr.value, None)
                    return EcoErrorCode.SUCCESS

            return EcoErrorCode.INVALIDARG

        def EnumConnections(self, enum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
            """Hand back an enumerator with a snapshot of the current sink list.

            Args:
                enum: Output pointer receiving the new enumerator.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(enum):
                return EcoErrorCode.POINTER

            new_enum = EcoBinarySearchEnumConnections()
            result = new_enum.__eco_new__(cast(self.system, Ptr[IEcoUnknown]), self.sink_list)
            if result != 0:
                return result
            ALIVE[addressof(new_enum)] = new_enum
            enum.obj = pointer(new_enum, IEcoEnumConnections, shift=True)
            return EcoErrorCode.SUCCESS
