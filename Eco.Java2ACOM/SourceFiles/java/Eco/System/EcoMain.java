package Eco.System;

import Eco.Core.IEcoSystemNative;

import java.util.ServiceLoader;

class EcoMain {
    public static void main(String[] args) {
        IEcoSystemNative iSys = new IEcoSystemNative();
        int result = CEcoSystem.createCEcoSystem(args, iSys);
        if (result != 0) {
            System.exit(result);
        }
        ServiceLoader<EcoApp> loader = ServiceLoader.load(EcoApp.class);
        for (EcoApp app : loader) {
            app.EcoMain(iSys);
        }
        CEcoSystem.deleteCEcoSystem(iSys);
    }
}
