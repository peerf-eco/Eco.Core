package Eco.DateTime;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

@EcoNative
public interface IEcoDateTime1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0xCA, 0x5A, 0x4E, 0x0E, 0x7E, (byte) 0xEB, 0x4C, (byte) 0xA9, (byte) 0x92, (byte) 0xC6, 0x22, 0x04, 0x3F, (byte) 0xFA, 0x07, (byte) 0xBC});

    IEcoDateTime1 Now();
    IEcoDateTime1 Clone();
    ECOTIMEVAL get_SystemTime();
    void set_SystemTime(ECOTIMEVAL value);
    int get_TimeOfDaySec();
    int get_TimeOfDayUSec();
    String ToString();
    String ToStringFormat(String format);
}
