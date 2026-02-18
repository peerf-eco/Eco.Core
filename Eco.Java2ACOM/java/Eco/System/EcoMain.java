package Eco.System;

import Eco.Core.IEcoSystem_Client;

import java.util.ServiceLoader;

public class EcoMain {
    public static void main(String[] args) {
        IEcoSystem_Client iSys = new IEcoSystem_Client();
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
