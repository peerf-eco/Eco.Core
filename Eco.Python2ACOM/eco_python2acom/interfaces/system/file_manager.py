"""File System Management interfaces for EcoOS.

The FileSystemManagement component provides file system operations.

Interfaces:
    IEcoFileSystemManagement1: Top-level manager providing sub-managers.
    IEcoFileManager1: File operations (create, open, close).
    IEcoDirectoryManager1: Directory operations.
    IEcoDriveManager1: Drive/volume operations.

Reference:
    Based on IEcoFileSystemManagement1.h and related headers
    from Eco.FileSystemManagement1/SharedFiles.
"""

from __future__ import annotations

from ctypes import _Pointer
from typing import TYPE_CHECKING

from eco_python2acom.core.guid import UGUIDPtr
from eco_python2acom.core.types import CharPtr, EcoResult, Int32, UInt32, UInt32Ptr, Void, VoidPtr
from eco_python2acom.interfaces.base import IEcoUnknown
from eco_python2acom.interfaces.decorators import interface, method
from eco_python2acom.interfaces.guids.iid import (
    IID_IEcoDirectoryInfo1,
    IID_IEcoDirectoryManager1,
    IID_IEcoDriveInfo1,
    IID_IEcoDriveManager1,
    IID_IEcoFile1,
    IID_IEcoFileInfo1,
    IID_IEcoFileManager1,
    IID_IEcoFileSystemManagement1,
)

# =============================================================================
# IEcoFileInfo1
# =============================================================================


@interface(iid=IID_IEcoFileInfo1)
class IEcoFileInfo1(IEcoUnknown):
    """File information interface.

    Provides methods to retrieve file metadata such as file ID.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_Id(self) -> UGUIDPtr:
        """Get the file identifier.

        Returns:
            Pointer to UGUID representing the file ID.
        """
        ...


if TYPE_CHECKING:
    IEcoFileInfo1Ptr = _Pointer[IEcoFileInfo1]
else:
    IEcoFileInfo1Ptr = IEcoFileInfo1._interface_ptr_

# =============================================================================
# IEcoFile1
# =============================================================================


@interface(iid=IID_IEcoFile1)
class IEcoFile1(IEcoUnknown):
    """File interface for file operations.

    Provides methods to read, write, seek, and get file information.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_Size(self) -> Int32:
        """Get the size of the file.

        Returns:
            Size of the file in bytes.
        """
        ...

    @method
    def get_Name(self) -> CharPtr:
        """Get the name of the file.

        Returns:
            File name string.
        """
        ...

    @method
    def get_Info(self) -> IEcoFileInfo1Ptr:
        """Get the file information interface.

        Returns:
            Pointer to IEcoFileInfo1 for retrieving file metadata.
        """
        ...

    @method
    def get_Pointer(self) -> UInt32:
        """Get the current file pointer position.

        Returns:
            Current position of the file pointer.
        """
        ...

    @method
    def set_Pointer(self, position: UInt32) -> Void:
        """Set the file pointer to a specific position.

        Args:
            position: New position for the file pointer.
        """
        ...

    @method
    def Read(self, buffer: VoidPtr, size: UInt32Ptr) -> EcoResult:
        """Read data from the file into a buffer.

        Args:
            buffer: Pointer to a buffer where data will be read into.
            size: Pointer to a variable that specifies the number of bytes to read.
                  On return, it contains the actual number of bytes read.
        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def Write(self, buffer: VoidPtr, size: UInt32Ptr) -> EcoResult:
        """Write data from a buffer to the file.

        Args:
            buffer: Pointer to a buffer containing data to be written.
            size: Pointer to a variable that specifies the number of bytes to write.
                  On return, it contains the actual number of bytes written.
        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def Close(self) -> EcoResult:
        """Close the file.

        Returns:
            0 on success, error code otherwise.
        """
        ...

    @method
    def get_Descriptor(self) -> Int32:
        """Get the file descriptor.

        Returns:
            File descriptor as an integer.
        """
        ...


if TYPE_CHECKING:
    IEcoFile1Ptr = _Pointer[IEcoFile1]
else:
    IEcoFile1Ptr = IEcoFile1._interface_ptr_

# =============================================================================
# IEcoDriveInfo1
# =============================================================================


@interface(iid=IID_IEcoDriveInfo1)
class IEcoDriveInfo1(IEcoUnknown):
    """Drive information interface.

    Provides methods to retrieve drive metadata such as drive ID.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_Id(self) -> UGUIDPtr:
        """Get the drive identifier.

        Returns:
            Pointer to UGUID representing the drive ID.
        """
        ...


if TYPE_CHECKING:
    IEcoDriveInfo1Ptr = _Pointer[IEcoDriveInfo1]
else:
    IEcoDriveInfo1Ptr = IEcoDriveInfo1._interface_ptr_

# =============================================================================
# IEcoDriveManager1
# =============================================================================


@interface(iid=IID_IEcoDriveManager1)
class IEcoDriveManager1(IEcoUnknown):
    """Drive manager interface for drive operations.

    Provides methods to retrieve drive information by name.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_Drive(self, name: CharPtr) -> IEcoDriveInfo1Ptr:
        """Get a drive by its name.

        Args:
            name: Drive name string.

        Returns:
            Pointer to IEcoDriveInfo1 for the specified drive.
        """
        ...


if TYPE_CHECKING:
    IEcoDriveManager1Ptr = _Pointer[IEcoDriveManager1]
else:
    IEcoDriveManager1Ptr = IEcoDriveManager1._interface_ptr_

# =============================================================================
# IEcoDirectoryInfo1
# =============================================================================


@interface(iid=IID_IEcoDirectoryInfo1)
class IEcoDirectoryInfo1(IEcoUnknown):
    """Directory information interface.

    Provides methods to retrieve directory metadata such as directory ID.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_Id(self) -> UGUIDPtr:
        """Get the directory identifier.

        Returns:
            Pointer to UGUID representing the directory ID.
        """
        ...


if TYPE_CHECKING:
    IEcoDirectoryInfo1Ptr = _Pointer[IEcoDirectoryInfo1]
else:
    IEcoDirectoryInfo1Ptr = IEcoDirectoryInfo1._interface_ptr_

# =============================================================================
# IEcoDirectoryManager1
# =============================================================================


@interface(iid=IID_IEcoDirectoryManager1)
class IEcoDirectoryManager1(IEcoUnknown):
    """Directory manager interface for directory operations.

    Provides methods to create and delete directories.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Create(self, name: CharPtr) -> IEcoDirectoryInfo1Ptr:
        """Create a new directory.

        Args:
            name: Directory name string.

        Returns:
            Pointer to IEcoDirectoryInfo1 for the created directory.
        """
        ...

    @method
    def Delete(self, name: CharPtr) -> EcoResult:
        """Delete an existing directory.

        Args:
            name: Directory name string.

        Returns:
            0 on success, error code otherwise.
        """
        ...


if TYPE_CHECKING:
    IEcoDirectoryManager1Ptr = _Pointer[IEcoDirectoryManager1]
else:
    IEcoDirectoryManager1Ptr = IEcoDirectoryManager1._interface_ptr_

# =============================================================================
# IEcoFileManager1
# =============================================================================


@interface(iid=IID_IEcoFileManager1)
class IEcoFileManager1(IEcoUnknown):
    """File manager interface for file operations.

    Provides methods to create, open, and close files.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def Create(self, name: CharPtr) -> IEcoFile1Ptr:
        """Create a new file.

        Args:
            name: File name string.

        Returns:
            Pointer to IEcoFile1.
        """
        ...

    @method
    def Open(self, name: CharPtr) -> IEcoFile1Ptr:
        """Open an existing file.

        Args:
            name: File name string.

        Returns:
            Pointer to IEcoFile1.
        """
        ...

    @method
    def Close(self, pFile: IEcoFile1Ptr) -> EcoResult:
        """Close a file.

        Args:
            pFile: Pointer to IEcoFile1.

        Returns:
            0 on success, error code otherwise.
        """
        ...


if TYPE_CHECKING:
    IEcoFileManager1Ptr = _Pointer[IEcoFileManager1]
else:
    IEcoFileManager1Ptr = IEcoFileManager1._interface_ptr_

# =============================================================================
# IEcoFileSystemManagement1
# =============================================================================


@interface(iid=IID_IEcoFileSystemManagement1)
class IEcoFileSystemManagement1(IEcoUnknown):
    """File system management interface.

    Provides access to file, directory, and drive managers.

    Inherits:
        IEcoUnknown: QueryInterface, AddRef, Release
    """

    @method
    def get_FileManager(self) -> IEcoFileManager1Ptr:
        """Get the file manager.

        Returns:
            Pointer to IEcoFileManager1.
        """
        ...

    @method
    def get_DirectoryManager(self) -> IEcoDirectoryManager1Ptr:
        """Get the directory manager.

        Returns:
            Pointer to IEcoDirectoryManager1.
        """
        ...

    @method
    def get_DriveManager(self) -> IEcoDriveManager1Ptr:
        """Get the drive manager.

        Returns:
            Pointer to IEcoDriveManager1.
        """
        ...


if TYPE_CHECKING:
    IEcoFileSystemManagement1Ptr = _Pointer[IEcoFileSystemManagement1]
else:
    IEcoFileSystemManagement1Ptr = IEcoFileSystemManagement1._interface_ptr_
