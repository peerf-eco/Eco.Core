package Eco.MemoryManager;

import Eco.Core.IEcoUnknown_Client;

public class IEcoMemoryAllocator_Client extends IEcoUnknown_Client implements IEcoMemoryAllocator {
    @Override
    public long Alloc(long size) {
        return (Long) invokeNative(3, new Object[]{size}, long.class);
    }

    @Override
    public void Free(long pv) {
        invokeNative(4, new Object[]{pv}, void.class);
    }

    @Override
    public long Realloc(long pv, long size) {
        return (Long) invokeNative(5, new Object[]{pv, size}, long.class);
    }

    @Override
    public long Copy(long pvDst, long pvSrc, long size) {
        return (Long) invokeNative(6, new Object[]{pvDst, pvSrc, size}, long.class);
    }

    @Override
    public long Fill(long pvDst, char fill, long size) {
        return (Long) invokeNative(7, new Object[]{pvDst, fill, size}, long.class);
    }

    @Override
    public int Compare(long pv1, long pv2, long size) {
        return (Integer) invokeNative(8, new Object[]{pv1, pv2, size}, int.class);
    }

    @Override
    public int RetrieveSize(long pv) {
        return (Integer) invokeNative(9, new Object[]{pv}, long.class);
    }
}
