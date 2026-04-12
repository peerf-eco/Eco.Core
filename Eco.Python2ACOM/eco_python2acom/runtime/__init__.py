"""EcoOS runtime initialization and component management.

This package provides the core runtime functionality for the
Python2ACOM bridge, including:

Modules:
    loader: Load EcoOS component libraries and extract factories.
    system: EcoSystem - main entry point for ACOM interaction.
    utils: Helper functions for library filename handling and GUID parsing.
"""

from eco_python2acom.runtime.loader import EcoLibLoader
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.runtime.utils import guid_to_lib_filename, is_eco_lib, lib_filename_to_guid

__all__ = [
    "EcoSystem",
    "EcoLibLoader",
    "guid_to_lib_filename",
    "is_eco_lib",
    "lib_filename_to_guid",
]
