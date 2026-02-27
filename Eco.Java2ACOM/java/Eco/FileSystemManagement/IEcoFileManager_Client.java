package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;

public class IEcoFileManager_Client extends IEcoUnknown_Client implements IEcoFileManager {
    @Override
    public IEcoFile Create(String pszName) {
        return (IEcoFile) invokeNative(3, new Object[]{pszName}, IEcoFile.class);
    }

    @Override
    public IEcoFile Open(String pszName) {
        return (IEcoFile) invokeNative(4, new Object[]{pszName}, IEcoFile.class);
    }

    @Override
    public int Close(IEcoFile iFile) {
        return (Integer) invokeNative(5, new Object[]{iFile}, int.class);
    }
}
