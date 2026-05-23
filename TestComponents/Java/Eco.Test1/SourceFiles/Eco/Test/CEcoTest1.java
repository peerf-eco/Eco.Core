package Eco.Test;

import Eco.Core.ErrEcoCodes;
import Eco.Core.IEcoUnknown;
import Eco.Core.Pointer;
import Eco.Core.UGUID;

class CEcoTest1 {
    private static final byte INT8 = -42;
    private static final short INT16 = -1234;
    private static final int INT32 = -123456;
    private static final long INT64 = -1234567890123L;
    private static final byte UINT8 = (byte) 200;
    private static final short UINT16 = (short) 50000;
    private static final int UINT32 = (int) 3000000000L;
    private static final long UINT64 = 9876543210123L;
    private static final float FLOAT = 1.5F;
    private static final double DOUBLE = 3.1415926535897932;
    private static final boolean BOOL = true;
    private static final char CHAR = 'Z';
    private static final char WCHAR = 'я';
    private static final String ASTRING = "OUT";
    private static final String WSTRING = "ВЫХОД";
    private static final UGUID UGUID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, (byte) 0x88, (byte) 0x99, (byte) 0xAA, (byte) 0xBB, (byte) 0xCC, (byte) 0xDD, (byte) 0xEE, (byte) 0xFF, 0x00});

    private final IEcoTest1 m_iTest;
    private int m_cRef;
    private int m_lastVoidInput;

    CEcoTest1() {
        m_iTest = new CEcoTest1_IEcoTest1();
        m_cRef = 1;
        m_lastVoidInput = 0;
    }

    short Init(IEcoUnknown iUnkSystem) {
        return ErrEcoCodes.ERR_ECO_OK;
    }

    short Create(IEcoUnknown iUnkSystem, IEcoUnknown iUnkOuter) {
        return ErrEcoCodes.ERR_ECO_OK;
    }

    void Delete() {}

    short QueryInterface(UGUID riid, Pointer<IEcoUnknown> pIUnk) {
        if (riid.equals(IEcoTest1.IID)) {
            pIUnk.value = m_iTest;
            m_iTest.AddRef();
        } else if (riid.equals(IEcoUnknown.IID)) {
            pIUnk.value = m_iTest;
            m_iTest.AddRef();
        } else {
            pIUnk.value = null;
            return ErrEcoCodes.ERR_ECO_NOINTERFACE;
        }
        return ErrEcoCodes.ERR_ECO_OK;
    }

    int AddRef() {
        m_cRef++;
        return m_cRef;
    }

    int Release() {
        m_cRef--;
        if (m_cRef == 0) {
            Delete();
        }
        return m_cRef;
    }

    private class CEcoTest1_IEcoTest1 implements IEcoTest1 {
        @Override
        public short QueryInterface(UGUID riid, Pointer<IEcoUnknown> pIUnk) {
            return CEcoTest1.this.QueryInterface(riid, pIUnk);
        }

        @Override
        public int AddRef() {
            return CEcoTest1.this.AddRef();
        }

        @Override
        public int Release() {
            return CEcoTest1.this.Release();
        }

        @Override
        public byte TestInt8(byte in, Pointer<Byte> inOut, Pointer<Byte> out) {
            if (inOut != null) inOut.value = (byte) (inOut.value + in);
            if (out != null) out.value = INT8;
            return (byte) (in * 2);
        }

        @Override
        public short TestInt16(short in, Pointer<Short> inOut, Pointer<Short> out) {
            if (inOut != null) inOut.value = (short) (inOut.value + in);
            if (out != null) out.value = INT16;
            return (short) (in * 2);
        }

        @Override
        public int TestInt32(int in, Pointer<Integer> inOut, Pointer<Integer> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = INT32;
            return in * 2;
        }

        @Override
        public long TestInt64(long in, Pointer<Long> inOut, Pointer<Long> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = INT64;
            return in * 2;
        }

        @Override
        public byte TestUInt8(byte in, Pointer<Byte> inOut, Pointer<Byte> out) {
            if (inOut != null) inOut.value = (byte) (inOut.value + in);
            if (out != null) out.value = UINT8;
            return (byte) (in * 2);
        }

        @Override
        public short TestUInt16(short in, Pointer<Short> inOut, Pointer<Short> out) {
            if (inOut != null) inOut.value = (short) (inOut.value + in);
            if (out != null) out.value = UINT16;
            return (short) (in * 2);
        }

        @Override
        public int TestUInt32(int in, Pointer<Integer> inOut, Pointer<Integer> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = UINT32;
            return in * 2;
        }

        @Override
        public long TestUInt64(long in, Pointer<Long> inOut, Pointer<Long> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = UINT64;
            return in * 2;
        }

        @Override
        public float TestFloat(float in, Pointer<Float> inOut, Pointer<Float> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = FLOAT;
            return in * 2;
        }

        @Override
        public double TestDouble(double in, Pointer<Double> inOut, Pointer<Double> out) {
            if (inOut != null) inOut.value = inOut.value + in;
            if (out != null) out.value = DOUBLE;
            return in * 2;
        }

        @Override
        public boolean TestBoolean(boolean in, Pointer<Boolean> inOut, Pointer<Boolean> out) {
            if (inOut != null) inOut.value = inOut.value ^ in;
            if (out != null) out.value = BOOL;
            return !in;
        }

        @Override
        public char TestChar(char in, Pointer<Character> inOut, Pointer<Character> out) {
            if (inOut != null) inOut.value = (char) (inOut.value + in);
            if (out != null) out.value = CHAR;
            return (char) (in + 1);
        }

        @Override
        public char TestWChar(char in, Pointer<Character> inOut, Pointer<Character> out) {
            if (inOut != null) inOut.value = (char) (inOut.value + in);
            if (out != null) out.value = WCHAR;
            return (char) (in + 1);
        }

        @Override
        public String TestAString(String in, Pointer<String> inOut, Pointer<String> out) {
            if (inOut != null) inOut.value = inOut.value + ":" + in;
            if (out != null) out.value = ASTRING;
            return "RESULT:" + in;
        }

        @Override
        public String TestWString(String in, Pointer<String> inOut, Pointer<String> out) {
            if (inOut != null) inOut.value = inOut.value + ":" + in;
            if (out != null) out.value = WSTRING;
            return "РЕЗУЛЬТАТ:" + in;
        }

        @Override
        public UGUID TestUGUID(UGUID in, Pointer<UGUID> inOut, Pointer<UGUID> out) {
            if (inOut != null) inOut.value = new UGUID(in);
            if (out != null) out.value = new UGUID(UGUID);
            UGUID result = new UGUID(in);
            result.Data[0] = (byte) ~(result.Data[0] & 0xFF);
            return result;
        }

        @Override
        public IEcoUnknown TestInterface(IEcoUnknown in, Pointer<IEcoUnknown> inOut, Pointer<IEcoUnknown> out) {
            in.AddRef();
            in.Release();
            if (inOut != null) {
                if (inOut.value != null) {
                    inOut.value.Release();
                }
                in.AddRef();
                inOut.value = in;
            }
            if (out != null) {
                in.AddRef();
                out.value = in;
            }
            return in;
        }

        @Override
        public Object TestVoidPtr(Object in, Pointer<Object> inOut, Pointer<Object> out) {
            if (inOut != null) inOut.value = in;
            if (out != null) out.value = in;
            return in;
        }

        @Override
        public void TestVoid(int in) {
            m_lastVoidInput = in;
        }

        @Override
        public int GetLastVoidInput() {
            return m_lastVoidInput;
        }
    }
}
