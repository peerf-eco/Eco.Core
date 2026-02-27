package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;
import Eco.Core.UGUID;

public class IEcoDriveInfo_Client extends IEcoUnknown_Client implements IEcoDriveInfo {
    @Override
    public UGUID get_Id() {
        return (UGUID) invokeNative(3, new Object[]{}, UGUID.class);
    }
}
