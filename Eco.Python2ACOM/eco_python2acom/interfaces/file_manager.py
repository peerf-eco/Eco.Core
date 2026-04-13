"""File system management interfaces for EcoOS.

The `FileSystemManagement` component provides file system operations.

Interfaces:
    IEcoFileSystemManagement1: Top-level manager providing sub-managers.
    IEcoFileManager1: File operations (create, open, close).
    IEcoDirectoryManager1: Directory operations.
    IEcoDriveManager1: Drive/volume operations.

Reference:
    Based on `IEcoFileSystemManagement1.h` and related headers
    from `Eco.FileSystemManagement1/SharedFiles`.
"""

from __future__ import annotations

from eco_python2acom.decorators.interface import interface
from eco_python2acom.guids.iid import (
    IID_IEcoDirectoryInfo1,
    IID_IEcoDirectoryManager1,
    IID_IEcoDriveInfo1,
    IID_IEcoDriveManager1,
    IID_IEcoFile1,
    IID_IEcoFileInfo1,
    IID_IEcoFileManager1,
    IID_IEcoFileSystemManagement1,
)
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.types.core import CString, Int16, Int32, UInt32, Void
from eco_python2acom.types.guid import UGUID
from eco_python2acom.types.pointer import Ptr

# =============================================================================
# IEcoFileInfo1
# =============================================================================


@interface(iid=IID_IEcoFileInfo1)
class IEcoFileInfo1(IEcoUnknown):
    """File information interface.

    Provides methods to retrieve file metadata such as file ID.
    """

    def get_id(self) -> Ptr[UGUID]:
        """Get the file identifier.

        Returns:
            Pointer to UGUID representing the file ID.
        """
        ...


# =============================================================================
# IEcoFile1
# =============================================================================


@interface(iid=IID_IEcoFile1)
class IEcoFile1(IEcoUnknown):
    """File interface for file operations.

    Provides methods to read, write, seek, and get file information.
    """

    def get_size(self) -> Int32:
        """Get the size of the file.

        Returns:
            Size of the file in bytes.
        """
        ...

    def get_name(self) -> CString:
        """Get the name of the file.

        Returns:
            File name string.
        """
        ...

    def get_info(self) -> Ptr[IEcoFileInfo1]:
        """Get the file information interface.

        Returns:
            Pointer to `IEcoFileInfo1` for retrieving file metadata.
        """
        ...

    def get_pointer(self) -> UInt32:
        """Get the current file pointer position.

        Returns:
            Current position of the file pointer.
        """
        ...

    def set_pointer(self, position: UInt32) -> Void:
        """Set the file pointer to a specific position.

        Args:
            position: New position for the file pointer.
        """
        ...

    def read(self, buffer: Ptr[Void], size: Ptr[UInt32]) -> Int16:
        """Read data from the file into a buffer.

        Args:
            buffer: Pointer to a buffer where data will be read into.
            size: Pointer to a variable that specifies the number of bytes to read.
                  On return, it contains the actual number of bytes read.
        Returns:
            0 on success, error code otherwise.
        """
        ...

    def write(self, buffer: Ptr[Void], size: Ptr[UInt32]) -> Int16:
        """Write data from a buffer to the file.

        Args:
            buffer: Pointer to a buffer containing data to be written.
            size: Pointer to a variable that specifies the number of bytes to write.
                  On return, it contains the actual number of bytes written.
        Returns:
            0 on success, error code otherwise.
        """
        ...

    def close(self) -> Int16:
        """Close the file.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    def get_descriptor(self) -> Int32:
        """Get the file descriptor.

        Returns:
            File descriptor as an integer.
        """
        ...


# =============================================================================
# IEcoDriveInfo1
# =============================================================================


@interface(iid=IID_IEcoDriveInfo1)
class IEcoDriveInfo1(IEcoUnknown):
    """Drive information interface.

    Provides methods to retrieve drive metadata such as drive ID.
    """

    def get_id(self) -> Ptr[UGUID]:
        """Get the drive identifier.

        Returns:
            Pointer to UGUID representing the drive ID.
        """
        ...


# =============================================================================
# IEcoDriveManager1
# =============================================================================


@interface(iid=IID_IEcoDriveManager1)
class IEcoDriveManager1(IEcoUnknown):
    """Drive manager interface for drive operations.

    Provides methods to retrieve drive information by name.
    """

    def get_drive(self, name: CString) -> Ptr[IEcoDriveInfo1]:
        """Get a drive by its name.

        Args:
            name: Drive name string.

        Returns:
            Pointer to `IEcoDriveInfo1` for the specified drive.
        """
        ...


# =============================================================================
# IEcoDirectoryInfo1
# =============================================================================


@interface(iid=IID_IEcoDirectoryInfo1)
class IEcoDirectoryInfo1(IEcoUnknown):
    """Directory information interface.

    Provides methods to retrieve directory metadata such as directory ID.
    """

    def get_id(self) -> Ptr[UGUID]:
        """Get the directory identifier.

        Returns:
            Pointer to UGUID representing the directory ID.
        """
        ...


# =============================================================================
# IEcoDirectoryManager1
# =============================================================================


@interface(iid=IID_IEcoDirectoryManager1)
class IEcoDirectoryManager1(IEcoUnknown):
    """Directory manager interface for directory operations.

    Provides methods to create and delete directories.
    """

    def create(self, name: CString) -> Ptr[IEcoDirectoryInfo1]:
        """Create a new directory.

        Args:
            name: Directory name string.

        Returns:
            Pointer to `IEcoDirectoryInfo1` for the created directory.
        """
        ...

    def delete(self, name: CString) -> Int16:
        """Delete an existing directory.

        Args:
            name: Directory name string.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoFileManager1
# =============================================================================


@interface(iid=IID_IEcoFileManager1)
class IEcoFileManager1(IEcoUnknown):
    """File manager interface for file operations.

    Provides methods to create, open, and close files.
    """

    def create(self, name: CString) -> Ptr[IEcoFile1]:
        """Create a new file.

        Args:
            name: File name string.

        Returns:
            Pointer to `IEcoFile1`.
        """
        ...

    def open(self, name: CString) -> Ptr[IEcoFile1]:
        """Open an existing file.

        Args:
            name: File name string.

        Returns:
            Pointer to `IEcoFile1`.
        """
        ...

    def close(self, file: Ptr[IEcoFile1]) -> Int16:
        """Close a file.

        Args:
            file: Pointer to `IEcoFile1`.

        Returns:
            0 on success, error code otherwise.
        """
        ...


# =============================================================================
# IEcoFileSystemManagement1
# =============================================================================


@interface(iid=IID_IEcoFileSystemManagement1)
class IEcoFileSystemManagement1(IEcoUnknown):
    """File system management interface.

    Provides access to file, directory, and drive managers.
    """

    def get_file_manager(self) -> Ptr[IEcoFileManager1]:
        """Get the file manager.

        Returns:
            Pointer to `IEcoFileManager1`.
        """
        ...

    def get_directory_manager(self) -> Ptr[IEcoDirectoryManager1]:
        """Get the directory manager.

        Returns:
            Pointer to `IEcoDirectoryManager1`.
        """
        ...

    def get_drive_manager(self) -> Ptr[IEcoDriveManager1]:
        """Get the drive manager.

        Returns:
            Pointer to `IEcoDriveManager1`.
        """
        ...
