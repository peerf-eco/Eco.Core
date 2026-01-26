"""Core module for eco_python2acom.

This module provides fundamental types, GUID handling, and error codes
for working with ACOM (Eco Component Object Model) components.

Submodules:
    types:
        ctypes aliases for EcoOS C types.
        Platform detection (IS_WINDOWS, IS_64BIT, etc.)
        Pointer type factory (Ptr).

    guid:
        UGUID structure for component/interface identification.
        Well-known GUIDs (IID_IEcoUnknown, etc.)

    errors:
        EcoError exception class.
        EcoErrorCode enumeration.
        Helper functions (check_result, is_success).

Usage:
    >>> from eco_python2acom.core.types import Int16, Int32, VoidPtr, IS_WINDOWS
    >>> from eco_python2acom.core.guid import UGUID, IID_IEcoUnknown
    >>> from eco_python2acom.core.errors import EcoError, check_result
"""

__all__: list[str] = []
