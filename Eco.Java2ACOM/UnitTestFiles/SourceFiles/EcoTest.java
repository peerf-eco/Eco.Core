import Eco.Calculator.*;
import Eco.System.EcoApp;
import Eco.Core.IEcoSystem;
import Eco.Core.IEcoUnknown;
import Eco.InterfaceBus.IEcoInterfaceBus;
import Eco.MemoryManager.IEcoMemoryManager;
import Eco.MemoryManager.IEcoMemoryAllocator;
import Eco.FileSystemManagement.IEcoFileManager;
import Eco.FileSystemManagement.IEcoFileSystemManagement;
import Eco.Core.IEcoSystem_Client;
import Eco.Core.IEcoUnknown_Client;
import Eco.InterfaceBus.IEcoInterfaceBus_Client;
import Eco.MemoryManager.IEcoMemoryAllocator_Client;
import Eco.FileSystemManagement.IEcoFileManager_Client;

public class EcoTest implements EcoApp {
    private static IEcoSystem_Client iSys = new IEcoSystem_Client();
    private static IEcoInterfaceBus_Client iBus = new IEcoInterfaceBus_Client();
    private static IEcoMemoryAllocator_Client iMem = new IEcoMemoryAllocator_Client();
    private static IEcoFileManager_Client iFileMgr = new IEcoFileManager_Client();
    private static IEcoCalculatorX_Client iCalcX = new IEcoCalculatorX_Client();
    private static IEcoCalculatorY_Client iCalcY = new IEcoCalculatorY_Client();

    @Override
    public void EcoMain(IEcoUnknown iUnk) {
        int result = Init(iUnk);
        if (result == 0) {
            System.out.println(iCalcX.Addition(9, 10));
            System.out.println(iCalcX.Subtraction(67, 13));
            System.out.println(iCalcY.Multiplication(6, 8));
            System.out.println(iCalcY.Division(42, 7));
        }
        Release();
    }

    private int Init(IEcoUnknown iUnk) {
        int result = iUnk.QueryInterface(IEcoSystem.GID, iSys);
        if (result != 0) return result;

        result = iSys.QueryInterface(IEcoInterfaceBus.IID, iBus);
        if (result != 0) return result;

        result = iBus.QueryComponent(IEcoMemoryManager.CID, new IEcoUnknown_Client(), IEcoMemoryAllocator.IID, iMem);
        if (result != 0) return result;

        result = iBus.QueryComponent(IEcoFileSystemManagement.CID, new IEcoUnknown_Client(), IEcoFileManager.IID, iFileMgr);
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoCalculatorC.CID, new IEcoUnknown_Client(), IEcoCalculatorX.IID, iCalcX);
        if (result != 0) return result;

        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, iCalcY);
        return result;
    }

    private void Release() {
        if (!iCalcY.isNull()) iCalcY.Release();
        if (!iCalcX.isNull()) iCalcX.Release();
        if (!iFileMgr.isNull()) iFileMgr.Release();
        if (!iMem.isNull()) iMem.Release();
        if (!iBus.isNull()) iBus.Release();
        if (!iSys.isNull()) iSys.Release();
    }
}
