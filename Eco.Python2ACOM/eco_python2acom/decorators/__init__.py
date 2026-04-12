"""Decorators for declarative EcoOS definitions.

This package provides decorators for defining EcoOS interfaces and structures.

Decorators:
    @model: Define an EcoOS structure.
    @interface: Define an EcoOS interface.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.model import model
from eco_python2acom.decorators.utils import eco_class

__all__ = ["model", "interface", "eco_class"]
