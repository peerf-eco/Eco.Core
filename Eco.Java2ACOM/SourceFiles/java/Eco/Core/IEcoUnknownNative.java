package Eco.Core;

public class IEcoUnknownNative implements IEcoUnknown {
    protected long pInstance = 0;

    public boolean isNull() {
        return pInstance == 0;
    }

    @Override
    public native short QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk);

    @Override
    public native int AddRef();

    @Override
    public native int Release();

    protected native Object GlobalDispatcher(int VTblIdx, Object[] args);
}
