package Eco.Core;

public class IEcoUnknown_Client implements IEcoUnknown {
    public long pInstance = 0;

    public boolean isNull() {
        return pInstance == 0;
    }

    protected native Object invokeNative(int VTblIdx, Object[] args, Class<?> returnType);

    @Override
    public int QueryInterface(UGUID riid, IEcoUnknown iUnk) {
        OutPointer pIUnk = new OutPointer(iUnk);
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
}
