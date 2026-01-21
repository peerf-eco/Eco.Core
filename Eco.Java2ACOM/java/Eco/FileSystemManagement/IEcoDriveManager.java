package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoDriveManager extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x05});

    public native IEcoDriveInfo get_Drive(String pszName);
}
