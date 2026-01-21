package Eco.Calculator;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoCalculatorX extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42, (byte) 0xAE, 0x06, (byte) 0x82, (byte) 0x81, (byte) 0x94, 0x47, (byte) 0x84, 0x3D});

    public native int Addition(int a, int b);
    public native int Subtraction(int a, int b);
}
