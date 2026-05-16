package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoFile1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x02});

    int get_Size();
    String get_Name();
    IEcoFileInfo1 get_Info();
    int get_Pointer();
    void set_Pointer(int value);
    short Read(Object pv, Integer pSize);
    short Write(Object pv, Integer pSize);
    short Close();
    Object get_Descriptor();
}
