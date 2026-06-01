"""Global pin-registry that keeps Python instances alive while exposed to C.

Holds Python instances whose vtable address is currently held by the C side,
so refcount-based collection cannot pull them out from under the C caller.
"""

from typing import Any

ALIVE: dict[int, Any] = {}


__all__ = ["ALIVE"]
