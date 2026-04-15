/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Method_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1Method_01434A0B
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
#include "CEcoTypeLib1Method.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoMethodDescriptor1 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_QueryInterface(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoMethodDescriptor1) ) {
        *ppv = &pCMe->m_pVTblIMethod;
        pCMe->m_pVTblIMethod->AddRef((IEcoMethodDescriptor1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIMethod;
        pCMe->m_pVTblIMethod->AddRef((IEcoMethodDescriptor1*)pCMe);
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
 *   AddRef function for the IEcoMethodDescriptor1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_AddRef(/* in */ IEcoMethodDescriptor1Ptr_t me) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

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
 *   Release function for the IEcoMethodDescriptor1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_Release(/* in */ IEcoMethodDescriptor1Ptr_t me) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

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
 *   get_Name Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_get_Name(/* in */ IEcoMethodDescriptor1Ptr_t me, /* out */ char_t** ppName) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0 || ppName == 0) {
        return ERR_ECO_POINTER;
    }

    *ppName = pCMe->m_Name;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   set_Name Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_set_Name(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ char_t* name) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Name = name;
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
static uint8_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_get_Flags(/* in */ IEcoMethodDescriptor1Ptr_t me) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_Flags;
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
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_set_Flags(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ uint8_t flags) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Flags = flags;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   AddParameter Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_AddParameter(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ struct IEcoParamDescriptor1* pIParam) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_pIParamList->pVTbl->Add(pCMe->m_pIParamList, (void*)pIParam);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_ParamCount Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint8_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_get_ParamCount(/* in */ IEcoMethodDescriptor1Ptr_t me) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIParamList->pVTbl->Count(pCMe->m_pIParamList);
}

/*
 *
 * <summary>
 *   GetParamAtIndex Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_GetParamAtIndex(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ uint8_t index, /* out */ struct IEcoParamDescriptor1** ppIParam) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0 || ppIParam == 0) {
        return ERR_ECO_POINTER;
    }

    *ppIParam = (struct IEcoParamDescriptor1*) pCMe->m_pIParamList->pVTbl->Item(pCMe->m_pIParamList, index);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   set_Result Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_set_Result(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ struct IEcoParamDescriptor1* pIParam) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_pIParamResult = pIParam;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_Result Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Method_01434A0B_get_Result(/* in */ IEcoMethodDescriptor1Ptr_t me, /* out */ struct IEcoParamDescriptor1** ppIParam) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;

    if (me == 0 || ppIParam == 0) {
        return ERR_ECO_POINTER;
    }

    *ppIParam = pCMe->m_pIParamResult;
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1Method_01434A0B(/*in*/ CEcoTypeLib1Method_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1Method_01434A0B* pCMe = (CEcoTypeLib1Method_01434A0B*)me;
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

    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_pIParamList);



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
static int16_t ECOCALLMETHOD createCEcoTypeLib1Method_01434A0B(/* in */ CEcoTypeLib1Method_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1Method_01434A0B(/* in */ CEcoTypeLib1Method_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIParamList != 0 ) {
            pCMe->m_pIParamList->pVTbl->Clear(pCMe->m_pIParamList);
            pCMe->m_pIParamList->pVTbl->Release(pCMe->m_pIParamList);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoMethodDescriptor1 Virtual Table */
IEcoMethodDescriptor1VTbl g_x74D2637AE0CE41B18876038E40CBC64EVTbl_01434A0B = {
    CEcoTypeLib1Method_01434A0B_QueryInterface,
    CEcoTypeLib1Method_01434A0B_AddRef,
    CEcoTypeLib1Method_01434A0B_Release,
    CEcoTypeLib1Method_01434A0B_get_Name,
    CEcoTypeLib1Method_01434A0B_set_Name,
    CEcoTypeLib1Method_01434A0B_get_Flags,
    CEcoTypeLib1Method_01434A0B_set_Flags,
    CEcoTypeLib1Method_01434A0B_AddParameter,
    CEcoTypeLib1Method_01434A0B_get_ParamCount,
    CEcoTypeLib1Method_01434A0B_GetParamAtIndex,
    CEcoTypeLib1Method_01434A0B_set_Result,
    CEcoTypeLib1Method_01434A0B_get_Result
};



/* Object Instance */
CEcoTypeLib1Method_01434A0B g_xCEcoTypeLib1Method_01434A0B = {
    &g_x74D2637AE0CE41B18876038E40CBC64EVTbl_01434A0B,
    initCEcoTypeLib1Method_01434A0B,
    createCEcoTypeLib1Method_01434A0B,
    deleteCEcoTypeLib1Method_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0, /* m_Name */
    0, /* m_Flags */
    0, /* m_pIParamList */
    0  /* m_pIParamResult */
};
