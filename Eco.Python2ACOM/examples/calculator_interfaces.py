"""Interface definitions for Eco.Calculator component.

This module provides Python interface definitions corresponding to
the IEcoCalculatorX and IEcoCalculatorY interfaces defined in
the Eco.Calculator C component.

These interfaces are defined declaratively using @interface and @method
decorators. The decorators automatically generate ctypes structures
for the virtual table (VTbl) that matches the C layout.

Important:
    The order of @method definitions MUST match the order in the C header file.
    This is because VTbl is a sequential array of function pointers.
"""

from eco_python2acom.core.types import Int16, Int32
from eco_python2acom.interfaces.decorators import interface, method


@interface(iid="93221116-2248-4742-AE06-82819447843D")
class IEcoCalculatorX:
    """Calculator X interface for addition and subtraction operations.

    This interface provides basic arithmetic operations:
    - Addition: a + b (returns int32)
    - Subtraction: a - b (returns int16)

    Note:
        Methods are inherited from IEcoUnknown: QueryInterface, AddRef, Release.
        These are automatically added by the @interface decorator.
    """

    @method
    def Addition(self, a: Int16, b: Int16) -> Int32:
        """Add two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The sum of a and b (int32 to avoid overflow).
        """
        ...

    @method
    def Subtraction(self, a: Int16, b: Int16) -> Int16:
        """Subtract two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The difference (a - b) as int16.
        """
        ...


@interface(iid="BD6414C2-9096-423E-A90C-04D77AFD1CAD")
class IEcoCalculatorY:
    """Calculator Y interface for multiplication and division operations.

    This interface provides arithmetic operations:
    - Multiplication: a * b (returns int32)
    - Division: a / b (returns int16)

    Note:
        Methods are inherited from IEcoUnknown: QueryInterface, AddRef, Release.
    """

    @method
    def Multiplication(self, a: Int16, b: Int16) -> Int32:
        """Multiply two numbers.

        Args:
            a: First operand (int16).
            b: Second operand (int16).

        Returns:
            The product of a and b (int32 to avoid overflow).
        """
        ...

    @method
    def Division(self, a: Int16, b: Int16) -> Int16:
        """Divide two numbers (integer division).

        Args:
            a: Dividend (int16).
            b: Divisor (int16).

        Returns:
            The quotient (a / b) as int16.

        Warning:
            Division by zero is undefined behavior.
        """
        ...


# Component ID for Eco.Calculator
# CID_EcoCalculator = {4828F655-2E45-40E7-8121-EBD220DC360E}
CID_EcoCalculator = "4828F655-2E45-40E7-8121-EBD220DC360E"
