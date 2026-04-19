"""Component Identifiers (CID) for EcoOS components.

CIDs uniquely identify component implementations. When you call
QueryComponent on the InterfaceBus, you specify which component
to instantiate using its CID.

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

# Mapping of CIDs to human-readable names
CID_NAMES: dict[UGUID, str] = {
    CID_EcoInterfaceBus1: "Eco.InterfaceBus1",
    CID_EcoMemoryManager1: "Eco.MemoryManager1",
    CID_EcoFileSystemManagement1: "Eco.FileSystemManagement1",
}


def get_component_name(cid: UGUID) -> str:
    """Get human-readable component name from CID.

    Args:
        cid: Component identifier.

    Returns:
        Component name string, or hex representation if unknown.
    """
    return CID_NAMES.get(cid, cid.to_string())
