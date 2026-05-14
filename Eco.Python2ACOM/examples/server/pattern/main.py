"""`Eco.NewProject` — minimal Eco component pattern.

Mirrors the C-side `CNewProject` template: a single method `MyFunction`
that copies an input string and returns the copy through an out-parameter.
"""

from typing import Optional

from interfaces import INewProject

from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.system import IEcoSystem1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast

CID_EcoNewProject = UGUID("C5197B8A-9032-4E84-99EA-12F6FB69AABD")


@component(cid=CID_EcoNewProject)
class EcoNewProject:
    """Minimal standalone component implementing `INewProject`."""

    system: Optional[Ptr[IEcoSystem1]]
    name: Optional[CString]

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — pull `IEcoSystem1` out of the system unknown."""
        self.system = None
        self.name = None
        if not bool(system):
            return Int16(EcoErrorCode.POINTER)

        system_ptr = Ptr[Void]()
        result = system.obj.QueryInterface(byref(GID_IEcoSystem), byref(system_ptr))
        if result.value != 0 or not system_ptr.value:
            return Int16(EcoErrorCode.NOSYSTEM)
        self.system = cast(system_ptr, Ptr[IEcoSystem1])

        return Int16(EcoErrorCode.SUCCESS)

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — nothing to do for this component."""
        return Int16(EcoErrorCode.SUCCESS)

    def __eco_del__(self) -> Void:
        """Cleanup phase — release the cached `IEcoSystem1` pointer."""
        if bool(self.system):
            self.system.obj.Release()
        self.system = None
        self.name = None

    @view
    class New(INewProject):
        """`INewProject` view — single method `MyFunction`."""

        def MyFunction(self, name: CString, copy: Ptr[CString]) -> Int16:
            """Copy the input string into the component and return the copy.

            Args:
                name: Source string.
                copy: Out-parameter receiving a pointer to the copied string.

            Returns:
                0 on success, error code otherwise.
            """
            if not bool(name) or not bool(copy):
                return Int16(EcoErrorCode.POINTER)

            self.name = CString(name.value)
            copy.obj.value = self.name.value
            return Int16(EcoErrorCode.SUCCESS)


@factory(component=EcoNewProject)
class EcoNewProjectFactory(IEcoComponentFactory):
    """Factory for `Eco.NewProject`."""

    name: CString = b"Eco.NewProject"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoNewProjectFactory)
