package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoConnectionPoint extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46});

    int GetConnectionInterface(UGUID IID);
    int GetConnectionPointContainer(IEcoUnknownPtr pCPC);
    int Advise(IEcoUnknown unkSink, IntegerPtr cCookie);
    int Unadvise(int cCookie);
    int EnumConnections(IEcoUnknownPtr pEnum);
}
