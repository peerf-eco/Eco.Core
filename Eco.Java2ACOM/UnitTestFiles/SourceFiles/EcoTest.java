import Eco.Core.*;
import Eco.Calculator.*;
import Eco.List.IEcoList1;
import Eco.List.IEcoList1Native;
import Eco.List.IdEcoList1;
import Eco.System.EcoApp;
import Eco.InterfaceBus.IEcoInterfaceBus1;
import Eco.InterfaceBus.IEcoInterfaceBus1Native;
import Eco.MemoryManager.IEcoMemoryAllocator1Native;
import Eco.FileSystemManagement.IEcoFileManager1Native;

public class EcoTest implements EcoApp {
    private static IEcoSystem1Native iSys;
    private static IEcoInterfaceBus1Native iBus;
    private static IEcoMemoryAllocator1Native iMem;
    private static IEcoFileManager1Native iFileMgr;
    private static IEcoCalculatorX iCalcX;
    private static IEcoCalculatorY iCalcY;
    private static IEcoList1 iList;

    @Override
    public void EcoMain(IEcoUnknown iUnk) {
        short result = Main(iUnk);
        Release();
        System.out.println("\nResult Code: " + result);
    }

    private short Main(IEcoUnknown iUnk) {
        short result = Init(iUnk);
        if (result != 0) return result;
        result = TestCalculator();
        if (result != 0) return result;
        result = TestList();
        if (result != 0) return result;
        return result;
    }

    private short Init(IEcoUnknown iUnk) {
        System.out.println("\n==== Init ====");

        iSys = new IEcoSystem1Native();
        iBus = new IEcoInterfaceBus1Native();

        short result = iUnk.QueryInterface(IEcoSystem1.GID_x86_32, new IEcoUnknownPtr(iSys));
        if (result != 0) return result;
        System.out.println("IEcoSystem1 QueryInterface: OK");

        result = iSys.QueryInterface(IEcoInterfaceBus1.IID, new IEcoUnknownPtr(iBus));
        if (result != 0) return result;
        System.out.println("IEcoInterfaceBus1 QueryInterface: OK");

        return result;
    }

    private short TestCalculator() {
        System.out.println("\n==== Test Calculator ====");

        iCalcX = new IEcoCalculatorXNative();
        iCalcY = new IEcoCalculatorYNative();

        short result = iBus.QueryComponent(IdEcoCalculatorC.CID, new IEcoUnknownNative(), IEcoCalculatorX.IID, new IEcoUnknownPtr(iCalcX));
        if (result != 0) return result;
        System.out.println("IEcoCalculatorX QueryComponent: OK");

        result = iCalcX.QueryInterface(IEcoCalculatorY.IID, new IEcoUnknownPtr(iCalcY));
        if (result != 0) return result;
        System.out.println("IEcoCalculatorY QueryInterface: OK");

        System.out.println("Addition: 9 + 10 = " + iCalcX.Addition((short) 9, (short) 10));
        System.out.println("Subtraction: 67 - 13 = " + iCalcX.Subtraction((short) 67, (short) 13));
        System.out.println("Multiplication: 6 * 8 = " + iCalcY.Multiplication((short) 6, (short) 8));
        System.out.println("Division: 42 / 7 = " + iCalcY.Division((short) 42, (short) 7));

        return result;
    }

    private short TestList() {
        System.out.println("\n==== Test List ====");

        iList = new IEcoList1Native();
        short result = iBus.QueryComponent(IdEcoList1.CID, new IEcoUnknownNative(), IEcoList1.IID, new IEcoUnknownPtr(iList));
        if (result != 0) return result;
        System.out.println("IEcoList1 QueryComponent: OK");

        Object obj1 = "OK1";
        Object obj2 = "OK2";
        Object obj3 = "OK3";

        System.out.println("Add " + obj1 + ", index: " + iList.Add(obj1));
        System.out.println("Add " + obj3 + ", index: " + iList.Add(obj3));
        System.out.println("Add " + obj2 + ", index: " + iList.Add(obj2));

        System.out.println("Item 0: " + iList.Item(0));
        System.out.println("Item 1: " + iList.Item(1));
        System.out.println("Item 2: " + iList.Item(2));

        iList.RemoveAt(1);
        System.out.println("Remove at 1, count: " + iList.Count());

        System.out.println("Item 0: " + iList.Item(0));
        System.out.println("Item 1: " + iList.Item(1));

        iList.Clear();
        System.out.println("Clear, count: " + iList.Count());

        return result;
    }

    private void Release() {
        System.out.println("\n==== Release ====");

        if (iSys != null && !iSys.isNull()) {
            System.out.println("IEcoSystem1 Release");
            iSys.Release();
        }
        if (iBus != null && !iBus.isNull()) {
            System.out.println("IEcoInterfaceBus1 Release");
            iBus.Release();
        }
        if (iCalcX != null && !((IEcoCalculatorXNative) iCalcX).isNull()) {
            System.out.println("IEcoCalculatorX Release");
            iCalcX.Release();
        }
        if (iCalcY != null && !((IEcoCalculatorYNative) iCalcY).isNull()) {
            System.out.println("IEcoCalculatorY Release");
            iCalcY.Release();
        }
        if (iList != null && !((IEcoList1Native) iList).isNull()) {
            System.out.println("IEcoList1 Release");
            iList.Release();
        }
    }
}
