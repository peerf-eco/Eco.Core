"""Interface Identifiers (IID) for EcoOS interfaces.

IIDs uniquely identify interface types.

Note:
    IID format is UGUID: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
"""

from eco_python2acom.types.guid import UGUID

# =============================================================================
# Base Interfaces
# =============================================================================

# IEcoUnknown IID = {00000000-0000-0000-0000-0000000000AA}
IID_IEcoUnknown = UGUID("00000000-0000-0000-0000-0000000000AA")

# IEcoComponentFactory IID = {00000000-0000-0000-0000-000000000055}
IID_IEcoComponentFactory = UGUID("00000000-0000-0000-0000-000000000055")


# =============================================================================
# InterfaceBus Interfaces (0xA0xx)
# =============================================================================

# IEcoInterfaceBus1 IID = {00000000-0000-0000-0000-A00000000101}
IID_IEcoInterfaceBus1 = UGUID("00000000-0000-0000-0000-A00000000101")

# IEcoInterfaceBus1MemExt IID = {00000000-0000-0000-0000-A00100000101}
IID_IEcoInterfaceBus1MemExt = UGUID("00000000-0000-0000-0000-A00100000101")

# IEcoInterfaceBus1FileExt IID = {00000000-0000-0000-0000-A00200000101}
IID_IEcoInterfaceBus1FileExt = UGUID("00000000-0000-0000-0000-A00200000101")

# IEcoInterfaceBus1NetExt IID = {00000000-0000-0000-0000-A00300000101}
IID_IEcoInterfaceBus1NetExt = UGUID("00000000-0000-0000-0000-A00300000101")


# =============================================================================
# Memory Manager Interfaces (0xB0xx)
# =============================================================================

# IEcoMemoryManager1 IID = {00000000-0000-0000-0000-B00000000101}
IID_IEcoMemoryManager1 = UGUID("00000000-0000-0000-0000-B00000000101")

# IEcoMemoryAllocator1 IID = {00000000-0000-0000-0000-B00000000102}
IID_IEcoMemoryAllocator1 = UGUID("00000000-0000-0000-0000-B00000000102")


# =============================================================================
# FileSystem Interfaces (0xC0xx)
# =============================================================================

# IEcoFileSystemManagement1 IID = {00000000-0000-0000-0000-C00000000101}
IID_IEcoFileSystemManagement1 = UGUID("00000000-0000-0000-0000-C00000000101")

# IEcoFileManager1 IID = {00000000-0000-0000-0000-C00000000102}
IID_IEcoFileManager1 = UGUID("00000000-0000-0000-0000-C00000000102")

# IEcoFileInfo1 IID = {00000000-0000-0000-0000-C00000000103}
IID_IEcoFileInfo1 = UGUID("00000000-0000-0000-0000-C00000000103")

# IEcoFile1 IID = {00000000-0000-0000-0000-C00000000104}
IID_IEcoFile1 = UGUID("00000000-0000-0000-0000-C00000000104")

# IEcoDriveManager1 IID = {00000000-0000-0000-0000-C00000000105}
IID_IEcoDriveManager1 = UGUID("00000000-0000-0000-0000-C00000000105")

# IEcoDriveInfo1 IID = {00000000-0000-0000-0000-C00000000106}
IID_IEcoDriveInfo1 = UGUID("00000000-0000-0000-0000-C00000000106")

# IEcoDirectoryManager1 IID = {00000000-0000-0000-0000-C00000000107}
IID_IEcoDirectoryManager1 = UGUID("00000000-0000-0000-0000-C00000000107")

# IEcoDirectoryInfo1 IID = {00000000-0000-0000-0000-C00000000108}
IID_IEcoDirectoryInfo1 = UGUID("00000000-0000-0000-0000-C00000000108")


# =============================================================================
# Connection Point Interfaces
# =============================================================================

# IEcoEnumConnections IID = {00000002-0000-0000-C000-000000000046}
IID_IEcoEnumConnections = UGUID("00000002-0000-0000-C000-000000000046")

# IEcoConnectionPoint IID = {00000003-0000-0000-C000-000000000046}
IID_IEcoConnectionPoint = UGUID("00000003-0000-0000-C000-000000000046")

# IEcoEnumConnectionPoints IID = {00000004-0000-0000-C000-000000000046}
IID_IEcoEnumConnectionPoints = UGUID("00000004-0000-0000-C000-000000000046")

# IEcoConnectionPointContainer IID = {00000005-0000-0000-C000-000000000046}
IID_IEcoConnectionPointContainer = UGUID("00000005-0000-0000-C000-000000000046")


# =============================================================================
# Collections
# =============================================================================

# IEcoList1 IID = {5AADBCB4-846C-4576-827B-287B5E67A152}
IID_IEcoList1 = UGUID("5AADBCB4-846C-4576-827B-287B5E67A152")


# =============================================================================
# System support Interfaces
# =============================================================================

# IEcoError1 IID = {00000000-0000-0000-0000-00000000FFFE}
IID_IEcoError1 = UGUID("00000000-0000-0000-0000-00000000FFFE")

# IEcoCommandArguments1 IID = {00000000-0000-0000-0000-000000000110}
IID_IEcoCommandArguments1 = UGUID("00000000-0000-0000-0000-000000000110")

# IEcoSystemInformation1 IID = {00000000-0000-0000-0000-0000000001FF}
IID_IEcoSystemInformation1 = UGUID("00000000-0000-0000-0000-0000000001FF")


# =============================================================================
# Language bridges
# =============================================================================

# IEcoACOM2Python IID = {107F4B5C-1FEF-47CC-A7E6-3167ABB9E0A7}
IID_IEcoACOM2Python = UGUID("107F4B5C-1FEF-47CC-A7E6-3167ABB9E0A7")

# IEcoACOM2Java IID = {ED2D1283-E263-48DC-B9A8-89E10C8C3657}
IID_IEcoACOM2Java = UGUID("ED2D1283-E263-48DC-B9A8-89E10C8C3657")
