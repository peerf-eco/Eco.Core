"""Decorator for defining ACOM component factories implemented in Python.

A factory is a small companion class for every `@component`-decorated class.
It implements `IEcoComponentFactory` and is the single entry point for creating
instances of the component.
"""

from collections.abc import Callable
from dataclasses import dataclass
from typing import Optional, TypeVar, dataclass_transform

from eco_python2acom.guids.iid import IID_IEcoComponentFactory, IID_IEcoUnknown
from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.interfaces.unknown import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, UInt32, Void
from eco_python2acom.types.errors import EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import addressof

C = TypeVar("C", bound=type)


# -----------------------------------------------------------------------------
# IEcoUnknown methods
# -----------------------------------------------------------------------------


def _make_factory_query_interface(supported: set[UGUID]) -> Callable[..., Int16]:
    """Build a standalone `QueryInterface` for a factory class.

    Args:
        supported: A set of UGUIDs representing the IIDs that this factory supports.
    """

    def QueryInterface(self, iid: Ptr[UGUID], out: Ptr[Ptr[Void]]) -> Int16:
        """Query for an interface supported by this factory.

        Args:
            iid: Pointer to the requested interface ID.
            out: Output pointer to receive the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out):
            return Int16(EcoErrorCode.POINTER)
        if iid.obj in supported:
            out.obj.value = addressof(self)
            self.AddRef()
            return Int16(EcoErrorCode.SUCCESS)
        out.obj.value = 0
        return Int16(EcoErrorCode.NOINTERFACE)

    return QueryInterface


def _make_factory_add_ref() -> Callable[..., UInt32]:
    """Build the `AddRef` method for a factory class."""

    def AddRef(self) -> UInt32:
        """Increment the factory's reference count.

        Returns:
            The new reference count after incrementing.
        """
        self.refs += 1
        return UInt32(self.refs)

    return AddRef


def _make_factory_release() -> Callable[..., UInt32]:
    """Build the `Release` method for a factory class.

    Unlike a component, a factory is a static object that simply
    decrements its counter.
    """

    def Release(self) -> UInt32:
        """Decrement the factory's reference count.

        Returns:
            The new reference count after decrementing.
        """
        self.refs -= 1
        return UInt32(self.refs)

    return Release


# -----------------------------------------------------------------------------
# IEcoComponentFactory methods
# -----------------------------------------------------------------------------


def _make_alloc(cls: type) -> Callable[..., Int16]:
    """Build the `Alloc` method for a factory class.

    Mirrors `CEcoXxxFactory` method `Alloc`:
      1. Reject aggregation requests for non-`IEcoUnknown` IIDs.
      2. Construct the component.
      3. Run the explicit allocation phase: `__eco_new__(system, outer)`.
      4. Run the explicit initialisation phase: `__eco_init__(system)`.
      5. Hand the requested interface to the caller via `QueryInterface`.
      6. Release the temporary reference held by this factory.

    Args:
        cls: The `@component`-decorated class this factory creates.
    """

    def Alloc(
        self,
        system: Optional[Ptr[IEcoUnknown]],
        outer: Optional[Ptr[IEcoUnknown]],
        iid: Ptr[UGUID],
        out: Ptr[Ptr[Void]],
    ) -> Int16:
        """Allocate a new component instance.

        Args:
            system: Pointer to system interface (can be NULL).
            outer: Outer unknown for aggregation (can be NULL).
            iid: Requested interface ID.
            out: Output pointer for the interface.

        Returns:
            0 on success, error code otherwise.
        """
        if not bool(iid) or not bool(out) or not bool(system):
            return Int16(EcoErrorCode.POINTER)

        # Aggregation is only valid when the caller is asking for `IEcoUnknown`.
        if bool(outer) and iid.obj != IID_IEcoUnknown:
            return Int16(EcoErrorCode.NOAGGREGATION)

        instance = cls()
        result = instance.__eco_new__(system, outer)
        if result.value != 0:
            return result

        result = instance.__eco_init__(system)
        if result.value != 0:
            return result

        result = instance.QueryInterface(iid, out)
        if result.value != 0:
            return result

        # Drop the temporary reference from this factory.
        instance.Release()
        return Int16(EcoErrorCode.SUCCESS)

    return Alloc


def _make_init() -> Callable[..., Int16]:
    """Build the `Init` method for a factory class."""

    def Init(self, system: Optional[Ptr[IEcoUnknown]], context: Ptr[Void]) -> Int16:
        """Initialize the factory with system context.

        Args:
            system: Pointer to system interface (can be NULL).
            context: Additional context (e.g., bus pointer).

        Returns:
            0 on success, error code otherwise.
        """
        return Int16(EcoErrorCode.SUCCESS)

    return Init


def _make_get_name() -> Callable[..., CString]:
    """Build the `get_Name` method for a factory class."""

    def get_Name(self) -> CString:
        """Get the component name.

        Returns:
            Pointer to null-terminated string.
        """
        return CString(self.name)

    return get_Name


def _make_get_version() -> Callable[..., CString]:
    """Build the `get_Version` method for a factory class."""

    def get_Version(self) -> CString:
        """Get the component version.

        Returns:
            Pointer to null-terminated string.
        """
        return CString(self.version)

    return get_Version


def _make_get_manufacturer() -> Callable[..., CString]:
    """Build the `get_Manufacturer` method for a factory class."""

    def get_Manufacturer(self) -> CString:
        """Get the component manufacturer.

        Returns:
            Pointer to null-terminated string.
        """
        return CString(self.manufacturer)

    return get_Manufacturer


# -----------------------------------------------------------------------------
# Decorator
# -----------------------------------------------------------------------------


@dataclass_transform()
def factory(component: type) -> Callable[[C], C]:
    """Decorator for defining ACOM component factories implemented in Python.

    Converts a class into an `IEcoComponentFactory` implementation that creates
    instances of the specified component class.

    Args:
        component: The `@component`-decorated class that this factory creates instances of.

    Returns:
        A decorator that turns the class into an EcoOS factory class.

    Raises:
        TypeError: If class is not decorated with @component, or
            the decorated class does not inherit from `IEcoComponentFactory`.
    """
    if not isinstance(component, type) or not getattr(component, "_eco_component_", False):
        raise TypeError(
            f"Class '{component.__name__}' is not a component and cannot be used in a factory"
        )

    def decorator(cls: C) -> C:
        """Converts the decorated class into an `IEcoComponentFactory` implementation."""
        if not issubclass(cls, IEcoComponentFactory):
            raise TypeError(f"Factory '{cls.__name__}' must inherit from 'IEcoComponentFactory'")

        supported = {IID_IEcoComponentFactory, IID_IEcoUnknown}

        cls.QueryInterface = _make_factory_query_interface(supported)
        cls.AddRef = _make_factory_add_ref()
        cls.Release = _make_factory_release()
        cls.Alloc = _make_alloc(component)
        cls.Init = _make_init()
        cls.get_Name = _make_get_name()
        cls.get_Version = _make_get_version()
        cls.get_Manufacturer = _make_get_manufacturer()

        cls._eco_factory_ = True
        cls._eco_component_cls_ = component

        cls = dataclass(init=True, repr=False, eq=False)(cls)
        return cls

    return decorator


__all__ = ["factory"]
