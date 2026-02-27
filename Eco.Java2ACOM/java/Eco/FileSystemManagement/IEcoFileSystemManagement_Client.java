package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;

public class IEcoFileSystemManagement_Client extends IEcoUnknown_Client implements IEcoFileSystemManagement {
    @Override
    public IEcoFileManager get_FileManager() {
        return (IEcoFileManager) invokeNative(3, new Object[]{}, IEcoFileManager.class);
    }

    @Override
    public IEcoDirectoryManager get_DirectoryManager() {
        return (IEcoDirectoryManager) invokeNative(4, new Object[]{}, IEcoDirectoryManager.class);
    }

    @Override
    public IEcoDriveManager get_DriveManager1() {
        return (IEcoDriveManager) invokeNative(5, new Object[]{}, IEcoDriveManager.class);
    }
}
