/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoACOM2Python
 * </summary>
 *
 * <description>
 *   This source file is the entry point
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */


#include <stdio.h>

/* Eco OS */
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IdEcoACOM2Python.h"
#include "IdEcoList1.h"
#include "IdEcoTypeLib1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"


#define ECO_EXAMPLE_PATH       "main.py"
#define ECO_EXAMPLE_CLASS_NAME "EcoCalculator"

/*
 *
 * <summary>
 *   EcoMain Function
 * </summary>
 *
 * <description>
 *   EcoMain function - entry point
 * </description>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;

    /* Pointers to the system and the interface bus */
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;

    /* Pointer to the tested interfaces */
    IEcoACOM2Python* pIEcoACOM2Python = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;

    UGUID CID_EcoCalculator = {0x01, 0x10, {0x48, 0x28, 0xF6, 0x55, 0x2E, 0x45, 0x40, 0xE7, 0x81, 0x21, 0xEB, 0xD2, 0x20, 0xDC, 0x36, 0x0E}};

    /* System interface check and creation */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            goto Release;
        }
    }

    /* Getting the interface for working with the interface bus */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }

#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoACOM2Python, (IEcoUnknown*)GetIEcoComponentFactoryPtr_219EDB626EF14B42BE16F93A566F1CC3);
    if (result != 0) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0) {
        goto Release;
    }
#endif

    /* Getting the tested interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoACOM2Python, 0, &IID_IEcoACOM2Python, (void**) &pIEcoACOM2Python);
    if (result != 0 || pIEcoACOM2Python == 0) {
        goto Release;
    }

    /* Register the calculator component */
    result = pIEcoACOM2Python->pVTbl->RegisterComponent(pIEcoACOM2Python, ECO_EXAMPLE_PATH, ECO_EXAMPLE_CLASS_NAME, &CID_EcoCalculator);
    if (result != 0) {
        goto Release;
    }

    /* Query the X interface from the bridge to test QueryComponent */
    result = pIEcoACOM2Python->pVTbl->QueryComponent(pIEcoACOM2Python, &CID_EcoCalculator, 0, &IID_IEcoCalculatorX, (void**) &pIX);
    if (result != 0 || pIX == 0) {
        goto Release;
    }

    /* Query the Y interface from the X interface to test QueryInterface */
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**) &pIY);
    if (result != 0 || pIY == 0) {
        goto Release;
    }

    printf("10 + 10 = %d\n", pIX->pVTbl->Addition(pIX, 10, 10));
    printf("30 - 45 = %d\n", pIX->pVTbl->Subtraction(pIX, 30, 45));
    printf("5 * 8 = %d\n", pIY->pVTbl->Multiplication(pIY, 5, 8));
    printf("42 / 7 = %d\n", pIY->pVTbl->Division(pIY, 42, 7));

Release:

    if (pIBus != 0) pIBus->pVTbl->Release(pIBus);
    if (pIX != 0) pIX->pVTbl->Release(pIX);
    if (pIY != 0) pIY->pVTbl->Release(pIY);
    if (pIEcoACOM2Python != 0) pIEcoACOM2Python->pVTbl->Release(pIEcoACOM2Python);
    if (pISys != 0) pISys->pVTbl->Release(pISys);

    return result;
}
