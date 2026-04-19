"""Connection point interfaces for EcoOS.

This module defines ACOM interfaces for the connection point pattern,
which allows connectable objects to expose outgoing interfaces (sinks)
and clients to establish or tear down connections. The model is
analogous to COM's IConnectionPoint and IConnectionPointContainer.

Interfaces:
    IEcoConnectionPoint: A single connection point; manage Advise/Unadvise.
    IEcoConnectionPointContainer: Container of connection points; find or enumerate.
    IEcoEnumConnectionPoints: Enumerator over connection points.
    IEcoEnumConnections: Enumerator over active connections (sink + cookie).

Structures:
    EcoConnectionData: Pair of sink pointer and connection cookie.

Reference:
    Based on IEcoConnectionPoint.h, IEcoConnectionPointContainer.h,
    IEcoEnumConnectionPoints.h, IEcoEnumConnections.h from Eco.Core1/SharedFiles.
"""

from __future__ import annotations

from eco_python2acom.decorators.interface import interface
from eco_python2acom.decorators.model import model
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

    Used by IEcoEnumConnections.Next to return the list of
    connected sinks and their cookies.

    Attributes:
        pUnk: Pointer to the sink's IEcoUnknown (client-side object).
        cCookie: Connection cookie returned by Advise.
    """

    pUnk: Ptr[Void]
    cCookie: UInt32


# =============================================================================
# IEcoConnectionPoint
# =============================================================================


@interface(iid=IID_IEcoConnectionPoint)
class IEcoConnectionPoint(IEcoUnknown):
    """Single connection point for one outgoing interface.

    A connectable object implements one IEcoConnectionPoint per
    outgoing interface (IID). Clients call Advise with their sink
    to receive callbacks; Unadvise(cookie) disconnects.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    def GetConnectionInterface(self, pIID: Ptr[UGUID]) -> Int16:
        """Get the IID of the outgoing interface supported by this point.

        Args:
            pIID: Output pointer to receive the interface ID.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def GetConnectionPointContainer(self, ppCPC: Ptr[Ptr[IEcoConnectionPointContainer]]) -> Int16:
        """Get the connection point container that owns this point.

        Args:
            ppCPC: Output pointer to receive IEcoConnectionPointContainer.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Advise(self, pUnkSink: Ptr[IEcoUnknown], pcCookie: Ptr[UInt32]) -> Int16:
        """Establish a connection between this point and the client's sink.

        Args:
            pUnkSink: Pointer to the client's sink.
            pcCookie: Output cookie identifying this connection; pass to
                Unadvise to disconnect.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Unadvise(self, cCookie: UInt32) -> Int16:
        """Terminate a connection previously established by Advise.

        Args:
            cCookie: Cookie returned by Advise.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def EnumConnections(self, ppEnum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
        """Create an enumerator over current connections.

        Args:
            ppEnum: Output pointer to receive IEcoEnumConnections.

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

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    def EnumConnectionPoints(self, ppEnum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
        """Create an enumerator over all connection points.

        Args:
            ppEnum: Output pointer to receive IEcoEnumConnectionPoints.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def FindConnectionPoint(self, riid: Ptr[UGUID], ppCP: Ptr[Ptr[IEcoConnectionPoint]]) -> Int16:
        """Find the connection point for a given outgoing interface IID.

        Args:
            riid: Interface ID of the outgoing interface.
            ppCP: Output pointer to receive IEcoConnectionPoint.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoEnumConnectionPoints
# =============================================================================


@interface(iid=IID_IEcoEnumConnectionPoints)
class IEcoEnumConnectionPoints(IEcoUnknown):
    """Enumerator over connection points.

    Standard enumeration interface: Next, Skip, Reset, Clone.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    def Next(
        self,
        cConnections: UInt32,
        ppCP: Ptr[Ptr[IEcoConnectionPoint]],
        pcFetched: Ptr[UInt32],
    ) -> Int16:
        """Retrieve the next connection point(s).

        Args:
            cConnections: Number of connection points to fetch (typically 1).
            ppCP: Output array of IEcoConnectionPoint pointers.
            pcFetched: Output number of pointers actually returned.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Skip(self, cConnections: UInt32) -> Int16:
        """Skip the next cConnections elements in the enumeration.

        Args:
            cConnections: Number of elements to skip.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Reset(self) -> Int16:
        """Reset the enumerator to the beginning.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Clone(self, ppEnum: Ptr[Ptr[IEcoEnumConnectionPoints]]) -> Int16:
        """Create a copy of the enumerator with the same state.

        Args:
            ppEnum: Output pointer to receive IEcoEnumConnectionPoints.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoEnumConnections
# =============================================================================


@interface(iid=IID_IEcoEnumConnections)
class IEcoEnumConnections(IEcoUnknown):
    """Enumerator over active connections (sink + cookie pairs).

    Returned by IEcoConnectionPoint.EnumConnections. Each element
    is an EcoConnectionData (pUnk, cCookie).

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    def Next(
        self,
        cConnections: UInt32,
        rgcd: Ptr[EcoConnectionData],
        pcFetched: Ptr[UInt32],
    ) -> Int16:
        """Retrieve the next connection data element(s).

        Args:
            cConnections: Number of elements to fetch.
            rgcd: Output array of EcoConnectionData structures.
            pcFetched: Output number of elements actually returned.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Skip(self, cConnections: UInt32) -> Int16:
        """Skip the next cConnections elements.

        Args:
            cConnections: Number of elements to skip.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Reset(self) -> Int16:
        """Reset the enumerator to the beginning.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def Clone(self, ppEnum: Ptr[Ptr[IEcoEnumConnections]]) -> Int16:
        """Create a copy of the enumerator with the same state.

        Args:
            ppEnum: Output pointer to receive IEcoEnumConnections.

        Returns:
            0 on success, error code otherwise.
        """
        ...
