"""Interfaces module for eco_python2acom.

This module provides base ACOM interfaces and declarative interface
definition tools using Python decorators.

Submodules:
    base:
        IEcoUnknown - Base interface for all ACOM components.
        IEcoUnknownVTbl - Virtual table for IEcoUnknown.
        IEcoComponentFactory - Factory interface for component creation.
        IEcoComponentFactoryVTbl - Virtual table for factory.
        Pointer types (IEcoUnknownPtr, IEcoComponentFactoryPtr).
        Helper functions (create_vtbl_type, create_interface_type).

    decorators:
        @interface - Decorator to define an ACOM interface.
        @method - Decorator to mark interface methods.
        InterfaceDescriptor - Metadata container for interfaces.
        MethodDescriptor - Metadata container for methods.
        Registry functions (get_interface, get_all_interfaces).
        iid_of() - Get IID from decorated interface class.

Usage:
    >>> from eco_python2acom.interfaces.base import IEcoUnknown, IEcoUnknownPtr
    >>> from eco_python2acom.interfaces.decorators import interface, method

Example:
    >>> from eco_python2acom.core.types import Int16, Int32
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX:
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
    ...
    ...     @method
    ...     def Subtraction(self, a: Int16, b: Int16) -> Int16: ...
"""

__all__: list[str] = []
