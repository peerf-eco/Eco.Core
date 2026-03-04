package Eco.MemoryManager;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoMemoryManager extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xB0, 0x00, 0x00, 0x00, 0x01, 0x01});

    int Init(long startAddress, long size);
    boolean get_Status(EcoMemoryManagerStatus status);
    boolean get_UsedBlocks(EcoMemoryManagerBlock[] blocks, long[] sizeInBlocks);
}
