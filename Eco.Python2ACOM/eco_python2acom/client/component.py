"""Component wrapper for ACOM components.

This module provides the ComponentWrapper class that wraps ACOM
component pointers and provides a Pythonic interface for calling methods.
"""

from __future__ import annotations

import ctypes
from typing import Any, Optional

from eco_python2acom.core.errors import EcoError, EcoErrorCode, check_result
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import VoidPtr
from eco_python2acom.interfaces.decorators import InterfaceDescriptor


class ComponentWrapper:
    """Wrapper for ACOM component instances.

    Provides a Pythonic interface to call component methods
    through the virtual table.

    Args:
        ptr: Raw pointer to the component interface.
        interface_class: The interface class (decorated with @interface).

    Attributes:
        ptr: The raw component pointer.
        interface_class: The interface class.
        typed_ptr: The typed pointer to the interface structure.
    """

    def __init__(self, ptr: VoidPtr, interface_class: type) -> None:
        if not ptr:
            raise EcoError(EcoErrorCode.POINTER, message="Component pointer is NULL")

        if not hasattr(interface_class, "_descriptor_"):
            raise ValueError(
                f"{interface_class.__name__} is not a decorated interface. "
                "Use @interface decorator."
            )

        self._ptr = ptr
        self._interface_class = interface_class
        self._descriptor: InterfaceDescriptor = interface_class._descriptor_

        # Cast to typed pointer
        interface_type = self._descriptor.interface_type
        if interface_type is None:
            raise EcoError(
                EcoErrorCode.POINTER,
                message=f"Interface {interface_class.__name__} has no generated interface_type",
            )
        self._typed_ptr = ctypes.cast(ptr, ctypes.POINTER(interface_type))

    @property
    def ptr(self) -> VoidPtr:
        """Get the raw component pointer."""
        return self._ptr

    @property
    def interface_class(self) -> type:
        """Get the interface class."""
        return self._interface_class

    @property
    def typed_ptr(self) -> Any:
        """Get the typed pointer."""
        return self._typed_ptr

    @property
    def iid(self) -> UGUID:
        """Get the interface ID."""
        return self._descriptor.iid

    def _get_vtbl(self) -> Any:
        """Get the VTbl from the typed pointer."""
        return self._typed_ptr.contents.pVTbl.contents

    def call_method(self, method_name: str, *args: Any) -> Any:
        """Call a method on the component by name.

        Args:
            method_name: The method name.
            *args: Method arguments.

        Returns:
            The method return value.

        Raises:
            AttributeError: If the method doesn't exist.
        """
        vtbl = self._get_vtbl()

        if not hasattr(vtbl, method_name):
            raise AttributeError(
                f"Interface {self._interface_class.__name__} has no method '{method_name}'"
            )

        method_func = getattr(vtbl, method_name)
        return method_func(self._typed_ptr, *args)

    def __getattr__(self, name: str) -> Any:
        """Enable direct method calls through attribute access.

        Args:
            name: The attribute/method name.

        Returns:
            A callable for interface methods, or raises AttributeError.

        Example:
            >>> result = wrapper.Addition(10, 20)
        """
        # Check if it's a VTbl method
        try:
            vtbl = self._get_vtbl()
            if hasattr(vtbl, name):
                method_func = getattr(vtbl, name)

                def wrapper_func(*args: Any) -> Any:
                    return method_func(self._typed_ptr, *args)

                return wrapper_func
        except (ValueError, ctypes.ArgumentError):
            pass

        raise AttributeError(f"'{type(self).__name__}' object has no attribute '{name}'")

    def query_interface(
        self, interface_class: type, iid: Optional[UGUID | str] = None
    ) -> ComponentWrapper:
        """Query for another interface on this component.

        Args:
            interface_class: The interface class for the requested interface.
                Must be decorated with @interface decorator.
            iid: Optional interface ID. If not provided, uses interface_class._iid_.

        Returns:
            A new wrapper for the requested interface.

        Raises:
            EcoError: If the interface is not supported.
            ValueError: If interface_class doesn't have _iid_ attribute.
        """
        # Get IID from interface_class if not provided
        if iid is None:
            if not hasattr(interface_class, "_iid_"):
                raise ValueError(
                    f"{interface_class.__name__} has no _iid_ attribute. "
                    "Make sure it's decorated with @interface."
                )
            guid = interface_class._iid_
        elif isinstance(iid, str):
            guid = UGUID(iid)
        else:
            guid = iid

        # Prepare output pointer
        ppv: VoidPtr = VoidPtr()

        # Call QueryInterface
        vtbl = self._get_vtbl()
        result = vtbl.QueryInterface(self._typed_ptr, ctypes.byref(guid), ctypes.byref(ppv))

        check_result(result, operation="QueryInterface")

        return ComponentWrapper(ppv, interface_class)

    def add_ref(self) -> int:
        """Increment the reference count.

        Returns:
            The new reference count.
        """
        vtbl = self._get_vtbl()
        result: int = vtbl.AddRef(self._typed_ptr)
        return result

    def release(self) -> int:
        """Decrement the reference count.

        Returns:
            The new reference count.
        """
        vtbl = self._get_vtbl()
        result: int = vtbl.Release(self._typed_ptr)
        return result

    def __enter__(self) -> ComponentWrapper:
        """Enter context manager for reference counting."""
        self.add_ref()
        return self

    def __exit__(self, exc_type: Any, exc_val: Any, exc_tb: Any) -> None:
        """Exit context manager for reference counting."""
        self.release()

    def __repr__(self) -> str:
        """Return string representation."""
        return (
            f"ComponentWrapper("
            f"interface={self._interface_class.__name__}, "
            f"iid={self.iid.to_string()})"
        )
