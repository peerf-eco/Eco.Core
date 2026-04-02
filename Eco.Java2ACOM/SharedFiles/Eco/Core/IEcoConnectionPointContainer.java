package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoConnectionPointContainer extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, (byte) 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46});

    int EnumConnectionPoints(IEcoUnknownPtr pEnum);
    int FindConnectionPoint(UGUID riid, IEcoUnknownPtr pCP);
}
