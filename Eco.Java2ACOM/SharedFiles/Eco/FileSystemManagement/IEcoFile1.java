package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoFile1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x02});

    long get_Size();
    String get_Name();
    IEcoFileInfo1 get_Info();
    long get_Pointer();
    void set_Pointer(long value);
    int Read(long pv, Long pSize);
    int Write(long pv, Long pSize);
    int Close();
    long get_Descriptor();
}
