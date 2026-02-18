package Eco.Core;

public class IEcoSystem_Client extends IEcoUnknown_Client implements IEcoSystem {
    @Override
    public void set_Data(long pv) {
        invokeNative(3, new Object[]{pv}, void.class);
    }

    @Override
    public long get_Data() {
        return (Long) invokeNative(4, new Object[]{}, long.class);
    }
}
