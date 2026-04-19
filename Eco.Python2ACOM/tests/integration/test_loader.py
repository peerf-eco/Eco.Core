"""Integration tests for `eco_python2acom.runtime.loader`.

These tests exercise `EcoLibLoader` against a real `Eco.Test` library from
the shared data directory, verifying that it can be loaded and its factory is usable.
"""

import shutil
from pathlib import Path

import pytest

from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.runtime.loader import EcoLib, EcoLibLoader
from eco_python2acom.runtime.utils import guid_to_lib_filename
from eco_python2acom.types.errors import EcoError, EcoErrorCode
from eco_python2acom.types.pointer import Ptr
from tests.integration.conftest import DATA_DIR
from tests.integration.structures import CID_EcoTest


@pytest.mark.integration
class TestEcoLibLoaderLoad:
    """Tests for `EcoLibLoader.load` using the real `Eco.Test` library."""

    def test_missing_file_raises_file_not_found(
        self, eco_lib_loader: EcoLibLoader, tmp_path: Path
    ) -> None:
        """Loading a non-existent file raises `FileNotFoundError`."""
        missing = tmp_path / "not_exist.dll"
        with pytest.raises(FileNotFoundError, match="not found"):
            eco_lib_loader.load(missing)

    def test_load_failure_raises_eco_error(
        self, eco_lib_loader: EcoLibLoader, tmp_path: Path
    ) -> None:
        """A bogus library file is wrapped in an `EcoError(FAIL)`."""
        lib_path = tmp_path / "broken.dll"
        lib_path.write_bytes(b"not valid")

        with pytest.raises(EcoError) as exc_info:
            eco_lib_loader.load(lib_path)

        assert exc_info.value.code == EcoErrorCode.FAIL
        assert "Failed to load library" in str(exc_info.value)

    def test_successful_load_returns_eco_lib(
        self, eco_lib_loader: EcoLibLoader, eco_test_lib_path: Path
    ) -> None:
        """A successful load returns a populated `EcoLib`."""
        result = eco_lib_loader.load(eco_test_lib_path)

        assert isinstance(result, EcoLib)
        assert result.path == eco_test_lib_path.resolve()
        assert result.handle is not None
        assert isinstance(result.factory, Ptr[IEcoComponentFactory])


@pytest.mark.integration
class TestEcoLibLoaderLoadByCid:
    """Tests for `EcoLibLoader.load_by_cid` using the real `Eco.Test` library."""

    def test_not_found_in_any_path_raises(
        self, eco_lib_loader: EcoLibLoader, tmp_path: Path
    ) -> None:
        """Searching a CID in empty paths raises `FileNotFoundError`."""
        with pytest.raises(FileNotFoundError, match="not found for CID"):
            eco_lib_loader.load_by_cid(CID_EcoTest, [tmp_path])

    def test_finds_real_library_in_data_dir(
        self, eco_lib_loader: EcoLibLoader, eco_test_lib_path: Path
    ) -> None:
        """Method locates the real `Eco.Test` DLL by CID."""
        result = eco_lib_loader.load_by_cid(CID_EcoTest, [DATA_DIR])

        assert isinstance(result, EcoLib)
        assert result.path == eco_test_lib_path.resolve()
        assert isinstance(result.factory, Ptr[IEcoComponentFactory])

    def test_first_match_wins(
        self, eco_lib_loader: EcoLibLoader, eco_test_lib_path: Path, tmp_path: Path
    ) -> None:
        """When the DLL exists in multiple paths, the first path wins."""
        filename = guid_to_lib_filename(CID_EcoTest)
        copied = tmp_path / filename
        shutil.copy(eco_test_lib_path, copied)

        result = eco_lib_loader.load_by_cid(CID_EcoTest, [tmp_path, DATA_DIR])

        assert result.path == copied.resolve()
