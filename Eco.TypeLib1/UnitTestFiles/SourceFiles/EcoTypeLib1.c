/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoTypeLib1
 * </summary>
 *
 * <description>
 *   Unit-test entry point for the `Eco.TypeLib1` component.
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
#include "IdEcoTypeLib1.h"
#include "IdEcoList1.h"
#include "IEcoTest1.h"

#include "EcoTypeLib1Common.h"
#include "EcoTypeLib1InterfaceBus1.h"
#include "EcoTypeLib1Calculator.h"
#include "EcoTypeLib1List1.h"
#include "EcoTypeLib1Test1.h"

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
    IEcoSystem1* pISys = NULL;
    IEcoInterfaceBus1* pIBus = NULL;
    IEcoMemoryAllocator1* pIMem = NULL;
    IEcoTypeLib1* pTypeLib = NULL;

    /* System interface check and creation */
    if (pISys == NULL) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
        if (result != 0 && pISys == NULL) goto Release;
    }

    /* Getting the interface for working with the interface bus */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == NULL) goto Release;

#ifdef ECO_LIB
    /* Registration of static components in static-link mode */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0) goto Release;
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0) goto Release;
#endif

    /* Getting the memory management interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
    if (result != 0 || pIMem == NULL) goto Release;

    /* Getting the type-library component */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, (void**)&pTypeLib);
    if (result != 0 || pTypeLib == NULL) goto Release;

    /* Write one `.etl` per supported interface */
    SaveTypeLib_IEcoInterfaceBus1(pTypeLib);
    SaveTypeLib_IEcoCalculatorX(pTypeLib);
    SaveTypeLib_IEcoCalculatorY(pTypeLib);
    SaveTypeLib_IEcoList1(pTypeLib);
    SaveTypeLib_IEcoTest1(pTypeLib);

    /* Smoke check */
    LoadAndInspect(pTypeLib, &IID_IEcoInterfaceBus1);
    LoadAndInspect(pTypeLib, &IID_IEcoCalculatorX);
    LoadAndInspect(pTypeLib, &IID_IEcoCalculatorY);
    LoadAndInspect(pTypeLib, &IID_IEcoList1);
    LoadAndInspect(pTypeLib, &IID_IEcoTest1);

Release:

    if (pIBus != NULL) pIBus->pVTbl->Release(pIBus);
    if (pIMem != NULL) pIMem->pVTbl->Release(pIMem);
    if (pTypeLib != NULL) pTypeLib->pVTbl->Release(pTypeLib);
    if (pISys != NULL) pISys->pVTbl->Release(pISys);

    return result;
}
