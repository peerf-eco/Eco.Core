package Eco.System;

import Eco.Core.IEcoSystem1Native;

import java.util.ServiceLoader;

class EcoMain {
    public static void main(String[] args) {
        IEcoSystem1Native iSys = new IEcoSystem1Native();
        short result = CEcoSystem.createCEcoSystem(args, iSys);
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
