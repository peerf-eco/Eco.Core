package Eco.InterfaceBus;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoInterfaceBus extends IEcoUnknown {
    public static UGUID CID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x75, 0x73, 0x31});
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xA0, 0x00, 0x00, 0x00, 0x01, 0x01});

    public native int Init();
    public native int InitWith(long heapStartAddress, long size);
    public native int RegisterComponent(UGUID rcid, IEcoUnknown iFactory);
    public native int UnRegisterComponent(UGUID rcid);
    public native int QueryComponent(UGUID rcid, IEcoUnknown iUnkOuter, UGUID riid, IEcoUnknown iUnk);
}
