package Eco.Calculator;

import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;

public class IEcoCalculatorY extends IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0xBD, 0x64, 0x14, (byte) 0xC2, (byte) 0x90, (byte) 0x96, 0x42, 0x3E, (byte) 0xA9, 0x0C, 0x04, (byte) 0xD7, 0x7A, (byte) 0xFD, 0x1C, (byte) 0xAD});

    public int Multiplication(int a, int b) {
        return (Integer) invokeNative(3, new Object[]{a, b}, int.class);
    }

    public int Division(int a, int b) {
        return (Integer) invokeNative(4, new Object[]{a, b}, int.class);
    }
}
