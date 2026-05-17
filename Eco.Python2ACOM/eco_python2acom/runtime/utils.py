"""Helper functions for EcoOS runtime.

This module provides utility functions used across the runtime package.

Functions:
    guid_to_lib_filename: Convert UGUID to hex filename.
    lib_filename_to_guid: Parse hex filename to UGUID.
    is_eco_lib: Check if filename matches EcoOS library pattern.
"""

import re
from pathlib import Path
from typing import Optional

from eco_python2acom.types.guid import UGUID

# Library extension
LIB_EXTENSION = ".dll"

# Pattern for EcoOS library filenames: 32 hex characters
ECO_LIB_PATTERN = re.compile(rf"^[0-9A-Fa-f]{{32}}{re.escape(LIB_EXTENSION)}$")


def guid_to_lib_filename(guid: UGUID) -> str:
    """Convert UGUID to hex filename format used by EcoOS.

    Args:
        guid: The UGUID to convert.

    Returns:
        Filename like "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.dll"
    """
    hex_str = bytes(guid.data).hex().upper()
    return hex_str + LIB_EXTENSION


def lib_filename_to_guid(filename: str) -> Optional[UGUID]:
    """Parse a hex filename to extract UGUID.

    Args:
        filename: Filename like "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.dll"

    Returns:
        UGUID if parsing succeeds, None otherwise.
    """
    if not is_eco_lib(filename):
        return None

    data = bytes.fromhex(Path(filename).stem)
    return UGUID(data=data)


def is_eco_lib(filename: str) -> bool:
    """Check if filename matches EcoOS library naming pattern.

    EcoOS library files are named with 32 hex characters representing
    the component's CID.

    Args:
        filename: The filename to check.

    Returns:
        True if filename matches pattern, False otherwise.
    """
    return bool(ECO_LIB_PATTERN.match(Path(filename).name))
