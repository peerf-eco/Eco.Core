package Eco.System;

import Eco.Core.IEcoSystem;

public class CEcoSystem {
    static {
        System.loadLibrary("Java2ACOM");
    }

    public static native int createCEcoSystem(String[] args, IEcoSystem iSys);
    public static native void deleteCEcoSystem(IEcoSystem iSys);
}
