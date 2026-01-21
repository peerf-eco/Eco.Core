package Eco.Core;

public class IEcoUnknown {
    public static UGUID IID = new UGUID((byte) 0x01, (byte) 0x10, new byte[]{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xAA});

    private long pInstance = 0;

    public boolean isNull() {
        return pInstance == 0;
    }

    public native int QueryInterface(UGUID riid, IEcoUnknown iUnk);
    public native int AddRef();
    public native int Release();
}
