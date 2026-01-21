package Eco.MemoryManager;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoMemoryAllocator extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xB0, 0x00, 0x00, 0x00, 0x01, 0x02});

    public native long Alloc(long size);
    public native void Free(long pv);
    public native long Realloc(long pv, long size);
    public native long Copy(long pvDst, long pvSrc, long size);
    public native long Fill(long pvDst, char fill, long size);
    public native int Compare(long pv1, long pv2, long size);
    public native int RetrieveSize(long pv);
}
