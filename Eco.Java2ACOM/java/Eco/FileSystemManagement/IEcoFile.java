package Eco.FileSystemManagement;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoFile extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x01, 0x02});

    public native long get_Size();
    public native String get_Name();
    public native IEcoFileInfo get_Info();
    public native long get_Pointer();
    public native void set_Pointer(long value);
    public native int Read(long pv, Long pSize);
    public native int Write(long pv, Long pSize);
    public native int Close();
    public native long get_Descriptor();
}
