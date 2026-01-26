"""Eco.Python2ACOM - Python to ACOM Bridge Library.

This library provides a bridge between Python and ACOM (Eco Component Object Model),
enabling Python applications to load and interact with EcoOS components.

Package Structure:
    eco_python2acom/
    ├── core/           # Fundamental types and utilities
    │   ├── types       # ctypes aliases (Int16, Int32, VoidPtr, etc.)
    │   ├── guid        # UGUID structure for identifiers
    │   └── errors      # EcoError exception and error codes
    ├── interfaces/     # Interface definitions and decorators
    │   ├── base        # IEcoUnknown, IEcoComponentFactory
    │   └── decorators  # @interface, @method decorators
    └── client/         # Client-side component wrappers
        └── component   # ComponentWrapper class

Usage:
    Import from submodules explicitly for better clarity:

    >>> from eco_python2acom.core.types import Int16, Int32, VoidPtr
    >>> from eco_python2acom.core.guid import UGUID
    >>> from eco_python2acom.core.errors import EcoError, check_result
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>> from eco_python2acom.client.component import ComponentWrapper

Example:
    >>> from eco_python2acom.core.types import Int16, Int32
    >>> from eco_python2acom.interfaces.decorators import interface, method
    >>>
    >>> @interface(iid="93221116-2248-4742-AE06-82819447843D")
    ... class IEcoCalculatorX:
    ...     @method
    ...     def Addition(self, a: Int16, b: Int16) -> Int32: ...

Notes:
    - This library is part of the EcoOS ecosystem.
    - Requires ECO_FRAMEWORK_RT environment variable for runtime DLLs.
    - For more information, see: https://docs.ecoos.dev/
"""

__version__ = "0.3.0"

__all__: list[str] = ["__version__"]
