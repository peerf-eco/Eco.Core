package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;

public class IEcoDirectoryManager_Client extends IEcoUnknown_Client implements IEcoDirectoryManager {
    @Override
    public IEcoDirectoryInfo Create(String pszName) {
        return (IEcoDirectoryInfo) invokeNative(3, new Object[]{pszName}, IEcoDirectoryInfo.class);
    }

    @Override
    public int Delete(String pszName) {
        return (Integer) invokeNative(4, new Object[]{pszName}, int.class);
    }
}
