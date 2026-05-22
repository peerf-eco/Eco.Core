/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1DirectoryEntry_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1DirectoryEntry_01434A0B
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
#include "CEcoTypeLib1DirectoryEntry.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoInterfaceDirectoryEntry1 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_QueryInterface(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoInterfaceDirectoryEntry1) ) {
        *ppv = &pCMe->m_pVTblIEntry;
        pCMe->m_pVTblIEntry->AddRef((IEcoInterfaceDirectoryEntry1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEntry;
        pCMe->m_pVTblIEntry->AddRef((IEcoInterfaceDirectoryEntry1*)pCMe);
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
 *   AddRef function for the IEcoInterfaceDirectoryEntry1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_AddRef(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

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
 *   Release function for the IEcoInterfaceDirectoryEntry1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_Release(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

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
 *   get_IID Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_get_IID(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* out */ UGUID* pIID) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    if (me == 0 || pIID == 0) {
        return ERR_ECO_POINTER;
    }

    *pIID = pCMe->m_IID;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   set_IID Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_set_IID(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ UGUID iid) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_IID = iid;
    return ERR_ECO_SUCCESES;
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
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_get_Name(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* out */ char_t** ppName) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

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
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_set_Name(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ char_t* name) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;
    uint32_t size = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_Name != 0) {
        pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, pCMe->m_Name);
        pCMe->m_Name = 0;
    }
    if (name != 0) {
        size = strlen(name) + 1;
        pCMe->m_Name = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size);
        pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, (void*)pCMe->m_Name, (void*)name, size);
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_Namespace Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_get_Namespace(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* out */ char_t** ppNamespace) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    if (me == 0 || ppNamespace == 0) {
        return ERR_ECO_POINTER;
    }

    *ppNamespace = pCMe->m_Namespace;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   set_Namespace Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_set_Namespace(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ char_t* namespace) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;
    uint32_t size = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_Namespace != 0) {
        pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, pCMe->m_Namespace);
        pCMe->m_Namespace = 0;
    }
    if (namespace != 0) {
        size = strlen(namespace) + 1;
        pCMe->m_Namespace = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, size);
        pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, (void*)pCMe->m_Namespace, (void*)namespace, size);
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   get_Descriptor Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_get_Descriptor(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* out */ IEcoInterfaceDescriptor1** ppDescriptor) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    if (me == 0 || ppDescriptor == 0) {
        return ERR_ECO_POINTER;
    }

    *ppDescriptor = pCMe->m_pIDescriptor;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   set_Descriptor Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1DirectoryEntry_01434A0B_set_Descriptor(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ IEcoInterfaceDescriptor1* pDescriptor) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIDescriptor != 0) {
        pCMe->m_pIDescriptor->pVTbl->Release(pCMe->m_pIDescriptor);
        pCMe->m_pIDescriptor = 0;
    }
    if (pDescriptor != 0) {
        pDescriptor->pVTbl->AddRef(pDescriptor);
        pCMe->m_pIDescriptor = pDescriptor;
    }
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1DirectoryEntry_01434A0B(/*in*/ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCMe = (CEcoTypeLib1DirectoryEntry_01434A0B*)me;
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
static int16_t ECOCALLMETHOD createCEcoTypeLib1DirectoryEntry_01434A0B(/* in */ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1DirectoryEntry_01434A0B(/* in */ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if ( pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIDescriptor != 0 ) {
            pCMe->m_pIDescriptor->pVTbl->Release(pCMe->m_pIDescriptor);
        }
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_Namespace != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Namespace);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoInterfaceDirectoryEntry1 Virtual Table */
IEcoInterfaceDirectoryEntry1VTbl g_xF3D1C80F34C2478F8D93EB7ED39977E5VTbl_01434A0B = {
    CEcoTypeLib1DirectoryEntry_01434A0B_QueryInterface,
    CEcoTypeLib1DirectoryEntry_01434A0B_AddRef,
    CEcoTypeLib1DirectoryEntry_01434A0B_Release,
    CEcoTypeLib1DirectoryEntry_01434A0B_get_IID,
    CEcoTypeLib1DirectoryEntry_01434A0B_set_IID,
    CEcoTypeLib1DirectoryEntry_01434A0B_get_Name,
    CEcoTypeLib1DirectoryEntry_01434A0B_set_Name,
    CEcoTypeLib1DirectoryEntry_01434A0B_get_Namespace,
    CEcoTypeLib1DirectoryEntry_01434A0B_set_Namespace,
    CEcoTypeLib1DirectoryEntry_01434A0B_get_Descriptor,
    CEcoTypeLib1DirectoryEntry_01434A0B_set_Descriptor
};



/* Object Instance */
CEcoTypeLib1DirectoryEntry_01434A0B g_xCEcoTypeLib1DirectoryEntry_01434A0B = {
    &g_xF3D1C80F34C2478F8D93EB7ED39977E5VTbl_01434A0B,
    initCEcoTypeLib1DirectoryEntry_01434A0B,
    createCEcoTypeLib1DirectoryEntry_01434A0B,
    deleteCEcoTypeLib1DirectoryEntry_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0, /* m_IID */
    0, /* m_Name */
    0, /* m_Namespace */
    0  /* m_pIDescriptor */
};
