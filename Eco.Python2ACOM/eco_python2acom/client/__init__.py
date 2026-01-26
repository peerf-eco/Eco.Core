"""Client module for eco_python2acom.

This module provides functionality for working with ACOM component instances
from Python code.

Submodules:
    component:
        ComponentWrapper - Pythonic wrapper for ACOM component interfaces.
        Provides direct method calls, QueryInterface, reference counting.

Usage:
    >>> from eco_python2acom.client.component import ComponentWrapper

Example:
    >>> from eco_python2acom.client.component import ComponentWrapper
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>> from eco_python2acom.core.types import Int16, Int32
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX:
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...
    >>>
    >>> # After obtaining a component pointer (ptr):
    >>> calc = ComponentWrapper(ptr, IEcoCalculatorX)
    >>> result = calc.Addition(10, 20)  # Direct method call
    >>> calc.release()  # Release when done

Note:
    For full runtime initialization (loading InterfaceBus, MemoryManager),
    use EcoSystem from the runtime module (coming in future versions).
"""

__all__: list[str] = []
