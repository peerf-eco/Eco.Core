package Eco.InterfaceBus;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;

@EcoNative
public interface IEcoInterfaceBus1FileExt extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xA0, 0x02, 0x00, 0x00, 0x01, 0x01});

    short set_Manager(UGUID rcid);
    UGUID get_Manager();
    short set_SearchPath(String path);
    String get_SearchPath();
    short RegisterComponent(UGUID rcid, String filename);
    short QueryComponent(String filename, UGUID rcid, IEcoUnknown iUnkOuter, UGUID riid, IEcoUnknownPtr pIUnk);
}
