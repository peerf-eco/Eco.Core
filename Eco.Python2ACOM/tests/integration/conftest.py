"""Fixtures for integration tests with EcoOS runtime."""

from collections.abc import Generator
from pathlib import Path

import pytest

from eco_python2acom.runtime.loader import EcoLibLoader
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.runtime.utils import LIB_EXTENSION
from eco_python2acom.types.core import Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from tests.integration.structures import CID_EcoTest, IEcoTest, IID_IEcoTest

# Expected DLL filename derived from CID
ECO_TEST_LIB = CID_EcoTest.to_string(with_hyphens=False) + LIB_EXTENSION

# Path to the directory containing test DLLs
DATA_DIR = Path(__file__).resolve().parent.parent.parent / "data"


@pytest.fixture(scope="session")
def eco_test_lib_path() -> Path:
    """Fixture returning the path to `Eco.Test` library, skipping if missing."""
    path = DATA_DIR / ECO_TEST_LIB
    if not path.is_file():
        pytest.skip(f"'Eco.Test' library not found: {path}")
    return path


@pytest.fixture(scope="session")
def eco_system() -> Generator[EcoSystem, None, None]:
    """Session-scoped EcoSystem fixture.

    Initializes the runtime once for all integration tests.
    Skips the entire session if runtime is unavailable or initialization fails.
    """
    try:
        eco = EcoSystem(lib_dir=DATA_DIR)
    except Exception:
        pytest.skip("`EcoSystem` runtime unavailable or failed to initialize")

    with eco:
        yield eco


@pytest.fixture(scope="session")
def eco_test(
    eco_system: EcoSystem, eco_test_lib_path: Path
) -> Generator[Ptr[IEcoTest], None, None]:
    """Session-scoped IEcoTest component fixture.

    Queries the `Eco.Test` component from the bus.
    Skips if the library is not found or component cannot be created.
    """
    ppv = Ptr[Void]()
    result = eco_system.bus.obj.QueryComponent(
        byref(CID_EcoTest),
        None,
        byref(IID_IEcoTest),
        byref(ppv),
    )
    if result != 0 or not bool(ppv):
        pytest.skip(f"Failed to create 'Eco.Test' component (error = {result})")

    test_iface = cast(ppv, Ptr[IEcoTest])
    yield test_iface
    test_iface.obj.Release()


@pytest.fixture
def eco_lib_loader() -> EcoLibLoader:
    """Fixture returning a `EcoLibLoader` instance."""
    return EcoLibLoader()
