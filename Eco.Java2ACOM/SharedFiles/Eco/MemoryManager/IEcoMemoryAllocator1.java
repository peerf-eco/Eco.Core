package Eco.MemoryManager;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoMemoryAllocator1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xB0, 0x00, 0x00, 0x00, 0x01, 0x02});

    long Alloc(long size);
    void Free(long pv);
    long Realloc(long pv, long size);
    long Copy(long pvDst, long pvSrc, long size);
    long Fill(long pvDst, char fill, long size);
    int Compare(long pv1, long pv2, long size);
    int RetrieveSize(long pv);
}
