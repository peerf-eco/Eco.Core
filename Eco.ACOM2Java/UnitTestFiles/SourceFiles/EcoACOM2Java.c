/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoACOM2Java
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


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoACOM2Java.h"
#include "IdEcoList1.h"
#include "IdEcoTypeLib1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"

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
    /* Pointer to the system interface */
    IEcoSystem1* pISys = 0;
    /* Pointer to the interface for working with the system interface bus */
    IEcoInterfaceBus1* pIBus = 0;
    /* Pointer to the memory management interface */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Pointer to the tested interface */
    IEcoACOM2Java* pIEcoACOM2Java = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    UGUID CID_EcoCalculatorJ = {0x01, 0x10, {0x0A, 0x7B, 0x93, 0x7E, 0xD5, 0x0A, 0xC6, 0xC9, 0x4D, 0xFD, 0xA1, 0x28, 0x6B, 0xFD, 0xA9, 0xDC}};

    /* System interface check and creation */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Free the system interface in case of an error */
            goto Release;
        }
    }

    /* Getting the interface for working with the interface bus */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Free in case of an error */
        goto Release;
    }
#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoACOM2Java, (IEcoUnknown*)GetIEcoComponentFactoryPtr_C8035C9741CB40F9B104A12C3F41E2AA);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
#endif
    /* Getting the memory management interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Check */
    if (result != 0 || pIMem == 0) {
        /* Free the system interface in case of an error */
        goto Release;
    }

    /* Getting the tested interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoACOM2Java, 0, &IID_IEcoACOM2Java, (void**) &pIEcoACOM2Java);
    if (result != 0 || pIEcoACOM2Java == 0) {
        /* Free interfaces in case of an error */
        goto Release;
    }

    result = pIEcoACOM2Java->pVTbl->RegisterComponent(pIEcoACOM2Java, "C:\\Programming\\Eco.Core\\Eco.Java2ACOM\\BuildFiles\\production\\UnitTestFiles", "Eco/Calculator/CEcoCalculatorJ", &CID_EcoCalculatorJ);
    if (result != 0) {
        goto Release;
    }

    result = pIEcoACOM2Java->pVTbl->QueryComponent(pIEcoACOM2Java, &CID_EcoCalculatorJ, 0, &IID_IEcoCalculatorX, (void**) &pIX);
    if (result != 0) {
        goto Release;
    }

    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**) &pIY);
    if (result != 0) {
        goto Release;
    }

    printf("9 + 10 = %d\n", pIX->pVTbl->Addition(pIX, 9, 10));
    printf("67 - 13 = %d\n", pIX->pVTbl->Subtraction(pIX, 67, 13));
    printf("6 * 8 = %d\n", pIY->pVTbl->Multiplication(pIY, 6, 8));
    printf("42 / 7 = %d\n", pIY->pVTbl->Division(pIY, 42, 7));

Release:

    /* Free the interface for working with the interface bus */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Free the memory management interface */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Free the tested interfaces */
    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }
    if (pIEcoACOM2Java != 0) {
        pIEcoACOM2Java->pVTbl->Release(pIEcoACOM2Java);
    }

    /* Free the system interface */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}
