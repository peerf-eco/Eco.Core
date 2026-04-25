package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoDirectoryManager1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x07});

    IEcoDirectoryInfo1 Create(String pszName);
    int Delete(String pszName);
}
