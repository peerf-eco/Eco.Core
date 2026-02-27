package Eco.Calculator;

import Eco.Core.IEcoUnknown_Client;

public class IEcoCalculatorX_Client extends IEcoUnknown_Client implements IEcoCalculatorX {
    @Override
    public int Addition(int a, int b) {
        return (Integer) invokeNative(3, new Object[]{a, b}, int.class);
    }

    @Override
    public int Subtraction(int a, int b) {
        return (Integer) invokeNative(4, new Object[]{a, b}, int.class);
    }
}
