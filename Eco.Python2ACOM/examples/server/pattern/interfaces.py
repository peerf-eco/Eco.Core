"""ACOM interface for the `Eco.NewProject` pattern.

Mirrors the C-side `INewProject` interface from the `NewProject` template:
a single method `MyFunction` that copies an input string into a buffer
and returns the address of the copy via an out-parameter.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

IID_INewProject = UGUID("41517A23-84CF-46B5-92A2-173501E98BBF")


@interface(iid=IID_INewProject)
class INewProject(IEcoUnknown):
    """`INewProject` — minimal demonstration interface."""

    def MyFunction(self, name: CString, copy: Ptr[CString]) -> Int16:
        """Copy the input string and return the copy.

        Args:
            name: Source string.
            copy: Out-parameter receiving a pointer to the copied string.

        Returns:
            0 on success, error code otherwise.
        """
        ...
