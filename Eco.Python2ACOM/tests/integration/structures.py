"""Shared definitions and Python implementation for the `Eco.Test` integration tests."""

import functools

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.layout import model, union
from eco_python2acom.decorators.server.component import component
from eco_python2acom.decorators.server.factory import export, factory
from eco_python2acom.decorators.server.view import view
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.array import Array
from eco_python2acom.types.core import CString, Double, Int16, Int32, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.function import Func
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

# IEcoTest IID = {ABCDEF12-3456-7890-ABCD-EF1234567890}
IID_IEcoTest = UGUID("ABCDEF12-3456-7890-ABCD-EF1234567890")

# EcoTest CID = {12345678-90AB-CDEF-1234-567890ABCDEF}
CID_EcoTest = UGUID("12345678-90AB-CDEF-1234-567890ABCDEF")

# EcoACOM2Python CID = {219EDB62-6EF1-4B42-BE16-F93A566F1CC3}
CID_EcoACOM2Python = UGUID("219EDB62-6EF1-4B42-BE16-F93A566F1CC3")


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

    def UnpackInt(self, value: Ptr[EcoValue]) -> Int32:
        """Read the int view of an `EcoValue`."""
        ...

    def UnpackDouble(self, value: Ptr[EcoValue]) -> Double:
        """Read the double view of an `EcoValue`."""
        ...

    def ReinterpretAsInt(self, value: Ptr[EcoValue]) -> Int32:
        """Return the XOR of both int halves of an `EcoValue`."""
        ...

    def MakeRect(
        self, top_left: Ptr[EcoPoint], bottom_right: Ptr[EcoPoint], out_rect: Ptr[EcoRect]
    ) -> Int16:
        """Construct an `EcoRect` from two points."""
        ...

    def RectArea(self, rect: Ptr[EcoRect]) -> Int32:
        """Return |dx| * |dy| for the rectangle."""
        ...

    def SumPoints(self, points: Ptr[EcoPoint], size: UInt32, out_sum: Ptr[EcoPoint]) -> Int16:
        """Componentwise sum of an array of points."""
        ...

    def MakeVariant(self, tag: Int32, value: Ptr[EcoValue], out_variant: Ptr[EcoVariant]) -> Int16:
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


@component(cid=CID_EcoTest)
class EcoTest:
    """Pure-Python implementation of `IEcoTest`, exposed through `Eco.ACOM2Python`."""

    def __eco_new__(self, system: Ptr[IEcoUnknown], outer: Ptr[IEcoUnknown]) -> Int16:
        """Allocation phase — no resources to wire up for this stateless component."""
        return EcoErrorCode.SUCCESS

    def __eco_init__(self, system: Ptr[IEcoUnknown]) -> Int16:
        """Initialisation phase — no further setup is required."""
        return EcoErrorCode.SUCCESS

    def __eco_del__(self) -> Void:
        """Cleanup phase — nothing to release."""
        return None

    @view
    class Test(IEcoTest):
        """`IEcoTest` view — pure-Python mirror of the C-side surface."""

        def Addition(self, left: Int32, right: Int32) -> Int32:
            """Add two integers."""
            return left + right

        def Negation(self, number: Int32) -> Int32:
            """Negate an integer."""
            return -number

        def Multiplication(self, left: Double, right: Double) -> Double:
            """Multiply two doubles."""
            return left * right

        def SumArray(self, arr: Ptr[Int32], size: UInt32) -> Int32:
            """Sum elements of an array."""
            return sum(arr[idx] for idx in range(size))

        def FillArray(self, arr: Ptr[Int32], size: UInt32, value: Int32) -> Int16:
            """Fill an array with a value."""
            for idx in range(size):
                arr[idx] = Int32(value)
            return EcoErrorCode.SUCCESS

        def SortArray(self, arr: Ptr[Int32], size: UInt32) -> Int16:
            """Sort an array in-place in ascending order."""
            values = sorted(arr[idx] for idx in range(size))
            for idx, value in enumerate(values):
                arr[idx] = Int32(value)
            return EcoErrorCode.SUCCESS

        def GetName(self) -> CString:
            """Return the component name as a C string."""
            return b"EcoTest"

        def MakePoint(self, x: Int32, y: Int32, out_point: Ptr[EcoPoint]) -> Int16:
            """Construct an `EcoPoint(x, y)` into `out_point`."""
            point = EcoPoint()
            point.x = Int32(x)
            point.y = Int32(y)
            out_point.obj = point
            return EcoErrorCode.SUCCESS

        def CalculateDistance(self, point: Ptr[EcoPoint], other: Ptr[EcoPoint]) -> Int32:
            """Return the squared Euclidean distance between two points."""
            dx = other.obj.x - point.obj.x
            dy = other.obj.y - point.obj.y
            return dx * dx + dy * dy

        def Translate(self, point: Ptr[EcoPoint], dx: Int32, dy: Int32) -> Int16:
            """Translate a point in-place by `(dx, dy)`."""
            translated = EcoPoint()
            translated.x = Int32(point.obj.x + dx)
            translated.y = Int32(point.obj.y + dy)
            point.obj = translated
            return EcoErrorCode.SUCCESS

        def PackInt(self, value: Int32, out_value: Ptr[EcoValue]) -> Int16:
            """Pack `value` into the int half of `out_value`."""
            packed = EcoValue()
            packed.as_int = Int32(value)
            out_value.obj = packed
            return EcoErrorCode.SUCCESS

        def PackDouble(self, value: Double, out_value: Ptr[EcoValue]) -> Int16:
            """Pack `value` into the double half of `out_value`."""
            packed = EcoValue()
            packed.as_double = Double(value)
            out_value.obj = packed
            return EcoErrorCode.SUCCESS

        def UnpackInt(self, value: Ptr[EcoValue]) -> Int32:
            """Return the int view of an `EcoValue`."""
            return value.obj.as_int

        def UnpackDouble(self, value: Ptr[EcoValue]) -> Double:
            """Return the double view of an `EcoValue`."""
            return value.obj.as_double

        def ReinterpretAsInt(self, value: Ptr[EcoValue]) -> Int32:
            """Return the XOR of both int halves of an `EcoValue`."""
            return value.obj.as_bytes[0] ^ value.obj.as_bytes[1]

        def MakeRect(
            self, top_left: Ptr[EcoPoint], bottom_right: Ptr[EcoPoint], out_rect: Ptr[EcoRect]
        ) -> Int16:
            """Construct an `EcoRect` from two points."""
            rect = EcoRect()
            rect.top_left = top_left.obj
            rect.bottom_right = bottom_right.obj
            out_rect.obj = rect
            return EcoErrorCode.SUCCESS

        def RectArea(self, rect: Ptr[EcoRect]) -> Int32:
            """Return `|dx| * |dy|` for the rectangle."""
            dx = abs(rect.obj.bottom_right.x - rect.obj.top_left.x)
            dy = abs(rect.obj.bottom_right.y - rect.obj.top_left.y)
            return dx * dy

        def SumPoints(self, points: Ptr[EcoPoint], size: UInt32, out_sum: Ptr[EcoPoint]) -> Int16:
            """Componentwise sum of an array of points."""
            total = EcoPoint()
            total.x = 0
            total.y = 0
            for idx in range(size):
                point = points[idx]
                total.x += point.x
                total.y += point.y
            out_sum.obj = total
            return EcoErrorCode.SUCCESS

        def MakeVariant(
            self, tag: Int32, value: Ptr[EcoValue], out_variant: Ptr[EcoVariant]
        ) -> Int16:
            """Construct a tagged variant from a tag and a union value (by pointer)."""
            variant = EcoVariant()
            variant.tag = Int32(tag)
            variant.value = value.obj
            out_variant.obj = variant
            return EcoErrorCode.SUCCESS

        def GetVariantTag(self, variant: Ptr[EcoVariant]) -> Int32:
            """Return the tag of a tagged variant."""
            return variant.obj.tag

        def GetVariantValue(self, variant: Ptr[EcoVariant], out_value: Ptr[EcoValue]) -> Int16:
            """Copy the union-value half of a tagged variant into an out-parameter."""
            out_value.obj = variant.obj.value
            return EcoErrorCode.SUCCESS

        def SortArrayWith(self, arr: Ptr[Int32], size: UInt32, cmp: EcoCompareFunc) -> Int16:
            """Sort an array in-place using a user-supplied comparator callback."""
            values = [arr[i] for i in range(size)]
            values.sort(key=functools.cmp_to_key(lambda a, b: cmp(a, b)))
            for idx, value in enumerate(values):
                arr[idx] = Int32(value)
            return EcoErrorCode.SUCCESS

        def StringLength(self, string: CString) -> Int32:
            """Return the length of a C string passed as a parameter."""
            return len(string) if string else 0


@factory(component=EcoTest)
class EcoTestFactory(IEcoComponentFactory):
    """Factory exposing `EcoTest` through the `Eco.ACOM2Python` bridge."""

    name: CString = b"Eco.Test"
    version: CString = b"1.0.0.0"
    manufacturer: CString = b"Eco"


singleton, get_component_factory = export(EcoTestFactory)
