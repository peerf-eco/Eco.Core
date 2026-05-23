package Eco.Calculator;

import Eco.Core.*;

public class CEcoCalculatorJFactory implements IEcoComponentFactory {
    private int m_cRef;
    private String m_Name;
    private String m_Version;
    private String m_Manufacturer;

    public CEcoCalculatorJFactory() {
        m_cRef = 0;
        m_Name = "EcoCalculatorJ";
        m_Version = "1.0.0.0";
        m_Manufacturer = "PeerF";
    }

    @Override
    public short QueryInterface(UGUID riid, Pointer<IEcoUnknown> pIUnk) {
        if (riid.equals(IEcoUnknown.IID) || riid.equals(IEcoComponentFactory.IID)) {
            pIUnk.value = this;
        } else {
            pIUnk.value = null;
            return ErrEcoCodes.ERR_ECO_NOINTERFACE;
        }
        AddRef();
        return 0;
    }

    @Override
    public int AddRef() {
        m_cRef++;
        return m_cRef;
    }

    @Override
    public int Release() {
        m_cRef--;
        return m_cRef;
    }

    @Override
    public short Init(IEcoUnknown iSystem, IEcoUnknown iUnk) {
        return ErrEcoCodes.ERR_ECO_OK;
    }

    @Override
    public short Alloc(IEcoUnknown iSystem, IEcoUnknown iUnknownOuter, UGUID riid, Pointer<IEcoUnknown> pIUnk) {
        if (iUnknownOuter != null && !riid.equals(IEcoUnknown.IID)) {
            return ErrEcoCodes.ERR_ECO_NOAGGREGATION;
        }
        CEcoCalculatorJ cObj = new CEcoCalculatorJ();
        short result = cObj.Create(iSystem, iUnknownOuter);
        if (result != 0) return result;
        result = cObj.Init(iSystem);
        if (result != 0) return result;
        result = cObj.QueryInterface(riid, pIUnk);
        if (result != 0) return result;
        cObj.Release();
        return result;
    }

    @Override
    public String get_Name() {
        return m_Name;
    }

    @Override
    public String get_Version() {
        return m_Version;
    }

    @Override
    public String get_Manufacturer() {
        return m_Manufacturer;
    }
}
