"""Library loader for EcoOS components.

This module provides functionality to load libraries and extract
component factories.

Classes:
    `EcoLib`: Information about a loaded EcoOS library.
    `EcoLibLoader`: Loader for EcoOS component libraries.
"""

import logging
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

from eco_python2acom.interfaces.factory import IEcoComponentFactory
from eco_python2acom.runtime.logging import eco_logger
from eco_python2acom.runtime.utils import guid_to_lib_filename
from eco_python2acom.types.core import CDLL, Void
from eco_python2acom.types.errors import EcoError, EcoErrorCode
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr
from eco_python2acom.types.utils import cast


@dataclass
class EcoLib:
    """Information about a loaded EcoOS library.

    Attributes:
        path: Full path to the library file.
        handle: Library handle.
        factory: Pointer to the component factory interface.
    """

    path: Path
    handle: CDLL
    factory: Ptr[IEcoComponentFactory]


class EcoLibLoader:
    """Loader for EcoOS component libraries.

    Handles loading shared libraries and extracting component factories
    via the `GetIEcoComponentFactoryPtr` export.

    Attributes:
        logger: Logger used to report load operations.
    """

    def __init__(self, logger: Optional[logging.Logger] = None) -> None:
        """Initialize the loader.

        Args:
            logger: Optional logger to use. Defaults to the package-wide
                `loader` child logger.
        """
        self.logger = logger if logger is not None else eco_logger.getChild(type(self).__name__)

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
            self.logger.error("Not found ---> '%s'", path)
            raise FileNotFoundError(f"Library not found: '{path}'")

        # Load library
        try:
            handle = CDLL(str(path))
        except OSError as err:
            self.logger.error("'%s' ---> Incompatible architecture", path.name)
            raise EcoError(
                EcoErrorCode.FAIL,
                f"Failed to load library: '{path.name}': incompatible architecture",
            ) from err
        self.logger.debug("'%s' ---> Loaded", path.name)

        # Get factory pointer
        try:
            get_factory = handle.GetIEcoComponentFactoryPtr
            get_factory.restype = Ptr[Void]
            get_factory.argtypes = []
            factory_ptr: Ptr[Void] = get_factory()
        except AttributeError as err:
            self.logger.error(
                "'%s' ---> Missing export 'GetIEcoComponentFactoryPtr'",
                path.name,
            )
            raise EcoError(
                EcoErrorCode.COMPONENT_NOTFOUND,
                f"Library does not export `GetIEcoComponentFactoryPtr`: '{path.name}'",
            ) from err

        self.logger.debug("'%s' ---> Factory at <0x%x>", path.name, factory_ptr.value or 0)
        return EcoLib(
            path=path,
            handle=handle,
            factory=cast(factory_ptr, Ptr[IEcoComponentFactory]),
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
        self.logger.debug("CID = <%s> ---> Filename = '%s'", cid, filename)

        for search_path in search_paths:
            lib_path = search_path / filename
            if lib_path.exists():
                return self.load(lib_path)

        self.logger.error("CID = <%s> ---> Not found in any search path", cid)
        raise FileNotFoundError(f"Library not found for CID '{cid}' in paths: {search_paths}")
