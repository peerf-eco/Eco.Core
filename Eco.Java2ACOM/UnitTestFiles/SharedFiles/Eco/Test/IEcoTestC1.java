package Eco.Test;

import Eco.Annotations.EcoNative;
import Eco.Core.IEcoUnknown;
import Eco.Core.UGUID;
import Eco.Core.Pointer;

@EcoNative
public interface IEcoTestC1 extends IEcoUnknown {
    UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0xA7, (byte) 0x88, (byte) 0xE7, (byte) 0xF1, (byte) 0xD0, 0x58, 0x48, 0x70, (byte) 0xA1, 0x51, 0x1A, (byte) 0x9A, 0x75, (byte) 0xEF, (byte) 0xC8, 0x21});

    byte TestInt8(byte in, Pointer<Byte> inOut, Pointer<Byte> out);
    short TestInt16(short in, Pointer<Short> inOut, Pointer<Short> out);
    int TestInt32(int in, Pointer<Integer> inOut, Pointer<Integer> out);
    long TestInt64(long in, Pointer<Long> inOut, Pointer<Long> out);

    byte TestUInt8(byte in, Pointer<Byte> inOut, Pointer<Byte> out);
    short TestUInt16(short in, Pointer<Short> inOut, Pointer<Short> out);
    int TestUInt32(int in, Pointer<Integer> inOut, Pointer<Integer> out);
    long TestUInt64(long in, Pointer<Long> inOut, Pointer<Long> out);

    float TestFloat(float in, Pointer<Float> inOut, Pointer<Float> out);
    double TestDouble(double in, Pointer<Double> inOut, Pointer<Double> out);

    boolean TestBoolean(boolean in, Pointer<Boolean> inOut, Pointer<Boolean> out);
    char TestChar(char in, Pointer<Character> inOut, Pointer<Character> out);
    char TestWChar(char in, Pointer<Character> inOut, Pointer<Character> out);

    String TestAString(String in, Pointer<String> inOut, Pointer<String> out);
    String TestWString(String in, Pointer<String> inOut, Pointer<String> out);

    UGUID TestUGUID(UGUID in, Pointer<UGUID> inOut, Pointer<UGUID> out);
    IEcoUnknown TestInterface(IEcoUnknown in, Pointer<IEcoUnknown> inOut, Pointer<IEcoUnknown> out);
    Object TestVoidPtr(Object in, Pointer<Object> inOut, Pointer<Object> out);

    void TestVoid(int in);
    int GetLastVoidInput();
}
