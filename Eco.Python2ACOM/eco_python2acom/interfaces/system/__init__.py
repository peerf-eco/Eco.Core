"""System interfaces for EcoOS runtime.

This package contains core system interfaces defined declaratively
using @interface and @method decorators with proper inheritance.

All interfaces inherit from IEcoUnknown (QueryInterface, AddRef, Release).

Submodules:
    system: IEcoSystem1 -- the main entry point.
    interface_bus: IEcoInterfaceBus1 and extensions.
    memory_manager: IEcoMemoryManager1 and IEcoMemoryAllocator1.
    file_manager: IEcoFileSystemManagement1 and related interfaces.
    connections: Connection point interfaces (IEcoConnectionPoint, etc.).
"""

from eco_python2acom.interfaces.system.connections import (
    EcoConnectionData,
    IEcoConnectionPoint,
    IEcoConnectionPointContainer,
    IEcoEnumConnectionPoints,
    IEcoEnumConnections,
)
from eco_python2acom.interfaces.system.file_manager import (
    IEcoDirectoryManager1,
    IEcoDriveManager1,
    IEcoFileManager1,
    IEcoFileSystemManagement1,
)
from eco_python2acom.interfaces.system.interface_bus import (
    IEcoInterfaceBus1,
    IEcoInterfaceBus1FileExt,
    IEcoInterfaceBus1MemExt,
)
from eco_python2acom.interfaces.system.memory_manager import (
    IEcoMemoryAllocator1,
    IEcoMemoryManager1,
)
from eco_python2acom.interfaces.system.system import IEcoSystem1

__all__ = [
    # System
    "IEcoSystem1",
    # InterfaceBus
    "IEcoInterfaceBus1",
    "IEcoInterfaceBus1MemExt",
    "IEcoInterfaceBus1FileExt",
    # Memory
    "IEcoMemoryManager1",
    "IEcoMemoryAllocator1",
    # FileSystem
    "IEcoFileSystemManagement1",
    "IEcoFileManager1",
    "IEcoDirectoryManager1",
    "IEcoDriveManager1",
    # Connection points
    "EcoConnectionData",
    "IEcoConnectionPoint",
    "IEcoConnectionPointContainer",
    "IEcoEnumConnectionPoints",
    "IEcoEnumConnections",
]
