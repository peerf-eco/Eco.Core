"""DLL loader for EcoOS components.

This module provides functionality to load DLLs and extract
component factories.

Classes:
    LoadedDll: Information about a loaded DLL.
    DllLoader: Loader for EcoOS component DLLs.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from eco_python2acom.core.errors import EcoError, EcoErrorCode
from eco_python2acom.core.guid import UGUID
from eco_python2acom.core.types import DLL, VoidPtr
from eco_python2acom.interfaces.base import IEcoComponentFactory
from eco_python2acom.runtime.helpers import guid_to_filename


@dataclass
class LoadedDll:
    """Information about a loaded DLL.

    Attributes:
        path: Full path to the DLL file.
        handle: ctypes DLL handle.
        factory: The component factory interface.
    """

    path: Path
    handle: DLL
    factory: IEcoComponentFactory


class DllLoader:
    """Loader for EcoOS component DLLs.

    Handles loading DLLs and extracting component factories
    via GetIEcoComponentFactoryPtr export.
    """

    def load(self, path: Path) -> LoadedDll:
        """Load a DLL and extract its component factory.

        Args:
            path: Path to the DLL file.

        Returns:
            LoadedDll with handle and factory.

        Raises:
            FileNotFoundError: If DLL file does not exist.
            EcoError: If factory extraction fails.
        """
        # Verify file exists
        path = path.resolve()
        if not path.exists():
            raise FileNotFoundError(f"DLL not found: {path}")

        # Load DLL
        try:
            handle = DLL(str(path))
        except OSError as err:
            raise EcoError(EcoErrorCode.FAIL, f"Failed to load DLL: {path}. Error: {err}") from err

        # Get factory pointer
        try:
            get_factory = handle.GetIEcoComponentFactoryPtr
            get_factory.restype = VoidPtr
            get_factory.argtypes = []
            factory_ptr: VoidPtr = get_factory()
        except AttributeError as err:
            raise EcoError(
                EcoErrorCode.COMPONENT_NOTFOUND,
                f"DLL does not export GetIEcoComponentFactoryPtr: {path}",
            ) from err
        return LoadedDll(
            path=path,
            handle=handle,
            factory=IEcoComponentFactory(factory_ptr),
        )

    def load_by_cid(self, cid: UGUID, search_paths: list[Path]) -> LoadedDll:
        """Load a DLL by component ID, searching in provided paths.

        Args:
            cid: Component ID (CID).
            search_paths: List of directories to search.

        Returns:
            LoadedDll with handle and factory.

        Raises:
            FileNotFoundError: If DLL not found in any search path.
        """
        filename = guid_to_filename(cid)

        for search_path in search_paths:
            dll_path = search_path / filename
            if dll_path.exists():
                return self.load(dll_path)

        raise FileNotFoundError(f"DLL not found for CID {cid.to_string()} in paths: {search_paths}")
