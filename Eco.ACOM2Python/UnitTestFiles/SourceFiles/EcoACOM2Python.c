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


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoACOM2Python.h"

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
    char_t* name = 0;
    char_t* copyName = 0;
    /* Pointer to the tested interface */
    IEcoACOM2Python* pIEcoACOM2Python = 0;

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
    /* Registration of a static component for working with the list */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoACOM2Python, (IEcoUnknown*)GetIEcoComponentFactoryPtr_219EDB626EF14B42BE16F93A566F1CC3);
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

    /* Memory block allocation */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);

    /* Fill the memory block */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);


    /* Getting the tested interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoACOM2Python, 0, &IID_IEcoACOM2Python, (void**) &pIEcoACOM2Python);
    if (result != 0 || pIEcoACOM2Python == 0) {
        /* Free interfaces in case of an error */
        goto Release;
    }


    result = pIEcoACOM2Python->pVTbl->MyFunction(pIEcoACOM2Python, name, &copyName);


    /* Free the memory block */
    pIMem->pVTbl->Free(pIMem, name);

Release:

    /* Free the interface for working with the interface bus */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Free the memory management interface */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Free the tested interface */
    if (pIEcoACOM2Python != 0) {
        pIEcoACOM2Python->pVTbl->Release(pIEcoACOM2Python);
    }


    /* Free the system interface */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}
