"""Calculator interface definitions for Eco.Calculator component.

This module defines the ACOM interfaces for the calculator component.
Interfaces inherit from IEcoUnknown, so QueryInterface, AddRef, Release
are automatically included.

Reference:
    Based on IEcoCalculatorX.h and IEcoCalculatorY.h.
"""

from eco_python2acom.core.types import Int16, Int32
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.interfaces.decorators import interface, method

# Component ID (CID) for Eco.Calculator
CID_EcoCalculator = "4828F655-2E45-40E7-8121-EBD220DC360E"


@interface(iid="93221116-2248-4742-AE06-82819447843D")
class IEcoCalculatorX(IEcoUnknown):
    """Calculator interface X - Addition and Subtraction.

    Inherits from IEcoUnknown:
        - QueryInterface(riid, ppv) -> Int16
        - AddRef() -> UInt32
        - Release() -> UInt32
    """

    @method
    def Addition(self, a: Int16, b: Int16) -> Int32:
        """Add two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The sum of a and b.
        """
        ...

    @method
    def Subtraction(self, a: Int16, b: Int16) -> Int16:
        """Subtract two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The difference of a and b.
        """
        ...


@interface(iid="BD6414C2-9096-423E-A90C-04D77AFD1CAD")
class IEcoCalculatorY(IEcoUnknown):
    """Calculator interface Y - Multiplication and Division.

    Inherits from IEcoUnknown:
        - QueryInterface(riid, ppv) -> Int16
        - AddRef() -> UInt32
        - Release() -> UInt32
    """

    @method
    def Multiplication(self, a: Int16, b: Int16) -> Int32:
        """Multiply two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The product of a and b.
        """
        ...

    @method
    def Division(self, a: Int16, b: Int16) -> Int16:
        """Divide two numbers (integer division).

        Args:
            a: Dividend (int16).
            b: Divisor (int16).

        Returns:
            The division of a and b.
        """
        ...
