package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;

public class IEcoDriveManager_Client extends IEcoUnknown_Client implements IEcoDriveManager {
    @Override
    public IEcoDriveInfo get_Drive(String pszName) {
        return (IEcoDriveInfo) invokeNative(3, new Object[]{pszName}, IEcoDriveInfo.class);
    }
}
