import Eco.Core.*;
import Eco.InterfaceBus.*;
import Eco.Calculator.*;
import Eco.List.*;
import Eco.Test.*;
import Eco.System.EcoApp;

public class EcoTest implements EcoApp {
    private static IEcoSystem1Native iSys;
    private static IEcoInterfaceBus1Native iBus;
    private static IEcoCalculatorX iCalcX;
    private static IEcoCalculatorY iCalcY;
    private static IEcoList1 iList;
    private static IEcoTest1 iTest;

    @Override
    public void EcoMain(IEcoUnknown iUnk) {
        short result = Main(iUnk);
        Release();
        System.out.println("\nResult Code: " + result);
    }

    private short Main(IEcoUnknown iUnk) {
        short result = Init(iUnk);
        if (result != 0) return result;
        result = TestCalculator();
        if (result != 0) return result;
        result = TestList();
        if (result != 0) return result;
        result = TestComponent();
        if (result != 0) return result;
        return result;
    }

    private short Init(IEcoUnknown iUnk) {
        System.out.println("\n==== Init ====");

        iSys = new IEcoSystem1Native();
        iBus = new IEcoInterfaceBus1Native();

        System.out.print("IEcoSystem1 QueryInterface");
        short result = iUnk.QueryInterface(IEcoSystem1.GID_x86_32, new Pointer<>(iSys));
        if (result != 0) {
            System.out.println(": Fail");
            return result;
        }
        System.out.println(": OK");

        System.out.print("IEcoInterfaceBus1 QueryInterface");
        result = iSys.QueryInterface(IEcoInterfaceBus1.IID, new Pointer<>(iBus));
        if (result != 0) {
            System.out.println(": Fail");
            return result;
        }
        System.out.println(": OK");

        return result;
    }

    private short TestCalculator() {
        System.out.println("\n==== Test Calculator ====");

        iCalcX = new IEcoCalculatorXNative();
        iCalcY = new IEcoCalculatorYNative();

        System.out.print("IEcoCalculatorX QueryComponent");
        short result = iBus.QueryComponent(IdEcoCalculatorC.CID, new IEcoUnknownNative(), IEcoCalculatorX.IID, new Pointer<>(iCalcX));
        if (result != 0) {
            System.out.println(": Fail");
            return result;
        }
        System.out.println(": OK");

        System.out.print("IEcoCalculatorY QueryInterface");
        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, new Pointer<>(iCalcY));
        if (result != 0) {
            System.out.println(": Fail");
            return result;
        }
        System.out.println("I: OK");

        int resultAddition = iCalcX.Addition((short) 9, (short) 10);
        System.out.print("Addition: 9 + 10 = " + resultAddition);
        if (resultAddition == 19) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        short resultSubtraction = iCalcX.Subtraction((short) 67, (short) 13);
        System.out.print("Subtraction: 67 - 13 = " + resultSubtraction);
        if (resultSubtraction == 54) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        int resultMultiplication = iCalcY.Multiplication((short) 6, (short) 8);
        System.out.print("Multiplication: 6 * 8 = " + resultMultiplication);
        if (resultMultiplication == 48) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        int resultDivision = iCalcY.Division((short) 42, (short) 7);
        System.out.print("Division: 42 / 7 = " + resultDivision);
        if (resultDivision == 6) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        return result;
    }

    private short TestList() {
        System.out.println("\n==== Test List ====");

        iList = new IEcoList1Native();

        System.out.print("IEcoList1 QueryComponent");
        short result = iBus.QueryComponent(IdEcoList1.CID, new IEcoUnknownNative(), IEcoList1.IID, new Pointer<>(iList));
        if (result != 0) {
            System.out.println(": Fail");
            return result;
        }
        System.out.println(": OK");

        Object obj1 = "OK1";
        Object obj2 = "OK2";
        Object obj3 = "OK3";

        int index = iList.Add(obj1);
        System.out.print("Add " + obj1 + ", index = " + index);
        if (index == 0) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        index = iList.Add(obj3);
        System.out.print("Add " + obj3 + ", index = " + index);
        if (index == 1) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        index = iList.Add(obj2);
        System.out.print("Add " + obj2 + ", index = " + index);
        if (index == 2) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        Object obj = iList.Item(0);
        System.out.print("Item 0 = " + obj);
        if (obj == "OK1") {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        obj = iList.Item(1);
        System.out.print("Item 1 = " + obj);
        if (obj == "OK3") {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        obj = iList.Item(2);
        System.out.print("Item 2 = " + iList.Item(2));
        if (obj == "OK2") {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        iList.RemoveAt(1);
        int count = iList.Count();
        System.out.print("Remove at 1, count = " + count);
        if (count == 2) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        obj = iList.Item(0);
        System.out.print("Item 0 = " + obj);
        if (obj == "OK1") {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        obj = iList.Item(1);
        System.out.print("Item 1 = " + obj);
        if (obj == "OK2") {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        iList.Clear();
        count = iList.Count();
        System.out.print("Clear, count = " + count);
        if (count == 0) {
            System.out.println(": OK");
        } else {
            System.out.println(": Fail");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        return result;
    }

    private short TestComponent() {
        System.out.println("\n==== Test Component ====");

        iTest = new IEcoTest1Native();

        System.out.print("IEcoTest1 QueryComponent");
        short result = iBus.QueryComponent(IdEcoTest1.CID, new IEcoUnknownNative(), IEcoTest1.IID, new Pointer<>((iTest)));
        if (result != 0) {
            System.out.println(": Fail\n");
            return result;
        }
        System.out.println(": OK\n");

        /* ==== TestInt8 ==== */
        byte int8_in = 12;
        Pointer<Byte> int8_in_out = new Pointer<>((byte) -23);
        Pointer<Byte> int8_out = new Pointer<>((byte) 0);
        byte int8_res;

        int8_res = iTest.TestInt8(int8_in, int8_in_out, int8_out);
        System.out.println("12 * 2 = " + int8_res);
        System.out.println("12 - 23 = " + int8_in_out.value);
        System.out.println("-42 = " + int8_out.value);
        if (int8_res == 24 && int8_in_out.value == -11 && int8_out.value == -42) {
            System.out.println("TestInt8: OK\n");
        } else {
            System.out.println("TestInt8: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestInt16 ==== */
        short int16_in = 1234;
        Pointer<Short> int16_in_out = new Pointer<>((short) -2345);
        Pointer<Short> int16_out = new Pointer<>((short) 0);
        short int16_res;

        int16_res = iTest.TestInt16(int16_in, int16_in_out, int16_out);
        System.out.println("1234 * 2 = " + int16_res);
        System.out.println("1234 - 2345 = " + int16_in_out.value);
        System.out.println("-1234 = " + int16_out.value);
        if (int16_res == 2468 && int16_in_out.value == -1111 && int16_out.value == -1234) {
            System.out.println("TestInt16: OK\n");
        } else {
            System.out.println("TestInt16: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestInt32 ==== */
        int int32_in = 123456;
        Pointer<Integer> int32_in_out = new Pointer<>(-234567);
        Pointer<Integer> int32_out = new Pointer<>(0);
        int int32_res;

        int32_res = iTest.TestInt32(int32_in, int32_in_out, int32_out);
        System.out.println("123456 * 2 = " + int32_res);
        System.out.println("123456 - 234567 = " + int32_in_out.value);
        System.out.println("-123456 = " + int32_out.value);
        if (int32_res == 246912 && int32_in_out.value == -111111 && int32_out.value == -123456) {
            System.out.println("TestInt32: OK\n");
        } else {
            System.out.println("TestInt32: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestInt64 ==== */
        long int64_in = 12345678901L;
        Pointer<Long> int64_in_out = new Pointer<>(-23456789012L);
        Pointer<Long> int64_out = new Pointer<>(0L);
        long int64_res;

        int64_res = iTest.TestInt64(int64_in, int64_in_out, int64_out);
        System.out.println("12345678901 * 2 = " + int64_res);
        System.out.println("12345678901 - 23456789012 = " + int64_in_out.value);
        System.out.println("-1234567890123 = " + int64_out.value);
        if (int64_res == 24691357802L && int64_in_out.value == -11111110111L && int64_out.value == -1234567890123L) {
            System.out.println("TestInt64: OK\n");
        } else {
            System.out.println("TestInt64: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestUInt8 ==== */
        byte uint8_in = 21;
        Pointer<Byte> uint8_in_out = new Pointer<>((byte) 32);
        Pointer<Byte> uint8_out = new Pointer<>((byte) 0);
        byte uint8_res;

        uint8_res = iTest.TestUInt8(uint8_in, uint8_in_out, uint8_out);
        System.out.println("21 * 2 = " + uint8_res);
        System.out.println("21 + 32 = " + uint8_in_out.value);
        System.out.println("200 = " + Byte.toUnsignedInt(uint8_out.value));
        if (uint8_res == 42 && uint8_in_out.value == 53 && Byte.toUnsignedInt(uint8_out.value) == 200) {
            System.out.println("TestUInt8: OK\n");
        } else {
            System.out.println("TestUInt8: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestUInt16 ==== */
        short uint16_in = 4321;
        Pointer<Short> uint16_in_out = new Pointer<>((short) 5432);
        Pointer<Short> uint16_out = new Pointer<>((short) 0);
        short uint16_res;

        uint16_res = iTest.TestUInt16(uint16_in, uint16_in_out, uint16_out);
        System.out.println("4321 * 2 = " + uint16_res);
        System.out.println("4321 + 5432 = " + uint16_in_out.value);
        System.out.println("50000 = " + Short.toUnsignedInt(uint16_out.value));
        if (uint16_res == 8642 && uint16_in_out.value == 9753 && Short.toUnsignedInt(uint16_out.value) == 50000) {
            System.out.println("TestUInt16: OK\n");
        } else {
            System.out.println("TestUInt16: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestUInt32 ==== */
        int uint32_in = 654321;
        Pointer<Integer> uint32_in_out = new Pointer<>(765432);
        Pointer<Integer> uint32_out = new Pointer<>(0);
        int uint32_res;

        uint32_res = iTest.TestUInt32(uint32_in, uint32_in_out, uint32_out);
        System.out.println("654321 * 2 = " + uint32_res);
        System.out.println("654321 + 765432 = " + uint32_in_out.value);
        System.out.println("3000000000 = " + Integer.toUnsignedLong(uint32_out.value));
        if (uint32_res == 1308642 && uint32_in_out.value == 1419753 && Integer.toUnsignedLong(uint32_out.value) == 3000000000L) {
            System.out.println("TestUInt32: OK\n");
        } else {
            System.out.println("TestUInt32: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestInt64 ==== */
        long uint64_in = 10987654321L;
        Pointer<Long> uint64_in_out = new Pointer<>(21098765432L);
        Pointer<Long> uint64_out = new Pointer<>(0L);
        long uint64_res;

        uint64_res = iTest.TestUInt64(uint64_in, uint64_in_out, uint64_out);
        System.out.println("10987654321 * 2 = " + uint64_res);
        System.out.println("10987654321 + 21098765432 = " + uint64_in_out.value);
        System.out.println("9876543210123 = " + uint64_out.value);
        if (uint64_res == 21975308642L && uint64_in_out.value == 32086419753L && uint64_out.value == 9876543210123L) {
            System.out.println("TestUInt64: OK\n");
        } else {
            System.out.println("TestUInt64: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestFloat ==== */
        float float_in = 1.25F;
        Pointer<Float> float_in_out = new Pointer<>(2.5F);
        Pointer<Float> float_out = new Pointer<>(0F);
        float float_res;

        float_res = iTest.TestFloat(float_in, float_in_out, float_out);
        System.out.println("1.25 * 2 = " + float_res);
        System.out.println("1.25 + 2.5 = " + float_in_out.value);
        System.out.println("1.5 = " + float_out.value);
        if (float_res == 2.5F && float_in_out.value == 3.75F && float_out.value == 1.5F) {
            System.out.println("TestFloat: OK\n");
        } else {
            System.out.println("TestFloat: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestDouble ==== */
        double double_in = 1.125;
        Pointer<Double> double_in_out = new Pointer<>(2.75);
        Pointer<Double> double_out = new Pointer<>(0.0);
        double double_res;

        double_res = iTest.TestDouble(double_in, double_in_out, double_out);
        System.out.println("1.125 * 2 = " + double_res);
        System.out.println("1.125 + 2.75 = " + double_in_out.value);
        System.out.println("3.141592653589793 = " + double_out.value);
        if (double_res == 2.25 && double_in_out.value == 3.875 && double_out.value == 3.1415926535897932) {
            System.out.println("TestDouble: OK\n");
        } else {
            System.out.println("TestDouble: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestBoolean ==== */
        boolean bool_in = true;
        Pointer<Boolean> bool_in_out = new Pointer<>(false);
        Pointer<Boolean> bool_out = new Pointer<>(false);
        boolean bool_res;

        bool_res = iTest.TestBoolean(bool_in, bool_in_out, bool_out);
        System.out.println("!true = " + bool_res);
        System.out.println("false ^ true = " + bool_in_out.value);
        System.out.println("true = " + bool_out.value);
        if (!bool_res && bool_in_out.value && bool_out.value) {
            System.out.println("TestBoolean: OK\n");
        } else {
            System.out.println("TestBoolean: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestChar ==== */
        char char_in = 'A';
        Pointer<Character> char_in_out = new Pointer<>((char) 4);
        Pointer<Character> char_out = new Pointer<>((char) 0);
        char char_res;

        char_res = iTest.TestChar(char_in, char_in_out, char_out);
        System.out.println("A + 1 = " + char_res);
        System.out.println("A + 4 = " + char_in_out.value);
        System.out.println("Z = " + char_out.value);
        if (char_res == 'B' && char_in_out.value == 'E' && char_out.value == 'Z') {
            System.out.println("TestChar: OK\n");
        } else {
            System.out.println("TestChar: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestWChar ==== */
        char wchar_in = 'а';
        Pointer<Character> wchar_in_out = new Pointer<>((char) 4);
        Pointer<Character> wchar_out = new Pointer<>((char) 0);
        char wchar_res;

        wchar_res = iTest.TestWChar(wchar_in, wchar_in_out, wchar_out);
        System.out.println("а + 1 = " + wchar_res);
        System.out.println("а + 4 = " + wchar_in_out.value);
        System.out.println("я = " + wchar_out.value);
        if (wchar_res == 'б' && wchar_in_out.value == 'д' && wchar_out.value == 'я') {
            System.out.println("TestWChar: OK\n");
        } else {
            System.out.println("TestWChar: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestAString ==== */
        String astring_in = "IN";
        Pointer<String> astring_in_out = new Pointer<>("INOUT");
        Pointer<String> astring_out = new Pointer<>("");
        String astring_res;

        astring_res = iTest.TestAString(astring_in, astring_in_out, astring_out);
        System.out.println(astring_res);
        System.out.println(astring_in_out.value);
        System.out.println(astring_out.value);
        if (astring_res.equals("RESULT:IN") && astring_in_out.value.equals("INOUT:IN") && astring_out.value.equals("OUT")) {
            System.out.println("TestAString: OK\n");
        } else {
            System.out.println("TestAString: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestWString ==== */
        String wstring_in = "ВХОД";
        Pointer<String> wstring_in_out = new Pointer<>("ВХОД-ВЫХОД");
        Pointer<String> wstring_out = new Pointer<>("");
        String wstring_res;

        wstring_res = iTest.TestWString(wstring_in, wstring_in_out, wstring_out);
        System.out.println(wstring_res);
        System.out.println(wstring_in_out.value);
        System.out.println(wstring_out.value);
        if (wstring_res.equals("РЕЗУЛЬТАТ:ВХОД") && wstring_in_out.value.equals("ВХОД-ВЫХОД:ВХОД") && wstring_out.value.equals("ВЫХОД")) {
            System.out.println("TestWString: OK\n");
        } else {
            System.out.println("TestWString: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestUGUID ==== */
        UGUID uguid_in = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F});
        Pointer<UGUID> uguid_in_out = new Pointer<>(new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
        Pointer<UGUID> uguid_out = new Pointer<>();
        UGUID uguid_res;

        uguid_res = iTest.TestUGUID(uguid_in, uguid_in_out, uguid_out);
        System.out.println(uguid_res);
        System.out.println(uguid_in_out.value);
        System.out.println(uguid_out.value);
        if (
                uguid_res.equals(new UGUID((byte) 0x01, (byte) 0x10, new byte[]{(byte) 0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F})) &&
                uguid_in_out.value.equals(uguid_in) &&
                uguid_out.value.equals(new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, (byte) 0x88, (byte) 0x99, (byte) 0xAA, (byte) 0xBB, (byte) 0xCC, (byte) 0xDD, (byte) 0xEE, (byte) 0xFF, 0x00}))
        ) {
            System.out.println("TestUGUID: OK\n");
        } else {
            System.out.println("TestUGUID: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestInterface ==== */
        IEcoUnknown interface_in = iTest;
        Pointer<IEcoUnknown> interface_in_out = new Pointer<>(new IEcoUnknownNative());
        Pointer<IEcoUnknown> interface_out = new Pointer<>(new IEcoUnknownNative());
        IEcoUnknown interface_res;

        interface_res = iTest.TestInterface(interface_in, interface_in_out, interface_out);
        int count1 = interface_res.Release();
        System.out.println("res.Release: " + count1);
        int count2 = interface_in_out.value.Release();
        System.out.println("in_out.Release: " + count2);
        int count3 = interface_out.value.AddRef();
        System.out.println("out.AddRef: " + count3);
        int count4 = iTest.Release();
        System.out.println("orig.Release: " + count4);
        if (count1 == 2 && count2 == 1 && count3 == 2 && count4 == 1) {
            System.out.println("TestInterface: OK\n");
        } else {
            System.out.println("TestInterface: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestVoidPtr ==== */
        Object obj_in = "OK";
        Pointer<Object> obj_in_out = new Pointer<>(0);
        Pointer<Object> obj_out = new Pointer<>(false);
        Object obj_res;

        obj_res = iTest.TestVoidPtr(obj_in, obj_in_out, obj_out);
        System.out.println("res = " + obj_res);
        System.out.println("in+out = " + obj_in_out.value);
        System.out.println("out = " + obj_out.value);
        if (obj_res.equals("OK") && obj_in_out.value.equals("OK") && obj_out.value.equals("OK")) {
            System.out.println("TestVoidPtr: OK\n");
        } else {
            System.out.println("TestVoidPtr: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        /* ==== TestVoid + GetLastVoidInput ==== */
        iTest.TestVoid(42);
        int output = iTest.GetLastVoidInput();
        System.out.println("output = " + output);
        if (output == 42) {
            System.out.println("TestVoid + GetLastVoidInput: OK\n");
        } else {
            System.out.println("TestVoid + GetLastVoidInput: Fail\n");
            result = ErrEcoCodes.ERR_ECO_FAIL;
        }

        return result;
    }

    private void Release() {
        System.out.println("\n==== Release ====");

        if (iSys != null && !iSys.isNull()) {
            System.out.println("IEcoSystem1 Release");
            iSys.Release();
        }
        if (iBus != null && !iBus.isNull()) {
            System.out.println("IEcoInterfaceBus1 Release");
            iBus.Release();
        }
        if (iCalcX != null && !((IEcoCalculatorXNative) iCalcX).isNull()) {
            System.out.println("IEcoCalculatorX Release");
            iCalcX.Release();
        }
        if (iCalcY != null && !((IEcoCalculatorYNative) iCalcY).isNull()) {
            System.out.println("IEcoCalculatorY Release");
            iCalcY.Release();
        }
        if (iList != null && !((IEcoList1Native) iList).isNull()) {
            System.out.println("IEcoList1 Release");
            iList.Release();
        }
        if (iTest != null && !((IEcoTest1Native) iTest).isNull()) {
            System.out.println("IEcoTestC1 Release");
            iTest.Release();
        }
    }
}
