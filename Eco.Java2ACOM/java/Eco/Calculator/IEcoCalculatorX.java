package Eco.Calculator;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoCalculatorX extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42, (byte) 0xAE, 0x06, (byte) 0x82, (byte) 0x81, (byte) 0x94, 0x47, (byte) 0x84, 0x3D});

    public int Addition(int a, int b) {
        return (Integer) invokeNative(3, new Object[]{a, b}, int.class);
    }

    public int Subtraction(int a, int b) {
        return (Integer) invokeNative(4, new Object[]{a, b}, int.class);
    }
}
