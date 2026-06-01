"""Fixtures for integration tests with EcoOS runtime."""

from collections.abc import Generator
from pathlib import Path

import pytest

from eco_python2acom.guids.iid import IID_IEcoACOM2Python
from eco_python2acom.interfaces.acom2python import IEcoACOM2Python
from eco_python2acom.runtime.loader import EcoLibLoader
from eco_python2acom.runtime.system import EcoSystem
from eco_python2acom.runtime.utils import LIB_EXTENSION
from eco_python2acom.types.core import Void
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import byref, cast
from tests.integration.structures import CID_EcoACOM2Python, CID_EcoTest, IEcoTest, IID_IEcoTest

# Expected DLL filename derived from CID
ECO_TEST_LIB = CID_EcoTest.to_string(with_hyphens=False) + LIB_EXTENSION

# Path to the directory containing test DLLs
DATA_DIR = Path(__file__).resolve().parent.parent.parent / "data"

# Path to the Python module that hosts `EcoTest` and `get_component_factory`
ECO_TEST_PATH = Path(__file__).resolve().parent / "structures.py"


@pytest.fixture(scope="session")
def eco_test_lib_path() -> Path:
    """Fixture returning the path to `Eco.Test` library, skipping if missing."""
    path = DATA_DIR / ECO_TEST_LIB
    if not path.is_file():
        pytest.skip(f"'Eco.Test' library not found: {path}")
    return path


@pytest.fixture(scope="session")
def eco_system() -> Generator[EcoSystem, None, None]:
    """Session-scoped `EcoSystem` fixture.

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
def eco_bridge(eco_system: EcoSystem) -> Generator[Ptr[IEcoACOM2Python], None, None]:
    """Session-scoped `IEcoACOM2Python` bridge fixture.

    Queries the bridge interface from the bus once per session, so Python
    component factories can be registered through it. Skips if the bridge
    is not exposed by the loaded runtime.
    """
    bridge_ptr = Ptr[Void]()
    result = eco_system.bus.obj.QueryComponent(
        byref(CID_EcoACOM2Python), None, byref(IID_IEcoACOM2Python), byref(bridge_ptr)
    )
    if result != 0 or not bool(bridge_ptr):
        pytest.skip(f"`Eco.ACOM2Python` bridge not available (code = {result})")

    bridge = cast(bridge_ptr, Ptr[IEcoACOM2Python])
    yield bridge
    bridge.obj.Release()


@pytest.fixture(scope="session")
def eco_test_c(
    eco_system: EcoSystem, eco_test_lib_path: Path
) -> Generator[Ptr[IEcoTest], None, None]:
    """Session-scoped `IEcoTest` component fixture backed by the native `Eco.Test.dll`."""
    ppv = Ptr[Void]()
    result = eco_system.bus.obj.QueryComponent(
        byref(CID_EcoTest), None, byref(IID_IEcoTest), byref(ppv)
    )
    if result != 0 or not bool(ppv):
        pytest.skip(f"Failed to create 'Eco.Test' component (code = {result})")

    test_iface = cast(ppv, Ptr[IEcoTest])
    yield test_iface
    test_iface.obj.Release()


@pytest.fixture(scope="session")
def eco_test_py(eco_bridge: Ptr[IEcoACOM2Python]) -> Generator[Ptr[IEcoTest], None, None]:
    """Session-scoped `IEcoTest` component fixture backed by the Python `EcoTest`.

    Registers `Eco.Test` through the `Eco.ACOM2Python` bridge under
    `CID_EcoTest`, queries an `IEcoTest` interface, and unregisters the
    factory after the session ends.
    """
    result = eco_bridge.obj.RegisterComponent(str(ECO_TEST_PATH).encode(), byref(CID_EcoTest))
    if result != 0:
        pytest.skip(f"Failed to register `Eco.Test` with bridge (code = {result})")

    ppv = Ptr[Void]()
    result = eco_bridge.obj.QueryComponent(
        byref(CID_EcoTest), None, byref(IID_IEcoTest), byref(ppv)
    )
    if result != 0 or not bool(ppv):
        eco_bridge.obj.UnRegisterComponent(byref(CID_EcoTest))
        pytest.skip(f"Failed to create `Eco.Test` component (code = {result})")

    test_iface = cast(ppv, Ptr[IEcoTest])
    yield test_iface
    test_iface.obj.Release()
    eco_bridge.obj.UnRegisterComponent(byref(CID_EcoTest))


@pytest.fixture(scope="session", params=["c", "py"], ids=["c", "py"])
def eco_test(request: pytest.FixtureRequest) -> Ptr[IEcoTest]:
    """Parameterised `IEcoTest` fixture multiplexing the C and Python implementations.

    Each test that consumes `eco_test` is automatically run twice:
        - `[c]`: against the native `Eco.Test.dll` factory on the bus;
        - `[py]`: against the `EcoTest` factory registered with the bridge.
    """
    return request.getfixturevalue(f"eco_test_{request.param}")


@pytest.fixture
def eco_lib_loader() -> EcoLibLoader:
    """Fixture returning a `EcoLibLoader` instance."""
    return EcoLibLoader()
