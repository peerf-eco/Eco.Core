import Eco.System.CEcoSystem;
import Eco.Core.IEcoSystem;
import Eco.Core.IEcoUnknown;
import Eco.InterfaceBus.IEcoInterfaceBus;
import Eco.MemoryManager.IEcoMemoryManager;
import Eco.MemoryManager.IEcoMemoryAllocator;
import Eco.FileSystemManagement.IEcoFileManager;
import Eco.FileSystemManagement.IEcoFileSystemManagement;
import Eco.Calculator.IdEcoCalculatorC;
import Eco.Calculator.IEcoCalculatorX;
import Eco.Calculator.IEcoCalculatorY;

public class Main {
    private static IEcoSystem iSys = new IEcoSystem();
    private static IEcoInterfaceBus iBus = new IEcoInterfaceBus();
    private static IEcoMemoryAllocator iMem = new IEcoMemoryAllocator();
    private static IEcoFileManager iFileMgr = new IEcoFileManager();
    private static IEcoCalculatorX iCalcX = new IEcoCalculatorX();
    private static IEcoCalculatorY iCalcY = new IEcoCalculatorY();

    public static void main(String[] args) {
        IEcoSystem iSys = new IEcoSystem();
        int result = CEcoSystem.createCEcoSystem(args, iSys);
        if (result != 0) {
            System.exit(result);
        }
        EcoMain(iSys);
        CEcoSystem.deleteCEcoSystem(iSys);
    }

    private static void EcoMain(IEcoUnknown iUnk) {
        int result = iUnk.QueryInterface(IEcoSystem.GID, iSys);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        result = iSys.QueryInterface(IEcoInterfaceBus.IID, iBus);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        result = iBus.QueryComponent(IEcoMemoryManager.CID, new IEcoUnknown(), IEcoMemoryAllocator.IID, iMem);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        result = iBus.QueryComponent(IEcoFileSystemManagement.CID, new IEcoUnknown(), IEcoFileManager.IID, iFileMgr);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        result = iBus.QueryComponent(IdEcoCalculatorC.CID, new IEcoUnknown(), IEcoCalculatorX.IID, iCalcX);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, iCalcY);
        if (result != 0) {
            Release();
            System.exit(result);
        }
        System.out.println(iCalcX.Addition(9, 10));
        System.out.println(iCalcX.Subtraction(67, 13));
        System.out.println(iCalcY.Multiplication(6, 8));
        System.out.println(iCalcY.Division(42, 7));
        Release();
    }

    private static void Release() {
        if (!iCalcY.isNull()) {
            iCalcY.Release();
        }
        if (!iCalcX.isNull()) {
            iCalcX.Release();
        }
        if (!iFileMgr.isNull()) {
            iFileMgr.Release();
        }
        if (!iMem.isNull()) {
            iMem.Release();
        }
        if (!iBus.isNull()) {
            iBus.Release();
        }
        if (!iSys.isNull()) {
            iSys.Release();
        }
    }
}
