"""Connection point interfaces for EcoOS.

This module defines ACOM interfaces for the connection point pattern,
which allows connectable objects to expose outgoing interfaces (sinks)
and clients to establish or tear down connections.

Interfaces:
    IEcoConnectionPoint: A single connection point; manage advise/unadvise.
    IEcoConnectionPointContainer: Container of connection points; find or enumerate.
    IEcoEnumConnectionPoints: Enumerator over connection points.
    IEcoEnumConnections: Enumerator over active connections (sink + cookie).

Reference:
    Based on `IEcoConnectionPoint.h`, `IEcoConnectionPointContainer.h`,
    `IEcoEnumConnectionPoints.h`, `IEcoEnumConnections.h` from `Eco.Core1/SharedFiles`.
"""

from __future__ import annotations

from eco_python2acom.decorators import interface, model
from eco_python2acom.guids.iid import (
    IID_IEcoConnectionPoint,
    IID_IEcoConnectionPointContainer,
    IID_IEcoEnumConnectionPoints,
    IID_IEcoEnumConnections,
)
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.core import Int16, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

# =============================================================================
# EcoConnectionData
# =============================================================================


@model
class EcoConnectionData:
    """Data for one active connection (sink + cookie).

    Attributes:
        ptr: Pointer to the sink's `IEcoUnknown` (client-side object).
        cookie: Connection cookie returned by Advise.
    """

    ptr: Ptr[Void]
    cookie: UInt32


# =============================================================================
# IEcoConnectionPoint
# =============================================================================


@interface(iid=IID_IEcoConnectionPoint)
class IEcoConnectionPoint(IEcoUnknown):
    """Single connection point for one outgoing interface.

    Allows clients to establish or terminate connections to sinks implementing
    the outgoing interface, and to enumerate current connections.
    """

    def get_connection_interface(self, iid: Ptr[UGUID]) -> Int16:
        """Get the IID of the outgoing interface supported by this point.

        Args:
            iid: Output pointer to receive the interface ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_connection_point_container(
        self, container: Ptr[Ptr[IEcoConnectionPointContainer]]
    ) -> Int16:
        """Get the connection point container that owns this point.

        Args:
            container: Output pointer to receive `IEcoConnectionPointContainer`.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def advise(self, sink: Ptr[IEcoUnknown], cookie: Ptr[UInt32]) -> Int16:
        """Establish a connection between this point and the client's sink.

        Args:
            sink: Pointer to the client's sink.
            cookie: Output cookie identifying this connection.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def unadvise(self, cookie: UInt32) -> Int16:
        """Terminate a connection identified by the given cookie.

        Args:
            cookie: Cookie identifying the connection to terminate.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def enum_connections(self, enum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
        """Create an enumerator over current connections.

        Args:
            enum: Output pointer to receive `IEcoEnumConnections`.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoConnectionPointContainer
# =============================================================================


@interface(iid=IID_IEcoConnectionPointContainer)
class IEcoConnectionPointContainer(IEcoUnknown):
    """Container of connection points on a connectable object.

    Allows clients to find a connection point by IID or enumerate
    all connection points.
    """

    def enum_connection_points(self, enum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
        """Create an enumerator over all connection points.

        Args:
            enum: Output pointer to receive `IEcoEnumConnectionPoints`.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def find_connection_point(self, iid: Ptr[UGUID], point: Ptr[Ptr[IEcoConnectionPoint]]) -> Int16:
        """Find the connection point for a given outgoing interface IID.

        Args:
            iid: Interface ID of the outgoing interface.
            point: Output pointer to receive `IEcoConnectionPoint`.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoEnumConnectionPoints
# =============================================================================


@interface(iid=IID_IEcoEnumConnectionPoints)
class IEcoEnumConnectionPoints(IEcoUnknown):
    """Enumerator over connection points."""

    def next(
        self, count: UInt32, points: Ptr[Ptr[IEcoConnectionPoint]], fetched: Ptr[UInt32]
    ) -> Int16:
        """Retrieve the next connection point(s).

        Args:
            count: Number of connection points to fetch.
            point: Output array of `IEcoConnectionPoint` pointers.
            fetched: Output number of pointers actually returned.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def skip(self, count: UInt32) -> Int16:
        """Skip the next elements in the enumeration.

        Args:
            count: Number of elements to skip.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def reset(self) -> Int16:
        """Reset the enumerator to the beginning.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def clone(self, enum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
        """Create a copy of the enumerator with the same state.

        Args:
            enum: Output pointer to receive `IEcoEnumConnectionPoints`.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoEnumConnections
# =============================================================================


@interface(iid=IID_IEcoEnumConnections)
class IEcoEnumConnections(IEcoUnknown):
    """Enumerator over active connections (sink + cookie pairs)."""

    def next(
        self,
        count: UInt32,
        data: Ptr[EcoConnectionData],
        fetched: Ptr[UInt32],
    ) -> Int16:
        """Retrieve the next connection data element(s).

        Args:
            count: Number of elements to fetch.
            data: Output array of `EcoConnectionData` structures.
            fetched: Output number of elements actually returned.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def skip(self, count: UInt32) -> Int16:
        """Skip the next elements in the enumeration.

        Args:
            count: Number of elements to skip.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def reset(self) -> Int16:
        """Reset the enumerator to the beginning.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def clone(self, enum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
        """Create a copy of the enumerator with the same state.

        Args:
            enum: Output pointer to receive `IEcoEnumConnections`.

        Returns:
            0 on success, error code otherwise.
        """
        ...
