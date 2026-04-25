package Eco.Core;

public class IEcoUnknownNative implements IEcoUnknown {
    protected long pInstance = 0;

    public boolean isNull() {
        return pInstance == 0;
    }

    @Override
    public int QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk) {
        return (Integer) invokeNative(0, new Object[]{riid, pIUnk}, int.class);
    }

    @Override
    public int AddRef() {
        return (Integer) invokeNative(1, new Object[]{}, int.class);
    }

    @Override
    public int Release() {
        return (Integer) invokeNative(2, new Object[]{}, int.class);
    }

    protected native Object invokeNative(int VTblIdx, Object[] args, Class<?> returnType);
}
