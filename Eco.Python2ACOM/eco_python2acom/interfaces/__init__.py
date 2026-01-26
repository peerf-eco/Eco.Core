"""Interfaces module for eco_python2acom.

This module provides base ACOM interfaces and VTbl (Virtual Table) mechanisms
for working with EcoOS components.

The main components are:
- IEcoUnknown: Base interface for all ACOM components
- IEcoComponentFactory: Factory interface for component creation
- VTbl: Virtual table management utilities
- Decorators: @interface, @method for declarative interface definitions
"""

from eco_python2acom.interfaces.base import (
    IEcoComponentFactory,
    IEcoComponentFactoryVTbl,
    IEcoUnknown,
    IEcoUnknownVTbl,
)
from eco_python2acom.interfaces.decorators import (
    InterfaceDescriptor,
    MethodDescriptor,
    get_all_interfaces,
    get_interface,
    iid_of,
    interface,
    method,
)

__all__ = [
    # Base interfaces
    "IEcoUnknown",
    "IEcoUnknownVTbl",
    "IEcoComponentFactory",
    "IEcoComponentFactoryVTbl",
    # Decorators
    "interface",
    "method",
    "iid_of",
    # Descriptors
    "InterfaceDescriptor",
    "MethodDescriptor",
    # Registry
    "get_interface",
    "get_all_interfaces",
]
