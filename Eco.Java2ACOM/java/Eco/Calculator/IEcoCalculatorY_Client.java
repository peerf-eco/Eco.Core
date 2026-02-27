package Eco.Calculator;

import Eco.Core.IEcoUnknown_Client;

public class IEcoCalculatorY_Client extends IEcoUnknown_Client implements IEcoCalculatorY {
    @Override
    public int Multiplication(int a, int b) {
        return (Integer) invokeNative(3, new Object[]{a, b}, int.class);
    }

    @Override
    public int Division(int a, int b) {
        return (Integer) invokeNative(4, new Object[]{a, b}, int.class);
    }
}
