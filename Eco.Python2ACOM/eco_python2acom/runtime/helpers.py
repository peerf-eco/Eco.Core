"""Helper functions for EcoOS runtime.

This module provides utility functions used across the runtime package.

Functions:
    guid_to_filename: Convert UGUID to hex filename.
    filename_to_guid: Parse hex filename to UGUID.
    is_eco_dll: Check if filename matches EcoOS DLL pattern.
    get_runtime_dll_path: Build GID-based path for runtime DLLs.
"""

from __future__ import annotations

import re
from pathlib import Path
from typing import Optional

from eco_python2acom.core.guid import UGUID
from eco_python2acom.interfaces.guids.gid import GID_IEcoSystem

# Pattern for EcoOS DLL filenames: 32 hex characters
ECO_DLL_PATTERN = re.compile(r"^[0-9A-Fa-f]{32}\.dll$")

# DLL extension for Windows
DLL_EXTENSION = ".dll"


def guid_to_filename(guid: UGUID) -> str:
    """Convert UGUID to hex filename format used by EcoOS.

    EcoOS names DLL files using the hex representation of the
    component's CID or system GID.

    Args:
        guid: The UGUID to convert.

    Returns:
        Filename like "00000000000000000000000042757331.dll"
    """
    hex_str = bytes(guid.Data).hex().upper()
    return hex_str + DLL_EXTENSION


def filename_to_guid(filename: str) -> Optional[UGUID]:
    """Parse a hex filename to extract UGUID.

    Args:
        filename: Filename like "00000000000000000000000042757331.dll"

    Returns:
        UGUID if parsing succeeds, None otherwise.
    """
    name = Path(filename).stem
    try:
        data = bytes.fromhex(name)
        if len(data) != 0x10:
            return None

        return UGUID(data=data)
    except (ValueError, TypeError):
        return None


def is_eco_dll(filename: str) -> bool:
    """Check if filename matches EcoOS DLL naming pattern.

    EcoOS DLLs are named with 32 hex characters representing
    the component's CID.

    Args:
        filename: The filename to check.

    Returns:
        True if filename matches pattern, False otherwise.
    """
    return bool(ECO_DLL_PATTERN.match(Path(filename).name))


def get_runtime_dll_path(eco_framework_rt: str) -> Path:
    """Build the GID-based subdirectory path for runtime DLLs.

    EcoOS runtime DLLs are stored under a subdirectory named by the
    hex representation of the active ``GID_IEcoSystem`` Data field
    (e.g. ``00000000000000000000000086640300`` for x64).

    Args:
        eco_framework_rt: Root path from ``ECO_FRAMEWORK_RT`` env var.

    Returns:
        Path like ``{eco_framework_rt}/00000000000000000000000086640300/``.
    """
    gid_hex = bytes(GID_IEcoSystem.Data).hex()
    return Path(eco_framework_rt) / gid_hex
