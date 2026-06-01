import Eco.Core.*;
import Eco.Calculator.*;
import Eco.System.EcoApp;
import Eco.InterfaceBus.IEcoInterfaceBus1;
import Eco.MemoryManager.IEcoMemoryAllocator1;
import Eco.FileSystemManagement.IEcoFileManager1;
import Eco.InterfaceBus.IEcoInterfaceBus1Native;
import Eco.MemoryManager.IEcoMemoryAllocator1Native;
import Eco.FileSystemManagement.IEcoFileManager1Native;
import Eco.MemoryManager.IdEcoMemoryManager1;
import Eco.FileSystemManagement.IdEcoFileSystemManagement1;

public class EcoTest implements EcoApp {
    private static IEcoSystem1Native iSys;
    private static IEcoInterfaceBus1Native iBus;
    private static IEcoMemoryAllocator1Native iMem;
    private static IEcoFileManager1Native iFileMgr;
    private static IEcoCalculatorX iCalcX;
    private static IEcoCalculatorY iCalcY;

    @Override
    public void EcoMain(IEcoUnknown iUnk) {
        short result = TestJNI(iUnk);
        System.out.println("result code = " + result + "\n");
        result = TestJava();
        System.out.println("result code = " + result + "\n");
    }

    private short TestJNI(IEcoUnknown iUnk) {
        System.out.println("==== JNI test ====");
        short result = InitJNI(iUnk);
        if (result == 0) {
            TestInterfaces();
        }
        ReleaseJNI();
        return result;
    }

    private short InitJNI(IEcoUnknown iUnk) {
        iSys = new IEcoSystem1Native();
        iBus = new IEcoInterfaceBus1Native();
        iMem = new IEcoMemoryAllocator1Native();
        iFileMgr = new IEcoFileManager1Native();
        iCalcX = new IEcoCalculatorXNative();
        iCalcY = new IEcoCalculatorYNative();

        short result = iUnk.QueryInterface(IEcoSystem1.GID_x86_32, new IEcoUnknownPtr(iSys));
        if (result != 0) return result;

        result = iSys.QueryInterface(IEcoInterfaceBus1.IID, new IEcoUnknownPtr(iBus));
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoMemoryManager1.CID, new IEcoUnknownNative(), IEcoMemoryAllocator1.IID, new IEcoUnknownPtr(iMem));
        if (result != 0) return result;

        result = iBus.QueryComponent(IdEcoFileSystemManagement1.CID, new IEcoUnknownNative(), IEcoFileManager1.IID, new IEcoUnknownPtr(iFileMgr));
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

    private short TestJava() {
        System.out.println("==== Java test ====");
        short result = InitJava();
        if (result == 0) {
            TestInterfaces();
        }
        ReleaseJava();
        return result;
    }

    private short InitJava() {
        iCalcX = null;
        iCalcY = null;

        IEcoUnknownPtr pICalcX = new IEcoUnknownPtr();
        short result = new CEcoCalculatorJ().create(pICalcX);
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
        System.out.println("9 + 10 = " + iCalcX.Addition((short) 9, (short) 10));
        System.out.println("67 - 13 = " + iCalcX.Subtraction((short) 67, (short) 13));
        System.out.println("6 * 8 = " + iCalcY.Multiplication((short) 6, (short) 8));
        System.out.println("42 / 7 = " + iCalcY.Division((short) 42, (short) 7));
    }
}
