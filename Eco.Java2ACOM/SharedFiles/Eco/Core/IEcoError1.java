package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoError1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xFF, (byte) 0xFE});

    void set_ErrorMode(short errMode);
    short get_ErrorMode();
    void set_LastError(short errCode);
    short get_LastError();
    short get_Description(short errCode, Pointer<String> message, Pointer<Short> iSize);
}
