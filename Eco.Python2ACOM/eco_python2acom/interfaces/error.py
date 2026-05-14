"""Error interface for EcoOS.

The `IEcoError1` interface exposes the system's error-tracking facility:
each component can report and inspect the last failure code, query the
human-readable description for a code, and configure error-handling mode.

Reference:
    Based on `IEcoError1.h` from `Eco.Core1/SharedFiles`.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import IID_IEcoError1
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import Int16, UInt16, Void
from eco_python2acom.types.pointer import Ptr


@interface(iid=IID_IEcoError1)
class IEcoError1(IEcoUnknown):
    """System error-tracking interface."""

    def set_ErrorMode(self, err_mode: UInt16) -> Void:
        """Set the current error-handling mode.

        Args:
            err_mode: New error-handling mode flags.
        """
        ...

    def get_ErrorMode(self) -> UInt16:
        """Return the current error-handling mode.

        Returns:
            Active error-handling mode flags.
        """
        ...

    def set_LastError(self, err_code: Int16) -> Void:
        """Record the last error code on the current thread / component.

        Args:
            errCode: Error code to store.
        """
        ...

    def get_LastError(self) -> Int16:
        """Return the most recently recorded error code.

        Returns:
            The last error code, or 0 if none was set.
        """
        ...

    def get_Description(self, err_code: Int16, message: Ptr[Void], size: Ptr[UInt16]) -> Int16:
        """Fill `message` with the human-readable description of `err_code`.

        Args:
            err_code: Error code to describe.
            message: In/out buffer that receives the description text.
            size: In/out — on input the buffer capacity, on output the bytes
                actually written.

        Returns:
            0 on success, error code otherwise.
        """
        ...
