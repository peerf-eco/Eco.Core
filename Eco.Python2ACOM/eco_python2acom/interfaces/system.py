"""System interfaces.

The `IEcoSystem1` interface is the main entry point provided to EcoOS
user-mode applications. `IEcoCommandArguments1` and `IEcoSystemInformation1`
are companion interfaces that expose command-line arguments and basic system
identity information respectively.

Reference:
    Based on `IEcoSystem1.h` (Eco.Core1), `IEcoCommandArguments1.h` and
    `IEcoSystemInformation1.h` (Eco.System1) from `SharedFiles`.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.gid import GID_IEcoSystem
from eco_python2acom.guids.iid import IID_IEcoCommandArguments1, IID_IEcoSystemInformation1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Void
from eco_python2acom.types.pointer import Ptr


@interface(iid=GID_IEcoSystem)
class IEcoSystem1(IEcoUnknown):
    """System interface - main entry point for EcoOS applications.

    Used by client code (e.g. `EcoMain`) to access the interface bus
    and other system services. The interface is identified by `GID_IEcoSystem`.
    """

    def set_Data(self, data: Ptr[Void]) -> Void:
        """Store arbitrary user data on the system object.

        Args:
            data: Pointer to user data.
        """
        ...

    def get_Data(self) -> Ptr[Void]:
        """Retrieve previously stored user data.

        Returns:
            Pointer to user data.
        """
        ...


@interface(iid=IID_IEcoCommandArguments1)
class IEcoCommandArguments1(IEcoUnknown):
    """Access to the process command-line arguments."""

    def get_Count(self) -> Int16:
        """Return the number of command-line arguments.

        Returns:
            Argument count.
        """
        ...

    def get_Args(self) -> Ptr[CString]:
        """Return the raw `argv` array.

        Returns:
            Pointer to the array of argument strings.
        """
        ...

    def get_Path(self) -> CString:
        """Return the path of the running executable.

        Returns:
            Pointer to the executable path string.
        """
        ...

    def get_Arg(self, index: Int16) -> CString:
        """Return a single command-line argument by index.

        Args:
            index: Argument index in `[0, get_Count())`.

        Returns:
            Pointer to the argument string.
        """
        ...


@interface(iid=IID_IEcoSystemInformation1)
class IEcoSystemInformation1(IEcoUnknown):
    """Basic identity information about the running system."""

    def get_Name(self) -> CString:
        """Return the system name.

        Returns:
            Pointer to the system name string.
        """
        ...

    def get_Id(self) -> CString:
        """Return the system identifier (typically a hardware-derived UUID).

        Returns:
            Pointer to the system ID string.
        """
        ...
