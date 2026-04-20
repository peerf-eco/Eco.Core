"""Interface and model definitions for the `Eco.Test` component."""

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model, union
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CString, Double, Int16, Int32, UInt32
from eco_python2acom.types.function import Func
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

# IEcoTest IID = {ABCDEF12-3456-7890-ABCD-EF1234567890}
IID_IEcoTest = UGUID("ABCDEF12-3456-7890-ABCD-EF1234567890")

# EcoTest CID = {12345678-90AB-CDEF-1234-567890ABCDEF}
CID_EcoTest = UGUID("12345678-90AB-CDEF-1234-567890ABCDEF")


@model
class EcoPoint:
    """2D point structure mirroring the C-side EcoPoint."""

    x: Int32
    y: Int32

    def __eq__(self, other: object) -> bool:
        """Compare two points by coordinates."""
        if not isinstance(other, EcoPoint):
            return NotImplemented
        return self.x == other.x and self.y == other.y


@model
class EcoRect:
    """Rectangle composed of two nested `EcoPoint`s (top-left / bottom-right)."""

    top_left: EcoPoint
    bottom_right: EcoPoint


@union
class EcoValue:
    """Union reinterpreting the same storage as different scalar types."""

    as_int: Int32
    as_double: Double
    as_bytes: Array[Int32, 2]


# Tag values for `EcoVariant`
ECO_VALUE_INT = 0
ECO_VALUE_DOUBLE = 1


@model
class EcoVariant:
    """Tagged-variant: a model containing a union by value."""

    tag: Int32
    value: EcoValue


# Comparator callback: `(a, b) -> <0 | 0 | >0`, stdcall calling convention.
EcoCompareFunc = Func[Int32, [Int32, Int32]]


@interface(iid=IID_IEcoTest)
class IEcoTest(IEcoUnknown):
    """Test interface with methods covering various data types."""

    def Addition(self, left: Int32, right: Int32) -> Int32:
        """Add two integers."""
        ...

    def Negation(self, number: Int32) -> Int32:
        """Negate an integer."""
        ...

    def Multiplication(self, left: Double, right: Double) -> Double:
        """Multiply two doubles."""
        ...

    def SumArray(self, arr: Ptr[Int32], size: UInt32) -> Int32:
        """Sum elements of an array."""
        ...

    def FillArray(self, arr: Ptr[Int32], size: UInt32, value: Int32) -> Int16:
        """Fill an array with a value."""
        ...

    def SortArray(self, arr: Ptr[Int32], size: UInt32) -> Int16:
        """Sort an array in-place."""
        ...

    def GetName(self) -> CString:
        """Return the component name as a C string."""
        ...

    def MakePoint(self, x: Int32, y: Int32, out_point: Ptr[EcoPoint]) -> Int16:
        """Construct an EcoPoint structure by value."""
        ...

    def CalculateDistance(self, point: Ptr[EcoPoint], other: Ptr[EcoPoint]) -> Int32:
        """Compute squared Euclidean distance between two points."""
        ...

    def Translate(self, point: Ptr[EcoPoint], dx: Int32, dy: Int32) -> Int16:
        """Translate a point in-place by (dx, dy)."""
        ...

    def PackInt(self, value: Int32, out_value: Ptr[EcoValue]) -> Int16:
        """Write an int into an `EcoValue` out-parameter."""
        ...

    def PackDouble(self, value: Double, out_value: Ptr[EcoValue]) -> Int16:
        """Write a double into an `EcoValue` out-parameter."""
        ...

    def UnpackInt(self, value: EcoValue) -> Int32:
        """Read the int view of an `EcoValue` passed by value."""
        ...

    def UnpackDouble(self, value: EcoValue) -> Double:
        """Read the double view of an `EcoValue` passed by value."""
        ...

    def ReinterpretAsInt(self, value: EcoValue) -> Int32:
        """Return the XOR of both int halves of an `EcoValue` (by value)."""
        ...

    def MakeRect(self, top_left: EcoPoint, bottom_right: EcoPoint, out_rect: Ptr[EcoRect]) -> Int16:
        """Construct an `EcoRect` from two points passed by value."""
        ...

    def RectArea(self, rect: Ptr[EcoRect]) -> Int32:
        """Return |dx| * |dy| for the rectangle."""
        ...

    def SumPoints(self, points: Ptr[EcoPoint], size: UInt32, out_sum: Ptr[EcoPoint]) -> Int16:
        """Componentwise sum of an array of points."""
        ...

    def MakeVariant(self, tag: Int32, value: EcoValue, out_variant: Ptr[EcoVariant]) -> Int16:
        """Construct a tagged variant from a tag and a union value."""
        ...

    def GetVariantTag(self, variant: Ptr[EcoVariant]) -> Int32:
        """Return the tag of a tagged variant."""
        ...

    def GetVariantValue(self, variant: Ptr[EcoVariant], out_value: Ptr[EcoValue]) -> Int16:
        """Copy the union-value half of a tagged variant into an out-parameter."""
        ...

    def SortArrayWith(self, arr: Ptr[Int32], size: UInt32, cmp: EcoCompareFunc) -> Int16:
        """Sort an array in-place using a user-supplied comparator callback."""
        ...

    def StringLength(self, string: CString) -> Int32:
        """Return the length of a C string passed as a parameter (like `strlen`)."""
        ...
