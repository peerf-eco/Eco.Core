package Eco.System;

import Eco.Core.IEcoSystem1;

class CEcoSystem {
    static {
        System.loadLibrary("Java2ACOM");
    }

    public static native int createCEcoSystem(String[] args, IEcoSystem1 iSys);
    public static native void deleteCEcoSystem(IEcoSystem1 iSys);
}
