package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoComponentFactory extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55});

    int Alloc(IEcoUnknown iSystem, IEcoUnknown iUnknownOuter, UGUID riid, IEcoUnknownPtr pIUnk);
    int Init(IEcoUnknown iSystem, IEcoUnknown iUnk);
    String get_Name();
    String get_Version();
    String get_Manufacturer();
}
