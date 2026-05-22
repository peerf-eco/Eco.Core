/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Builder_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1Builder_01434A0B
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
#include "CEcoTypeLib1Builder.h"
#include "CEcoTypeLib1Parameter.h"
#include "CEcoTypeLib1Method.h"
#include "CEcoTypeLib1Const.h"
#include "CEcoTypeLib1Annotation.h"
#include "CEcoTypeLib1Interface.h"
#include "CEcoTypeLib1DirectoryEntry.h"
#include "CEcoTypeLib1Directory.h"

/* Resolved at compile time to 4 on 32-bit and 8 on 64-bit targets. */
#define ECO_PTR_SIZE (sizeof(void*))

const uint32_t ECO_TYPE_SIZE[] = {
    0,             /* ECO_TYPE_UNDEFINED */
    1,             /* ECO_TYPE_INT8 */
    2,             /* ECO_TYPE_INT16 */
    4,             /* ECO_TYPE_INT32 */
    8,             /* ECO_TYPE_INT64 */
    1,             /* ECO_TYPE_UINT8 */
    2,             /* ECO_TYPE_UINT16 */
    4,             /* ECO_TYPE_UINT32 */
    8,             /* ECO_TYPE_UINT64 */
    4,             /* ECO_TYPE_FLOAT */
    8,             /* ECO_TYPE_DOUBLE */
    1,             /* ECO_TYPE_BOOLEAN */
    1,             /* ECO_TYPE_CHAR */
    2,             /* ECO_TYPE_WCHAR */
    ECO_PTR_SIZE,  /* ECO_TYPE_ASTRING */
    ECO_PTR_SIZE,  /* ECO_TYPE_WSTRING */
    ECO_PTR_SIZE,  /* ECO_TYPE_INTERFACE — IEcoUnknown* */
    ECO_PTR_SIZE,  /* ECO_TYPE_UGUID — UGUID* */
    ECO_PTR_SIZE,  /* ECO_TYPE_VOIDPTR — void* */
    0              /* ECO_TYPE_VOID */
};

extern CEcoTypeLib1Parameter_01434A0B g_xCEcoTypeLib1Parameter_01434A0B;
extern CEcoTypeLib1Method_01434A0B g_xCEcoTypeLib1Method_01434A0B;
extern CEcoTypeLib1Const_01434A0B g_xCEcoTypeLib1Const_01434A0B;
extern CEcoTypeLib1Annotation_01434A0B g_xCEcoTypeLib1Annotation_01434A0B;
extern CEcoTypeLib1Interface_01434A0B g_xCEcoTypeLib1Interface_01434A0B;
extern CEcoTypeLib1DirectoryEntry_01434A0B g_xCEcoTypeLib1DirectoryEntry_01434A0B;
extern CEcoTypeLib1Directory_01434A0B g_xCEcoTypeLib1Directory_01434A0B;

static int16_t AllocEcoTypeLib1Parameter(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoParamDescriptor1** ppIParam) {
    CEcoTypeLib1Parameter_01434A0B* pCParam = (CEcoTypeLib1Parameter_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Parameter_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCParam, &g_xCEcoTypeLib1Parameter_01434A0B, sizeof(CEcoTypeLib1Parameter_01434A0B));

    result = pCParam->Create(pCParam, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCParam->Init(pCParam, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIParam = (IEcoParamDescriptor1*) pCParam;
    return result;
}

static int16_t AllocEcoTypeLib1Method(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoMethodDescriptor1** ppIMethod) {
    CEcoTypeLib1Method_01434A0B* pCMethod = (CEcoTypeLib1Method_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Method_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCMethod, &g_xCEcoTypeLib1Method_01434A0B, sizeof(CEcoTypeLib1Method_01434A0B));

    result = pCMethod->Create(pCMethod, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCMethod->Init(pCMethod, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIMethod = (IEcoMethodDescriptor1*) pCMethod;
    return result;
}

static int16_t AllocEcoTypeLib1Const(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoConstDescriptor1** ppIConst) {
    CEcoTypeLib1Const_01434A0B* pCConst = (CEcoTypeLib1Const_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Const_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCConst, &g_xCEcoTypeLib1Const_01434A0B, sizeof(CEcoTypeLib1Const_01434A0B));

    result = pCConst->Create(pCConst, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCConst->Init(pCConst, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIConst = (IEcoConstDescriptor1*) pCConst;
    return result;
}

static int16_t AllocEcoTypeLib1Annotation(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoAnnotationDescriptor1** ppIAnnotation) {
    CEcoTypeLib1Annotation_01434A0B* pCAnnotation = (CEcoTypeLib1Annotation_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Annotation_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCAnnotation, &g_xCEcoTypeLib1Annotation_01434A0B, sizeof(CEcoTypeLib1Annotation_01434A0B));

    result = pCAnnotation->Create(pCAnnotation, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCAnnotation->Init(pCAnnotation, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIAnnotation = (IEcoAnnotationDescriptor1*) pCAnnotation;
    return result;
}

static int16_t AllocEcoTypeLib1Interface(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoInterfaceDescriptor1** ppIDesc) {
    CEcoTypeLib1Interface_01434A0B* pCDesc = (CEcoTypeLib1Interface_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Interface_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCDesc, &g_xCEcoTypeLib1Interface_01434A0B, sizeof(CEcoTypeLib1Interface_01434A0B));

    result = pCDesc->Create(pCDesc, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCDesc->Init(pCDesc, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIDesc = (IEcoInterfaceDescriptor1*) pCDesc;
    return result;
}

static int16_t AllocEcoTypeLib1DirectoryEntry(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoInterfaceDirectoryEntry1** ppIEntry) {
    CEcoTypeLib1DirectoryEntry_01434A0B* pCEntry = (CEcoTypeLib1DirectoryEntry_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1DirectoryEntry_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCEntry, &g_xCEcoTypeLib1DirectoryEntry_01434A0B, sizeof(CEcoTypeLib1DirectoryEntry_01434A0B));

    result = pCEntry->Create(pCEntry, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCEntry->Init(pCEntry, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIEntry = (IEcoInterfaceDirectoryEntry1*) pCEntry;
    return result;
}

static int16_t AllocEcoTypeLib1Directory(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoInterfaceDirectory1** ppIDirectory) {
    CEcoTypeLib1Directory_01434A0B* pCDirectory = (CEcoTypeLib1Directory_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Directory_01434A0B));
    int16_t result = 0;

    pIMem->pVTbl->Copy(pIMem, (void*)pCDirectory, &g_xCEcoTypeLib1Directory_01434A0B, sizeof(CEcoTypeLib1Directory_01434A0B));

    result = pCDirectory->Create(pCDirectory, pIUnkSystem, 0);
    if (result != 0) {
        return result;
    }

    result = pCDirectory->Init(pCDirectory, pIUnkSystem);
    if (result != 0) {
        return result;
    }

    *ppIDirectory = (IEcoInterfaceDirectory1*) pCDirectory;
    return result;
}

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoTypeLib1Builder interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_QueryInterface(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoTypeLib1Builder) ) {
        *ppv = &pCMe->m_pVTblIBuilder;
        pCMe->m_pVTblIBuilder->AddRef((IEcoTypeLib1Builder*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIBuilder;
        pCMe->m_pVTblIBuilder->AddRef((IEcoTypeLib1Builder*)pCMe);
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
 *   AddRef function for the IEcoTypeLib1Builder interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_AddRef(/* in */ IEcoTypeLib1BuilderPtr_t me) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;

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
 *   Release function for the IEcoTypeLib1Builder interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_Release(/* in */ IEcoTypeLib1BuilderPtr_t me) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;

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
 *   CreateParameter Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateParameter(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const char_t* name, /* in */ uint16_t typeTag, /* in */ uint8_t flags, /* out */ struct IEcoParamDescriptor1** ppIParam) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIParam == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1Parameter(pCMe->m_pIMem, pCMe->m_pISys, ppIParam);
    if (result != 0) {
        return result;
    }

    result = (*ppIParam)->pVTbl->set_Name(*ppIParam, name);
    if (result != 0) {
        return result;
    }

    result = (*ppIParam)->pVTbl->set_Type(*ppIParam, typeTag);
    if (result != 0) {
        return result;
    }

    return (*ppIParam)->pVTbl->set_Flags(*ppIParam, flags);
}

/*
 *
 * <summary>
 *   CreateMethod Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateMethod(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const char_t* name, /* in */ uint8_t flags, /* out */ struct IEcoMethodDescriptor1** ppIMethod) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIMethod == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1Method(pCMe->m_pIMem, pCMe->m_pISys, ppIMethod);
    if (result != 0) {
        return result;
    }

    result = (*ppIMethod)->pVTbl->set_Name(*ppIMethod, name);
    if (result != 0) {
        return result;
    }

    return (*ppIMethod)->pVTbl->set_Flags(*ppIMethod, flags);
}

/*
 *
 * <summary>
 *   CreateConstant Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateAnnotation(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const char_t* key, /* in */ const char_t* value, /* out */ struct IEcoAnnotationDescriptor1** ppIAnnotation) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIAnnotation == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1Annotation(pCMe->m_pIMem, pCMe->m_pISys, ppIAnnotation);
    if (result != 0) {
        return result;
    }

    result = (*ppIAnnotation)->pVTbl->set_Key(*ppIAnnotation, key);
    if (result != 0) {
        return result;
    }

    return (*ppIAnnotation)->pVTbl->set_Value(*ppIAnnotation, value);
}

/*
 *
 * <summary>
 *   CreateAnnotation Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateConstant(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const char_t* name, /* in */ uint16_t typeTag, /* in */ voidptr_t value, /* out */ struct IEcoConstDescriptor1** ppIConst) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIConst == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1Const(pCMe->m_pIMem, pCMe->m_pISys, ppIConst);
    if (result != 0) {
        return result;
    }

    result = (*ppIConst)->pVTbl->set_Name(*ppIConst, name);
    if (result != 0) {
        return result;
    }

    result = (*ppIConst)->pVTbl->set_Type(*ppIConst, typeTag);
    if (result != 0) {
        return result;
    }

    return (*ppIConst)->pVTbl->set_Value(*ppIConst, ECO_TYPE_SIZE[typeTag], value);
}

/*
 *
 * <summary>
 *   CreateInterfaceDescriptor Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateInterfaceDescriptor(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ uint16_t parentIndex, /* in */ uint8_t flags, /* out */ struct IEcoInterfaceDescriptor1** ppIDesc) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIDesc == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1Interface(pCMe->m_pIMem, pCMe->m_pISys, ppIDesc);
    if (result != 0) {
        return result;
    }

    result = (*ppIDesc)->pVTbl->set_ParentIndex(*ppIDesc, parentIndex);
    if (result != 0) {
        return result;
    }

    return (*ppIDesc)->pVTbl->set_Flags(*ppIDesc, flags);
}

/*
 *
 * <summary>
 *   CreateInterfaceDirectoryEntry Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateInterfaceDirectoryEntry(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const char_t* name, /* in */ const char_t* namespace, /* in */ const UGUID* iid, /* in */ struct IEcoInterfaceDescriptor1* pIDesc, /* out */ struct IEcoInterfaceDirectoryEntry1** ppIEntry) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
    int16_t result = 0;

    if (me == 0 || ppIEntry == 0) {
        return ERR_ECO_POINTER;
    }

    result = AllocEcoTypeLib1DirectoryEntry(pCMe->m_pIMem, pCMe->m_pISys, ppIEntry);
    if (result != 0) {
        return result;
    }

    result = (*ppIEntry)->pVTbl->set_Name(*ppIEntry, name);
    if (result != 0) {
        return result;
    }

    result = (*ppIEntry)->pVTbl->set_Namespace(*ppIEntry, namespace);
    if (result != 0) {
        return result;
    }

    result = (*ppIEntry)->pVTbl->set_IID(*ppIEntry, *iid);
    if (result != 0) {
        return result;
    }

    return (*ppIEntry)->pVTbl->set_Descriptor(*ppIEntry, pIDesc);
}

/*
 *
 * <summary>
 *   CreateInterfaceDirectory Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Builder_01434A0B_CreateInterfaceDirectory(/* in */ IEcoTypeLib1BuilderPtr_t me, /* out */ struct IEcoInterfaceDirectory1** ppIDirectory) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;

    if (me == 0 || ppIDirectory == 0) {
        return ERR_ECO_POINTER;
    }

    return AllocEcoTypeLib1Directory(pCMe->m_pIMem, pCMe->m_pISys, ppIDirectory);
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1Builder_01434A0B(/*in*/ CEcoTypeLib1Builder_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1Builder_01434A0B* pCMe = (CEcoTypeLib1Builder_01434A0B*)me;
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
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pCMe->m_pIBus);

    /* Getting the component ID for working with memory */
    result = pCMe->m_pIBus->pVTbl->QueryInterface(pCMe->m_pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

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
static int16_t ECOCALLMETHOD createCEcoTypeLib1Builder_01434A0B(/* in */ CEcoTypeLib1Builder_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1Builder_01434A0B(/* in */ CEcoTypeLib1Builder_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIBus != 0 ) {
            pCMe->m_pIBus->pVTbl->Release(pCMe->m_pIBus);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoTypeLib1Builder Virtual Table */
IEcoTypeLib1BuilderVTbl g_x8B8B4F5C12BB4E448C694F35D95BABA1VTbl_01434A0B = {
    CEcoTypeLib1Builder_01434A0B_QueryInterface,
    CEcoTypeLib1Builder_01434A0B_AddRef,
    CEcoTypeLib1Builder_01434A0B_Release,
    CEcoTypeLib1Builder_01434A0B_CreateParameter,
    CEcoTypeLib1Builder_01434A0B_CreateMethod,
    CEcoTypeLib1Builder_01434A0B_CreateConstant,
    CEcoTypeLib1Builder_01434A0B_CreateAnnotation,
    CEcoTypeLib1Builder_01434A0B_CreateInterfaceDescriptor,
    CEcoTypeLib1Builder_01434A0B_CreateInterfaceDirectoryEntry,
    CEcoTypeLib1Builder_01434A0B_CreateInterfaceDirectory
};



/* Object Instance */
CEcoTypeLib1Builder_01434A0B g_xCEcoTypeLib1Builder_01434A0B = {
    &g_x8B8B4F5C12BB4E448C694F35D95BABA1VTbl_01434A0B,
    initCEcoTypeLib1Builder_01434A0B,
    createCEcoTypeLib1Builder_01434A0B,
    deleteCEcoTypeLib1Builder_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0  /* m_pIBus */
};
