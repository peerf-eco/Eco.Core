package Eco.MemoryManager;

import Eco.Core.IEcoUnknown_Client;

public class IEcoMemoryManager_Client extends IEcoUnknown_Client implements IEcoMemoryManager {
    @Override
    public int Init(long startAddress, long size) {
        return (Integer) invokeNative(3, new Object[]{startAddress, size}, int.class);
    }

    @Override
    public boolean get_Status(EcoMemoryManagerStatus status) {
        return (Boolean) invokeNative(4, new Object[]{status}, boolean.class);
    }

    @Override
    public boolean get_UsedBlocks(EcoMemoryManagerBlock[] blocks, long[] sizeInBlocks) {
        return (Boolean) invokeNative(5, new Object[]{blocks, sizeInBlocks}, boolean.class);
    }
}
