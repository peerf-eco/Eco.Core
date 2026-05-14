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
CID_EcoInterfaceBus1 = UGUID("00000000-0000-0000-0000-000042757331")

# EcoMemoryManager1 CID = {00000000-0000-0000-0000-00004D656D31}
CID_EcoMemoryManager1 = UGUID("00000000-0000-0000-0000-00004D656D31")

# EcoFileSystemManagement1 CID = {00000000-0000-0000-0000-000046534D31}
CID_EcoFileSystemManagement1 = UGUID("00000000-0000-0000-0000-000046534D31")

# EcoList1 CID = {53884AFC-93C4-48EC-AA92-9C8D3A562281}
CID_EcoList1 = UGUID("53884AFC-93C4-48EC-AA92-9C8D3A562281")
