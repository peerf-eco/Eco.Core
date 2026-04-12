"""Generation IDs (GID) for EcoOS architectures.

GIDs identify the target architecture/platform for runtime libraries.

Note:
    GID format is UGUID: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
"""

import struct
import sys

from eco_python2acom.types.guid import UGUID

# x86 32 bit GID = {00000000-0000-0000-0000-000014C00200}
GID_IEcoSystem_x86_32 = UGUID.from_string("00000000-0000-0000-0000-000014C00200")

# x86 64 bit (AMD64) GID = {00000000-0000-0000-0000-000086640300}
GID_IEcoSystem_x86_64 = UGUID.from_string("00000000-0000-0000-0000-000086640300")


# =============================================================================
# Runtime selection
# =============================================================================

if sys.platform != "win32":
    raise RuntimeError("EcoOS library is supported only on Windows")

_POINTER_BITS = struct.calcsize("P") * 8

if _POINTER_BITS == 32:
    GID_IEcoSystem = GID_IEcoSystem_x86_32
elif _POINTER_BITS == 64:
    GID_IEcoSystem = GID_IEcoSystem_x86_64
else:
    raise RuntimeError(f"Unsupported process architecture: {_POINTER_BITS}-bit")
