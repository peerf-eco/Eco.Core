"""Calculator interface definitions for Eco.Calculator component.

This module defines the ACOM interfaces used by the calculator examples.
All interfaces inherit from IEcoUnknown (QueryInterface, AddRef, Release).

Interfaces:
    IEcoCalculatorX: Addition and Subtraction.
    IEcoCalculatorY: Multiplication and Division.

Constants:
    CID_EcoCalculator: Component ID for the calculator component.
"""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.core import Int16, Int32


@interface(iid="93221116-2248-4742-AE06-82819447843D")
class IEcoCalculatorX(IEcoUnknown):
    """Calculator interface X — Addition and Subtraction.

    Inherits from IEcoUnknown:
        QueryInterface, AddRef, Release.
    """

    def Addition(self, a: Int16, b: Int16) -> Int32:
        """Add two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The sum of a and b.
        """
        ...

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
    """Calculator interface Y — Multiplication and Division.

    Inherits from IEcoUnknown:
        QueryInterface, AddRef, Release.
    """

    def Multiplication(self, a: Int16, b: Int16) -> Int32:
        """Multiply two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The product of a and b.
        """
        ...

    def Division(self, a: Int16, b: Int16) -> Int16:
        """Divide two numbers (integer division).

        Args:
            a: Dividend (int16).
            b: Divisor (int16).

        Returns:
            The quotient of a and b.
        """
        ...
