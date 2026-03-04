import Eco.Core.*;
import Eco.Calculator.*;
import Eco.System.EcoApp;
import Eco.InterfaceBus.IEcoInterfaceBus;
import Eco.MemoryManager.IEcoMemoryAllocator;
import Eco.FileSystemManagement.IEcoFileManager;
import Eco.InterfaceBus.IEcoInterfaceBusNative;
import Eco.MemoryManager.IEcoMemoryAllocatorNative;
import Eco.FileSystemManagement.IEcoFileManagerNative;
import Eco.MemoryManager.IdEcoMemoryManager;
import Eco.FileSystemManagement.IdEcoFileSystemManagement;

public class EcoTest implements EcoApp {
    private static IEcoSystemNative iSys;
    private static IEcoInterfaceBusNative iBus;
    private static IEcoMemoryAllocatorNative iMem;
    private static IEcoFileManagerNative iFileMgr;
    private static IEcoCalculatorX iCalcX;
    private static IEcoCalculatorY iCalcY;

    @Override
    public void EcoMain(IEcoUnknown iUnk) {
        int result = TestJNI(iUnk);
        System.out.println("result code = " + result + "\n");
        result = TestJava();
        System.out.println("result code = " + result + "\n");
    }

    private int TestJNI(IEcoUnknown iUnk) {
        System.out.println("==== JNI test ====");
        int result = InitJNI(iUnk);
        if (result == 0) {
            TestInterfaces();
        }
        ReleaseJNI();
        return result;
    }

    private int InitJNI(IEcoUnknown iUnk) {
        iSys = new IEcoSystemNative();
        iBus = new IEcoInterfaceBusNative();
        iMem = new IEcoMemoryAllocatorNative();
        iFileMgr = new IEcoFileManagerNative();
        iCalcX = new IEcoCalculatorXNative();
        iCalcY = new IEcoCalculatorYNative();

        int result = iUnk.QueryInterface(IEcoSystem.GID, new IEcoUnknownPtr(iSys));
        if (result != 0) return result;

        result = iSys.QueryInterface(IEcoInterfaceBus.IID, new IEcoUnknownPtr(iBus));
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoMemoryManager.CID, new IEcoUnknownNative(), IEcoMemoryAllocator.IID, new IEcoUnknownPtr(iMem));
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoFileSystemManagement.CID, new IEcoUnknownNative(), IEcoFileManager.IID, new IEcoUnknownPtr(iFileMgr));
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoCalculatorC.CID, new IEcoUnknownNative(), IEcoCalculatorX.IID, new IEcoUnknownPtr(iCalcX));
        if (result != 0) return result;

        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, new IEcoUnknownPtr(iCalcY));
        return result;
    }

    private void ReleaseJNI() {
        if (!((IEcoCalculatorYNative) iCalcY).isNull()) iCalcY.Release();
        if (!((IEcoCalculatorXNative) iCalcX).isNull()) iCalcX.Release();
        if (!iFileMgr.isNull()) iFileMgr.Release();
        if (!iMem.isNull()) iMem.Release();
        if (!iBus.isNull()) iBus.Release();
        if (!iSys.isNull()) iSys.Release();
    }

    private int TestJava() {
        System.out.println("==== Java test ====");
        int result = InitJava();
        if (result == 0) {
            TestInterfaces();
        }
        ReleaseJava();
        return result;
    }

    private int InitJava() {
        iCalcX = null;
        iCalcY = null;

        IEcoUnknownPtr pICalcX = new IEcoUnknownPtr();
        int result = new CEcoCalculatorJ().create(pICalcX);
        if (result != 0) return result;
        iCalcX = (IEcoCalculatorX) pICalcX.iUnk;

        IEcoUnknownPtr pICalcY = new IEcoUnknownPtr();
        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, pICalcY);
        if (result != 0) return result;
        iCalcY = (IEcoCalculatorY) pICalcY.iUnk;

        return result;
    }

    private void ReleaseJava() {
        if (iCalcY != null) iCalcY.Release();
        if (iCalcX != null) iCalcX.Release();
    }

    private void TestInterfaces() {
        System.out.println("9 + 10 = " + iCalcX.Addition(9, 10));
        System.out.println("67 - 13 = " + iCalcX.Subtraction(67, 13));
        System.out.println("6 * 8 = " + iCalcY.Multiplication(6, 8));
        System.out.println("42 / 7 = " + iCalcY.Division(42, 7));
    }
}
