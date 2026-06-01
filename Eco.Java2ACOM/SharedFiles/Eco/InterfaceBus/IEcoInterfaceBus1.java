package Eco.InterfaceBus;

import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;
import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoInterfaceBus1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xA0, 0x00, 0x00, 0x00, 0x01, 0x01});

    short Init();
    short InitWith(Object heapStartAddress, int size);
    short RegisterComponent(UGUID rcid, IEcoUnknown iFactory);
    short UnRegisterComponent(UGUID rcid);
    short QueryComponent(UGUID rcid, IEcoUnknown iUnkOuter, UGUID riid, IEcoUnknownPtr pIUnk);
}
