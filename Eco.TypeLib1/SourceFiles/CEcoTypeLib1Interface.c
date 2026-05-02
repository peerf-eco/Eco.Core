/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Interface_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1Interface_01434A0B
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
#include "CEcoTypeLib1Interface.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoInterfaceDescriptor1 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_QueryInterface(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoInterfaceDescriptor1) ) {
        *ppv = &pCMe->m_pVTblIInterface;
        pCMe->m_pVTblIInterface->AddRef((IEcoInterfaceDescriptor1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIInterface;
        pCMe->m_pVTblIInterface->AddRef((IEcoInterfaceDescriptor1*)pCMe);
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
 *   AddRef function for the IEcoInterfaceDescriptor1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_AddRef(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    /* Pointer Validation */
    if ( me == 0 ) {
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
 *   Release function for the IEcoInterfaceDescriptor1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_Release(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    /* Pointer Validation */
    if ( me == 0 ) {
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
 *   set_ParentIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_set_ParentIndex(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t parentIndex) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_ParentIndex = parentIndex;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_ParentIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_ParentIndex(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_ParentIndex;
}

/*
 *
 * <summary>
 *   set_Flags Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_set_Flags(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint8_t flags) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Flags = flags;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_Flags Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint8_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_Flags(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_Flags;
}

/*
 *
 * <summary>
 *   AddMethod Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_AddMethod(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ struct IEcoMethodDescriptor1* pIMethod) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_pIMethodList->pVTbl->Add(pCMe->m_pIMethodList, (void*)pIMethod);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_MethodCount Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_MethodCount(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIMethodList->pVTbl->Count(pCMe->m_pIMethodList);
}

/*
 *
 * <summary>
 *   get_MethodAtIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_MethodAtIndex(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t index, /* out */ struct IEcoMethodDescriptor1** ppIMethod) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0 || ppIMethod == 0) {
        return ERR_ECO_POINTER;
    }

    *ppIMethod = (struct IEcoMethodDescriptor1*) pCMe->m_pIMethodList->pVTbl->Item(pCMe->m_pIMethodList, index);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   AddConstant Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_AddConstant(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ struct IEcoConstantDescriptor1* pIConstant) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_pIConstantList->pVTbl->Add(pCMe->m_pIConstantList, (void*)pIConstant);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_ConstantCount Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_ConstantCount(/* in */ IEcoInterfaceDescriptor1Ptr_t me) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIConstantList->pVTbl->Count(pCMe->m_pIConstantList);
}

/*
 *
 * <summary>
 *   get_ConstantAtIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Interface_01434A0B_get_ConstantAtIndex(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t index, /* out */ struct IEcoConstantDescriptor1** ppIConstant) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;

    if (me == 0 || ppIConstant == 0) {
        return ERR_ECO_POINTER;
    }

    *ppIConstant = (struct IEcoConstantDescriptor1*) pCMe->m_pIConstantList->pVTbl->Item(pCMe->m_pIConstantList, index);
    return ERR_ECO_SUCCESES;
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1Interface_01434A0B(/*in*/ CEcoTypeLib1Interface_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1Interface_01434A0B* pCMe = (CEcoTypeLib1Interface_01434A0B*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;	

    /* Pointer Validation */
    if ( me == 0 ) {
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

    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_pIMethodList);
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_pIConstantList);


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
static int16_t ECOCALLMETHOD createCEcoTypeLib1Interface_01434A0B(/* in */ CEcoTypeLib1Interface_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1Interface_01434A0B(/* in */ CEcoTypeLib1Interface_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if ( pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIMethodList != 0 ) {
            pCMe->m_pIMethodList->pVTbl->Clear(pCMe->m_pIMethodList);
            pCMe->m_pIMethodList->pVTbl->Release(pCMe->m_pIMethodList);
        }
        if ( pCMe->m_pIConstantList != 0 ) {
            pCMe->m_pIConstantList->pVTbl->Clear(pCMe->m_pIConstantList);
            pCMe->m_pIConstantList->pVTbl->Release(pCMe->m_pIConstantList);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoInterfaceDescriptor1 Virtual Table */
IEcoInterfaceDescriptor1VTbl g_x5D9AE041C0AB482C935FA35F0F07B0F1VTbl_01434A0B = {
    CEcoTypeLib1Interface_01434A0B_QueryInterface,
    CEcoTypeLib1Interface_01434A0B_AddRef,
    CEcoTypeLib1Interface_01434A0B_Release,
    CEcoTypeLib1Interface_01434A0B_set_ParentIndex,
    CEcoTypeLib1Interface_01434A0B_get_ParentIndex,
    CEcoTypeLib1Interface_01434A0B_set_Flags,
    CEcoTypeLib1Interface_01434A0B_get_Flags,
    CEcoTypeLib1Interface_01434A0B_AddMethod,
    CEcoTypeLib1Interface_01434A0B_get_MethodCount,
    CEcoTypeLib1Interface_01434A0B_get_MethodAtIndex,
    CEcoTypeLib1Interface_01434A0B_AddConstant,
    CEcoTypeLib1Interface_01434A0B_get_ConstantCount,
    CEcoTypeLib1Interface_01434A0B_get_ConstantAtIndex
};



/* Object Instance */
CEcoTypeLib1Interface_01434A0B g_xCEcoTypeLib1Interface_01434A0B = {
    &g_x5D9AE041C0AB482C935FA35F0F07B0F1VTbl_01434A0B,
    initCEcoTypeLib1Interface_01434A0B,
    createCEcoTypeLib1Interface_01434A0B,
    deleteCEcoTypeLib1Interface_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0, /* m_ParentIndex */
    0, /* m_Flags */
    0, /* m_pIMethodList */
    0  /* m_pIConstantList */
};
