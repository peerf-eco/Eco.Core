"""EcoOS runtime initialization and component management.

This package provides the core runtime functionality for the
Python2ACOM bridge, including:

Modules:
    system: EcoSystem - main entry point for ACOM interaction.
    loader: DllLoader - loading and factory extraction.
    helpers: Utility functions (GUID conversion, etc.).
"""

from eco_python2acom.runtime.helpers import (
    filename_to_guid,
    get_runtime_dll_path,
    guid_to_filename,
    is_eco_dll,
)
from eco_python2acom.runtime.loader import DllLoader, LoadedDll
from eco_python2acom.runtime.system import EcoSystem

__all__ = [
    # Main class
    "EcoSystem",
    # DLL loading
    "DllLoader",
    "LoadedDll",
    # Helpers
    "guid_to_filename",
    "filename_to_guid",
    "is_eco_dll",
    "get_runtime_dll_path",
]
