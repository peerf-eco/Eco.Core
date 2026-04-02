package Eco.InterfaceBus;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

@EcoNative
public interface IEcoInterfaceBus1MemExt extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xA0, 0x01, 0x00, 0x00, 0x01, 0x01});

    int set_Manager(UGUID rcid);
    UGUID get_Manager();
    int set_ExpandPool(boolean bExpandPool);
}
