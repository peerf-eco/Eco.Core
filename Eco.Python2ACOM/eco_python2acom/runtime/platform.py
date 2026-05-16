# mypy: ignore-errors
"""Platform-specific helpers used by the runtime.

Thin wrappers around operating-system facilities that supply data the runtime
reports through `IEcoSystemInformation1`.

Classes:
    `HwProfileInfo`: Layout of the hardware-profile descriptor.

Functions:
    `get_computer_name`: Read the local machine name.
    `get_hw_profile_id`: Read the hardware profile GUID.
"""

from eco_python2acom.decorators.layout import model
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CDLL, Char, UInt32
from eco_python2acom.types.utils import byref

# -----------------------------------------------------------------------------
# System libraries
# -----------------------------------------------------------------------------

KERNEL32 = "kernel32"
ADVAPI32 = "advapi32"


# -----------------------------------------------------------------------------
# HwProfileInfo
# -----------------------------------------------------------------------------


@model
class HwProfileInfo:
    """Layout of the hardware-profile descriptor.

    Attributes:
        dock_info: Docking-state flags.
        guid: Profile GUID enclosed in braces, e.g. `{XXXXXXXX-...-XXXXXXXXXXXX}`.
        name: Human-readable profile name.
    """

    dock_info: UInt32
    guid: Array[Char, 39]
    name: Array[Char, 80]


# -----------------------------------------------------------------------------
# Queries
# -----------------------------------------------------------------------------


def get_computer_name() -> bytes:
    """Read the local machine name.

    Returns:
        Machine name as raw bytes, or empty bytes on failure.
    """
    kernel32 = CDLL(KERNEL32)
    buffer = Array[Char, 256]()
    size = UInt32(256)
    kernel32.GetComputerNameA(buffer, byref(size))
    return buffer[: size.value]


def get_hw_profile_id() -> bytes:
    """Read the hardware profile GUID.

    The surrounding `{` and `}` braces returned by the OS are stripped.

    Returns:
        GUID as raw ASCII bytes, or empty bytes on failure.
    """
    advapi32 = CDLL(ADVAPI32)
    info = HwProfileInfo()
    if not advapi32.GetCurrentHwProfileA(byref(info)):
        return b""
    return info.guid[1:-2]


__all__ = ["HwProfileInfo", "get_computer_name", "get_hw_profile_id"]
