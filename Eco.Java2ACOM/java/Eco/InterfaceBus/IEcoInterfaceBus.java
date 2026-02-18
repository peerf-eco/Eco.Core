package Eco.InterfaceBus;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public interface IEcoInterfaceBus extends IEcoUnknown {
    UGUID CID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x75, 0x73, 0x31});
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xA0, 0x00, 0x00, 0x00, 0x01, 0x01});

    int Init();
    int InitWith(long heapStartAddress, long size);
    int RegisterComponent(UGUID rcid, IEcoUnknown iFactory);
    int UnRegisterComponent(UGUID rcid);
    int QueryComponent(UGUID rcid, IEcoUnknown iUnkOuter, UGUID riid, IEcoUnknown iUnk);
}
