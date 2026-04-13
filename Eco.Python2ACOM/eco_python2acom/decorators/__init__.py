"""Decorators for declarative EcoOS definitions.

This package provides decorators for defining EcoOS interfaces, structures, and unions.

Decorators:
    @model: Define an EcoOS structure.
    @union: Define an EcoOS union.
    @interface: Define an EcoOS interface.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.model import model
from eco_python2acom.decorators.union import union
from eco_python2acom.decorators.utils import eco_class

__all__ = ["model", "union", "interface", "eco_class"]
