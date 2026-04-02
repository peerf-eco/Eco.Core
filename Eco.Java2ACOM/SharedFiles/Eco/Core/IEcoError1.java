package Eco.Core;

import Eco.Annotations.EcoNative;

@EcoNative
public interface IEcoError1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xFF, (byte) 0xFE});

    void set_ErrorMode(int errMode);
    int get_ErrorMode();
    void set_LastError(int errCode);
    int get_LastError();
    int get_Description(int errCode, StringPtr message, IntegerPtr iSize);
}
