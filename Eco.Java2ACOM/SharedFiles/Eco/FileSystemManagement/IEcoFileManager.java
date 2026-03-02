package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoFileManager extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x02});

    IEcoFile Create(String pszName);
    IEcoFile Open(String pszName);
    int Close(IEcoFile iFile);
}
