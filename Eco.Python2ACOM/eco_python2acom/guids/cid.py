"""Component Identifiers (CID) for EcoOS components.

CIDs uniquely identify component implementations.

Note:
    CID format is UGUID: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
"""

from eco_python2acom.types.guid import UGUID

# =============================================================================
# Core System Components
# =============================================================================

# EcoInterfaceBus1 CID = {00000000-0000-0000-0000-000042757331}
CID_EcoInterfaceBus1 = UGUID.from_string("00000000-0000-0000-0000-000042757331")

# EcoMemoryManager1 CID = {00000000-0000-0000-0000-00004D656D31}
CID_EcoMemoryManager1 = UGUID.from_string("00000000-0000-0000-0000-00004D656D31")

# EcoFileSystemManagement1 CID = {00000000-0000-0000-0000-000046534D31}
CID_EcoFileSystemManagement1 = UGUID.from_string("00000000-0000-0000-0000-000046534D31")
