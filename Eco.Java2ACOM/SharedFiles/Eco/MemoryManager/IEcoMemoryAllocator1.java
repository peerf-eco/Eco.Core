package Eco.MemoryManager;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoMemoryAllocator1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xB0, 0x00, 0x00, 0x00, 0x01, 0x02});

    Object Alloc(int size);
    void Free(Object pv);
    Object Realloc(Object pv, int size);
    Object Copy(Object pvDst, Object pvSrc, int size);
    Object Fill(Object pvDst, char fill, int size);
    short Compare(Object pv1, Object pv2, int size);
    int RetrieveSize(Object pv);
}
