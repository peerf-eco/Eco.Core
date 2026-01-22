package Eco.System;

import Eco.Core.IEcoSystem;

import java.util.ServiceLoader;

public class EcoMain {
    public static void main(String[] args) {
        IEcoSystem iSys = new IEcoSystem();
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
