"""Unit tests for `eco_python2acom.types.function` module.

This module tests the generic `Func[R, [A, B, ...]]` type including metaclass
subscript syntax, restype substitution rules, instance creation, representation,
and error handling.

Test Classes:
    TestFuncCreation: Tests for `Func[R, [A, ...]]` type creation and attributes.
    TestFuncRestypeSubstitution: Tests for restype substitution rules.
    TestFuncInstantiation: Tests for wrapping Python callables into callbacks.
    TestFuncRepr: Tests for `__repr__` of function pointer instances.
    TestFuncValidation: Tests for subscript parameter validation.
    TestFuncCaching: Tests for metaclass type caching.
"""

import pytest

from eco_python2acom.types.core import Double, Int16, Int32, UInt8, Void
from eco_python2acom.types.function import Func
from eco_python2acom.types.pointer import Ptr


@pytest.mark.unit
class TestFuncCreation:
    """Tests for `Func[R, [A, ...]]` type creation and attributes.

    Verifies subscript syntax, return/argument type storage, and class naming.
    """

    def test_create_func_type(self) -> None:
        """Verifies `Func[R, [A, B]]` returns a callable class with stored signature."""
        FuncType = Func[Int32, [Int32, Int32]]
        assert FuncType._restype_ is Int32
        assert FuncType._argtypes_ == (Int32, Int32)

    def test_create_func_no_args(self) -> None:
        """Verifies `Func[R, []]` creates a zero-argument function type."""
        FuncType = Func[Int32, []]
        assert FuncType._restype_ is Int32
        assert FuncType._argtypes_ == ()

    def test_create_func_void_return(self) -> None:
        """Verifies `Void` is an accepted return type."""
        FuncType = Func[Void, [Int32]]
        assert FuncType._restype_ is None

    def test_class_name_uses_type_names(self) -> None:
        """Verifies display name uses canonical EcoOS type labels."""
        FuncType = Func[Int32, [Double, UInt8]]
        assert FuncType.__name__ == "Func[Int32, [Double, UInt8]]"

    def test_class_name_empty_argtypes(self) -> None:
        """Verifies display name renders empty argtypes cleanly."""
        FuncType = Func[Int16, []]
        assert FuncType.__name__ == "Func[Int16, []]"


@pytest.mark.unit
class TestFuncRestypeSubstitution:
    """Tests for the restype substitution rules applied when building the signature.

    Notes:
        - `Void` becomes `None`.
        - Any typed `Ptr[T]` becomes `Ptr[Void]`.
    """

    def test_void_restype_is_translated_to_none(self) -> None:
        """`Func[Void, [...]]._restype_` is `None`."""
        FuncType = Func[Void, [Int32]]
        assert FuncType._restype_ is None

    def test_typed_pointer_restype_is_substituted_with_void_pointer(self) -> None:
        """`Func[Ptr[Int32], [...]]._restype_` is `Ptr[Void]`."""
        FuncType = Func[Ptr[Int32], [Int32]]
        assert FuncType._restype_ is Ptr[Void]

    def test_void_pointer_restype_is_unchanged(self) -> None:
        """`Func[Ptr[Void], [...]]._restype_` stays `Ptr[Void]`."""
        FuncType = Func[Ptr[Void], [Int32]]
        assert FuncType._restype_ is Ptr[Void]

    def test_scalar_restype_is_unchanged(self) -> None:
        """`Func[Int32, [...]]._restype_` stays as the declared scalar."""
        FuncType = Func[Int32, [Int32]]
        assert FuncType._restype_ is Int32


@pytest.mark.unit
class TestFuncInstantiation:
    """Tests for wrapping Python callables into function pointer instances.

    Verifies that a Python callable can be wrapped as a callback.
    """

    def test_wrap_callable(self) -> None:
        """Verifies a Python callable can be wrapped into a function pointer."""
        Compare = Func[Int32, [Int32, Int32]]

        def cmp(a: int, b: int) -> int:
            return a - b

        inst = Compare(cmp)
        assert inst is not None

    def test_null_instance_is_falsy(self) -> None:
        """Verifies a default-constructed instance is falsy (NULL)."""
        Compare = Func[Int32, [Int32, Int32]]
        inst = Compare()
        assert not bool(inst)


@pytest.mark.unit
class TestFuncRepr:
    """Tests for `__repr__` of function pointer instances."""

    def test_repr_null(self) -> None:
        """Verifies `__repr__` marks NULL instances clearly."""
        Compare = Func[Int32, [Int32, Int32]]
        inst = Compare()
        assert repr(inst) == "<Func[Int32, [Int32, Int32]] NULL>"

    def test_repr_non_null_has_address(self) -> None:
        """Verifies `__repr__` of a wrapped callable includes an address."""
        Compare = Func[Int32, [Int32, Int32]]

        def cmp(a: int, b: int) -> int:
            return a - b

        inst = Compare(cmp)
        text = repr(inst)
        assert text.startswith("<Func[Int32, [Int32, Int32]] 0x")
        assert text.endswith(">")


@pytest.mark.unit
class TestFuncValidation:
    """Tests for `Func` subscript parameter validation.

    Verifies error handling for invalid subscript arguments.
    """

    @pytest.mark.parametrize(
        ["params", "err", "match"],
        [
            ((Int32,), ValueError, "2 parameters"),
            ((Int32, Int32, Int32), ValueError, "2 parameters"),
            ((Int32, Int32), TypeError, "must be a list"),
            ((Int32, (Int32,)), TypeError, "must be a list"),
            (("not_a_type", [Int32]), TypeError, "return type must be a type"),
            ((Int32, ["not_a_type"]), TypeError, "argument #0 must be a type"),
            ((Int32, [Int32, "not_a_type"]), TypeError, "argument #1 must be a type"),
            ((Int32, [Void]), ValueError, "cannot be 'Void'"),
            ((Int32, [Int32, Void]), ValueError, "argument #1 cannot be 'Void'"),
            ((Int32, [int]), TypeError, "Cannot create"),
        ],
        ids=[
            "single_param",
            "three_params",
            "argtypes_not_list",
            "argtypes_tuple",
            "non_type_restype",
            "non_type_argtype_first",
            "non_type_argtype_middle",
            "void_single_argtype",
            "void_among_argtypes",
            "non_valid_argtype",
        ],
    )
    def test_invalid_subscript_params(
        self, params: tuple, err: type[Exception], match: str
    ) -> None:
        """Verifies invalid subscript parameters raise the expected exception."""
        with pytest.raises(err, match=match):
            Func[params]


@pytest.mark.unit
class TestFuncCaching:
    """Tests for metaclass type caching.

    Verifies that `Func[R, [A, ...]]` returns the same type for identical signatures.
    """

    def test_same_signature_returns_same_type(self) -> None:
        """Verifies cache hit for identical signatures."""
        type1 = Func[Int32, [Int32, Int32]]
        type2 = Func[Int32, [Int32, Int32]]
        assert type1 is type2

    def test_different_restype_returns_different_types(self) -> None:
        """Verifies different return types produce different function types."""
        type1 = Func[Int32, [Int32]]
        type2 = Func[Int16, [Int32]]
        assert type1 is not type2

    def test_different_argtypes_returns_different_types(self) -> None:
        """Verifies different argument types produce different function types."""
        type1 = Func[Int32, [Int32, Int32]]
        type2 = Func[Int32, [Int32, UInt8]]
        type3 = Func[Int32, [Int32]]
        assert type1 is not type2
        assert type1 is not type3
