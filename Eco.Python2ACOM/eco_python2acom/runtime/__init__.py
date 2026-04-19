"""EcoOS runtime initialization and component management.

This package provides the core runtime functionality for the
Python2ACOM bridge, including:

Modules:
    system: EcoSystem - main entry point for ACOM interaction.
"""

from eco_python2acom.runtime.system import EcoSystem

__all__ = ["EcoSystem"]
