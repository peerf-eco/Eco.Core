package Eco.Calculator;

import Eco.Core.ErrEcoCodes;
import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;

public class CEcoCalculatorJFactory {
    public static short Alloc(IEcoUnknown iSystem, IEcoUnknown iUnknownOuter, UGUID riid, IEcoUnknownPtr pIUnknown) {
        if (iUnknownOuter != null && !riid.equals(IEcoUnknown.IID)) {
            return ErrEcoCodes.ERR_ECO_NOAGGREGATION;
        }
        CEcoCalculatorJ cObj = new CEcoCalculatorJ();
        short result = cObj.Create(iSystem, iUnknownOuter);
        if (result != 0) return result;
        result = cObj.Init(iSystem);
        if (result != 0) return result;
        result = cObj.QueryInterface(riid, pIUnknown);
        if (result != 0) return result;
        cObj.Release();
        return result;
    }
}
