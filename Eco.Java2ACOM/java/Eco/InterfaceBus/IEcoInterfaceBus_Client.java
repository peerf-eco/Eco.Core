package Eco.InterfaceBus;

import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknown_Client;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;

public class IEcoInterfaceBus_Client extends IEcoUnknown_Client implements IEcoInterfaceBus {
    @Override
    public int Init() {
        return (Integer) invokeNative(3, new Object[]{}, int.class);
    }

    @Override
    public int InitWith(long heapStartAddress, long size) {
        return (Integer) invokeNative(4, new Object[]{heapStartAddress, size}, int.class);
    }

    @Override
    public int RegisterComponent(UGUID rcid, IEcoUnknown iFactory) {
        return (Integer) invokeNative(5, new Object[]{rcid, iFactory}, int.class);
    }

    @Override
    public int UnRegisterComponent(UGUID rcid) {
        return (Integer) invokeNative(6, new Object[]{rcid}, int.class);
    }

    @Override
    public int QueryComponent(UGUID rcid, IEcoUnknown iUnkOuter, UGUID riid, IEcoUnknownPtr pIUnk) {
        return (Integer) invokeNative(7, new Object[]{rcid, iUnkOuter, riid, pIUnk}, int.class);
    }
}
