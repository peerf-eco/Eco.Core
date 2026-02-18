package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown_Client;
import Eco.Core.UGUID;

public class IEcoFileInfo_Client extends IEcoUnknown_Client implements IEcoFileInfo {
    @Override
    public UGUID get_Id() {
        return (UGUID) invokeNative(3, new Object[]{}, UGUID.class);
    }
}
