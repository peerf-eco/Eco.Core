package Eco.Core;

public class IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xAA});

    protected long pInstance = 0;

    public boolean isNull() {
        return pInstance == 0;
    }

    protected native Object invokeNative(int VTblIdx, Object[] args, Class<?> returnType);

    public native int QueryInterface(UGUID riid, IEcoUnknown iUnk);
    public native int AddRef();
    public native int Release();
}
