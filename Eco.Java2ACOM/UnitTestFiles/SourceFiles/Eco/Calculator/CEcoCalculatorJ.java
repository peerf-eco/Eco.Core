package Eco.Calculator;

import Eco.Core.ErrEcoCodes;
import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;

class CEcoCalculatorJ {
    private IEcoCalculatorX m_iCalcX;
    private IEcoCalculatorY m_iCalcY;
    private int m_cRef;

    CEcoCalculatorJ() {
        m_iCalcX = new CEcoCalculatorJ_IEcoCalculatorX();
        m_iCalcY = new CEcoCalculatorJ_IEcoCalculatorY();
        m_cRef = 1;
    }

    short Init(IEcoUnknown iUnkSystem) {
        return ErrEcoCodes.ERR_ECO_OK;
    }

    short Create(IEcoUnknown iUnkSystem, IEcoUnknown iUnkOuter) {
        return ErrEcoCodes.ERR_ECO_OK;
    }

    void Delete() {}

    short QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk) {
        if (riid.equals(IEcoCalculatorX.IID)) {
            pIUnk.iUnk = m_iCalcX;
            m_iCalcX.AddRef();
        } else if (riid.equals(IEcoCalculatorY.IID)) {
            pIUnk.iUnk = m_iCalcY;
            m_iCalcY.AddRef();
        } else if (riid.equals(IEcoUnknown.IID)) {
            pIUnk.iUnk = m_iCalcX;
            m_iCalcX.AddRef();
        } else {
            pIUnk.iUnk = null;
            return ErrEcoCodes.ERR_ECO_NOINTERFACE;
        }
        return ErrEcoCodes.ERR_ECO_OK;
    }

    int AddRef() {
        m_cRef++;
        return m_cRef;
    }

    int Release() {
        m_cRef--;
        if (m_cRef == 0) {
            Delete();
        }
        return m_cRef;
    }

    private class CEcoCalculatorJ_IEcoCalculatorX implements IEcoCalculatorX {
        @Override
        public short QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk) {
            return CEcoCalculatorJ.this.QueryInterface(riid, pIUnk);
        }

        @Override
        public int AddRef() {
            return CEcoCalculatorJ.this.AddRef();
        }

        @Override
        public int Release() {
            return CEcoCalculatorJ.this.Release();
        }

        @Override
        public int Addition(short a, short b) {
            return a + b;
        }

        @Override
        public short Subtraction(short a, short b) {
            return (short) (a - b);
        }
    }

    private class CEcoCalculatorJ_IEcoCalculatorY implements IEcoCalculatorY {
        @Override
        public short QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk) {
            return CEcoCalculatorJ.this.QueryInterface(riid, pIUnk);
        }

        @Override
        public int AddRef() {
            return CEcoCalculatorJ.this.AddRef();
        }

        @Override
        public int Release() {
            return CEcoCalculatorJ.this.Release();
        }

        @Override
        public int Multiplication(short a, short b) {
            return a * b;
        }

        @Override
        public short Division(short a, short b) {
            return (short) (a / b);
        }
    }
}
