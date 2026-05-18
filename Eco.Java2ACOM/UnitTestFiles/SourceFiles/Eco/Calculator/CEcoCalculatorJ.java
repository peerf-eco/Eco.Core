package Eco.Calculator;

import Eco.Core.ErrEcoCodes;
import Eco.Core.IEcoUnknown;
import Eco.Core.IEcoUnknownPtr;
import Eco.Core.UGUID;

public class CEcoCalculatorJ {
    private IEcoCalculatorX m_iCalcX = null;
    private IEcoCalculatorY m_iCalcY = null;
    private int m_cRef = 0;

    public short create(IEcoUnknownPtr pICalcX) {
        m_iCalcX = new CEcoCalculatorJ_IEcoCalculatorX();
        m_iCalcY = new CEcoCalculatorJ_IEcoCalculatorY();
        m_cRef = 1;
        pICalcX.iUnk = m_iCalcX;
        return ErrEcoCodes.ERR_ECO_OK;
    }

    private short QueryInterface(UGUID riid, IEcoUnknownPtr pIUnk) {
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

    private int AddRef() {
        m_cRef++;
        return ErrEcoCodes.ERR_ECO_OK;
    }

    private int Release() {
        m_cRef--;
        return ErrEcoCodes.ERR_ECO_OK;
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
