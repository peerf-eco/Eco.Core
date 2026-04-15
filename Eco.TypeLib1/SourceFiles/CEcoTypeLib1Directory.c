/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Directory_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1Directory_01434A0B
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */


#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoTypeLib1Directory.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoInterfaceDirectory1 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_QueryInterface(/* in */ IEcoInterfaceDirectory1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoInterfaceDirectory1) ) {
        *ppv = &pCMe->m_pVTblIDirectory;
        pCMe->m_pVTblIDirectory->AddRef((IEcoInterfaceDirectory1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIDirectory;
        pCMe->m_pVTblIDirectory->AddRef((IEcoInterfaceDirectory1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoInterfaceDirectory1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_AddRef(/* in */ IEcoInterfaceDirectory1Ptr_t me) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoInterfaceDirectory1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_Release(/* in */ IEcoInterfaceDirectory1Ptr_t me) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    --pCMe->m_cRef;
    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
		
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   AddEntry Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_AddEntry(/* in */ IEcoInterfaceDirectory1Ptr_t me, /* in */ uint16_t index, /* in */ struct IEcoInterfaceDirectoryEntry1* pIEntry) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (index == (uint16_t)-1) {
        pCMe->m_pIEntryList->pVTbl->Add(pCMe->m_pIEntryList, (void*)pIEntry);
        return ERR_ECO_SUCCESES;
    }

    pCMe->m_pIEntryList->pVTbl->InsertAt(pCMe->m_pIEntryList, index, (void*)pIEntry);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_NumInterfaces Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint16_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_get_NumInterfaces(/* in */ IEcoInterfaceDirectory1Ptr_t me) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIEntryList->pVTbl->Count(pCMe->m_pIEntryList);
}

/*
 *
 * <summary>
 *   GetEntryAtIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_GetEntryAtIndex(/* in */ IEcoInterfaceDirectory1Ptr_t me, /* in */ uint16_t index, /* out */ IEcoInterfaceDirectoryEntry1** ppEntry) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;

    if (me == 0 || ppEntry == 0) {
        return ERR_ECO_POINTER;
    }

    *ppEntry = (IEcoInterfaceDirectoryEntry1*) pCMe->m_pIEntryList->pVTbl->Item(pCMe->m_pIEntryList, index);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   GetEntryByIID Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Directory_01434A0B_GetEntryByIID(/* in */ IEcoInterfaceDirectory1Ptr_t me, /* in */ const UGUID* riid, /* out */ IEcoInterfaceDirectoryEntry1** ppEntry) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;
    uint32_t count = 0;
    uint32_t index = 0;
    UGUID iid = {0};

    if (me == 0 || ppEntry == 0) {
        return ERR_ECO_POINTER;
    }

    count = pCMe->m_pIEntryList->pVTbl->Count(pCMe->m_pIEntryList);
    while (index < count) {
        *ppEntry = (IEcoInterfaceDirectoryEntry1*) pCMe->m_pIEntryList->pVTbl->Item(pCMe->m_pIEntryList, index);
        (*ppEntry)->pVTbl->get_IID(*ppEntry, &iid);
        if (IsEqualUGUID(riid, &iid)) {
            return ERR_ECO_SUCCESES;
        }
    }
    ppEntry = 0;
    return ERR_ECO_FAIL;
}




/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoTypeLib1Directory_01434A0B(/*in*/ CEcoTypeLib1Directory_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1Directory_01434A0B* pCMe = (CEcoTypeLib1Directory_01434A0B*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;	

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_pIEntryList);



    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoTypeLib1Directory_01434A0B(/* in */ CEcoTypeLib1Directory_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }


    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   Delete Function
 * </summary>
 *
 * <description>
 *   Instance freeing function
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoTypeLib1Directory_01434A0B(/* in */ CEcoTypeLib1Directory_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIEntryList != 0 ) {
            pCMe->m_pIEntryList->pVTbl->Clear(pCMe->m_pIEntryList);
            pCMe->m_pIEntryList->pVTbl->Release(pCMe->m_pIEntryList);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoInterfaceDirectory1 Virtual Table */
IEcoInterfaceDirectory1VTbl g_x9E7246B89BCB46A2A016B92DC3E97F27VTbl_01434A0B = {
    CEcoTypeLib1Directory_01434A0B_QueryInterface,
    CEcoTypeLib1Directory_01434A0B_AddRef,
    CEcoTypeLib1Directory_01434A0B_Release,
    CEcoTypeLib1Directory_01434A0B_AddEntry,
    CEcoTypeLib1Directory_01434A0B_get_NumInterfaces,
    CEcoTypeLib1Directory_01434A0B_GetEntryAtIndex,
    CEcoTypeLib1Directory_01434A0B_GetEntryByIID
};



/* Object Instance */
CEcoTypeLib1Directory_01434A0B g_xCEcoTypeLib1Directory_01434A0B = {
    &g_x9E7246B89BCB46A2A016B92DC3E97F27VTbl_01434A0B,
    initCEcoTypeLib1Directory_01434A0B,
    createCEcoTypeLib1Directory_01434A0B,
    deleteCEcoTypeLib1Directory_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0  /* m_pIEntryList */
};
