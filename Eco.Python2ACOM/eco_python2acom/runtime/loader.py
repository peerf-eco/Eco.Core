"""Library loader for EcoOS components.

This module provides functionality to load libraries and extract
component factories.

Classes:
    EcoLib: Information about a loaded EcoOS library.
    EcoLibLoader: Loader for EcoOS component libraries.
"""

from dataclasses import dataclass
from pathlib import Path

from eco_python2acom.interfaces.base import IEcoComponentFactory
from eco_python2acom.runtime.utils import guid_to_lib_filename
from eco_python2acom.types.core import CDLL, Void
from eco_python2acom.types.errors import EcoError, EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr


@dataclass
class EcoLib:
    """Information about a loaded EcoOS library.

    Attributes:
        path: Full path to the library file.
        handle: Library handle.
        factory: The component factory interface.
    """

    path: Path
    handle: CDLL
    factory: IEcoComponentFactory


class EcoLibLoader:
    """Loader for EcoOS component libraries.

    Handles loading shared libraries and extracting component factories
    via the `GetIEcoComponentFactoryPtr` export.
    """

    def load(self, path: Path) -> EcoLib:
        """Load a library and extract its component factory.

        Args:
            path: Path to the library file.

        Returns:
            Library descriptor with handle and factory.

        Raises:
            FileNotFoundError: If library file does not exist.
            EcoError: If factory extraction fails.
        """
        # Verify file exists
        path = path.resolve()
        if not path.exists():
            raise FileNotFoundError(f"Library not found: '{path}'")

        # Load library
        try:
            handle = CDLL(str(path))
        except OSError as err:
            raise EcoError(
                EcoErrorCode.FAIL,
                f"Failed to load library: '{path.name}': incompatible architecture",
            ) from err

        # Get factory pointer
        try:
            get_factory = handle.GetIEcoComponentFactoryPtr
            get_factory.restype = Ptr[Void]
            get_factory.argtypes = []
            factory_ptr = get_factory()
        except AttributeError as err:
            raise EcoError(
                EcoErrorCode.COMPONENT_NOTFOUND,
                f"Library does not export `GetIEcoComponentFactoryPtr`: '{path.name}'",
            ) from err

        return EcoLib(
            path=path,
            handle=handle,
            factory=IEcoComponentFactory(factory_ptr),
        )

    def load_by_cid(self, cid: UGUID, search_paths: list[Path]) -> EcoLib:
        """Load a library by component ID, searching in provided paths.

        Args:
            cid: Component ID (CID).
            search_paths: List of directories to search.

        Returns:
            Library descriptor with handle and factory.

        Raises:
            FileNotFoundError: If library not found in any search path.
        """
        filename = guid_to_lib_filename(cid)

        for search_path in search_paths:
            lib_path = search_path / filename
            if lib_path.exists():
                return self.load(lib_path)

        raise FileNotFoundError(
            f"Library not found for CID '{cid.to_string()}' in paths: {search_paths}"
        )
