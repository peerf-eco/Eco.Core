/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1File_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1File_01434A0B
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
#include "CEcoTypeLib1File.h"

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoTypeLib1File interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_QueryInterface(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoTypeLib1File) ) {
        *ppv = &pCMe->m_pVTblIFile;
        pCMe->m_pVTblIFile->AddRef((IEcoTypeLib1File*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIFile;
        pCMe->m_pVTblIFile->AddRef((IEcoTypeLib1File*)pCMe);
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
 *   AddRef function for the IEcoTypeLib1File interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_AddRef(/* in */ IEcoTypeLib1FilePtr_t me) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

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
 *   Release function for the IEcoTypeLib1File interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_Release(/* in */ IEcoTypeLib1FilePtr_t me) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

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
 *   Open Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_Open(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ const char_t* path, /* in */ uint32_t mode) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    if (mode == ECO_OPEN_MODE_MEMORY) {
        pCMe->m_pBuffer = (void*)path;
        pCMe->m_Alloc = 0;
        pCMe->m_Size = 0;
        pCMe->m_Capacity = 0;
        return ERR_ECO_SUCCESES;
    }

    pCMe->m_pIFile = pCMe->m_pIFileMgr->pVTbl->Open(pCMe->m_pIFileMgr, path);
    if (pCMe->m_pIFile == 0) {
        if (mode & ECO_OPEN_MODE_READ_FILE) {
            return ERR_ECO_FAIL;
        }
        pCMe->m_pIFile = pCMe->m_pIFileMgr->pVTbl->Create(pCMe->m_pIFileMgr, path);
    }
    pCMe->m_Alloc = 1;

    if (mode & ECO_OPEN_MODE_READ_FILE) {
        pCMe->m_Size = pCMe->m_pIFile->pVTbl->get_Size(pCMe->m_pIFile);
        pCMe->m_Capacity = pCMe->m_Size;
        pCMe->m_pBuffer = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, pCMe->m_Size);
        pCMe->m_pIFile->pVTbl->Read(pCMe->m_pIFile, pCMe->m_pBuffer, &pCMe->m_Size);
    } else {
        pCMe->m_pBuffer = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, 3);
        pCMe->m_Size = 3;
        pCMe->m_Capacity = 3;
    }
    if ((mode & ECO_OPEN_MODE_WRITE_FILE) == 0) {
        pCMe->m_pIFile->pVTbl->Close(pCMe->m_pIFile);
        pCMe->m_pIFile = 0;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   Close Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_Close(/* in */ IEcoTypeLib1FilePtr_t me) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIFile != 0) {
        pCMe->m_pIFile->pVTbl->Write(pCMe->m_pIFile, pCMe->m_pBuffer, &pCMe->m_Size);
        pCMe->m_pIFile->pVTbl->Close(pCMe->m_pIFile);
    }
    if (pCMe->m_Alloc != 0) {
        pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, pCMe->m_pBuffer);
        pCMe->m_Alloc = 0;
    }
    pCMe->m_pBuffer = 0;
    pCMe->m_Size = 0;
    pCMe->m_Capacity = 0;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   Read Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_Read(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* in */ uint32_t size, /* out */ voidptr_t buffer) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || buffer == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_Size < offset + size) {
        return ERR_ECO_FAIL;
    }
    pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, buffer, (void*)((char_t*)pCMe->m_pBuffer + offset), size);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   ReadString Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_ReadString(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* out */ uint32_t* pSize, /* out */ char_t** ppBuffer) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;
    char_t* pSrc = 0;

    if (me == 0 || pSize == 0 || ppBuffer == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    pSrc = (char_t*)pCMe->m_pBuffer + offset;
    *pSize = strlen(pSrc) + 1;
    if (pCMe->m_Size < offset + *pSize) {
        return ERR_ECO_FAIL;
    }

    *ppBuffer = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, *pSize);
    pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, (void*)(*ppBuffer), (void*)pSrc, *pSize);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   Write Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_Write(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* in */ uint32_t size, /* in */ voidptr_t buffer) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || buffer == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_Capacity < offset + size) {
        pCMe->m_Capacity = pCMe->m_Capacity * 2 + 1;
        if (pCMe->m_Capacity < offset + size) {
            pCMe->m_Capacity = offset + size;
        }
        pCMe->m_pBuffer = pCMe->m_pIMem->pVTbl->Realloc(pCMe->m_pIMem, pCMe->m_pBuffer, pCMe->m_Capacity);
    }
    pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, (void*)((char_t*)pCMe->m_pBuffer + offset), buffer, size);
    if (pCMe->m_Size < offset + size) {
        pCMe->m_Size = offset + size;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   WriteString Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_WriteString(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* out */ uint32_t* pSize, /* in */ char_t* buffer) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || buffer == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    *pSize = strlen(buffer) + 1;
    return me->pVTbl->Write(me, offset, *pSize, (void*)buffer);
}

/*
 *
 * <summary>
 *   GetSize Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_GetSize(/* in */ IEcoTypeLib1FilePtr_t me) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_Size;
}

/*
 *
 * <summary>
 *   SetSize Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1File_01434A0B_SetSize(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t size) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;

    if (me == 0 || pCMe->m_pBuffer == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Size = size;
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1File_01434A0B(/*in*/ CEcoTypeLib1File_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1File_01434A0B* pCMe = (CEcoTypeLib1File_01434A0B*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoFileSystemManagement1* pIFileSysManagement = 0;
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

    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoFileSystemManagement1, 0, &IID_IEcoFileSystemManagement1, (void**) &pIFileSysManagement);
    pCMe->m_pIFileMgr = pIFileSysManagement->pVTbl->get_FileManager(pIFileSysManagement);
    pIFileSysManagement->pVTbl->Release(pIFileSysManagement);


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
static int16_t ECOCALLMETHOD createCEcoTypeLib1File_01434A0B(/* in */ CEcoTypeLib1File_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1File_01434A0B(/* in */ CEcoTypeLib1File_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if ( pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIFileMgr != 0 ) {
            pCMe->m_pIFileMgr->pVTbl->Release(pCMe->m_pIFileMgr);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoTypeLib1File Virtual Table */
IEcoTypeLib1FileVTbl g_x5517C25AA6C8477DB23728583771EA55VTbl_01434A0B = {
    CEcoTypeLib1File_01434A0B_QueryInterface,
    CEcoTypeLib1File_01434A0B_AddRef,
    CEcoTypeLib1File_01434A0B_Release,
    CEcoTypeLib1File_01434A0B_Open,
    CEcoTypeLib1File_01434A0B_Close,
    CEcoTypeLib1File_01434A0B_Read,
    CEcoTypeLib1File_01434A0B_ReadString,
    CEcoTypeLib1File_01434A0B_Write,
    CEcoTypeLib1File_01434A0B_WriteString,
    CEcoTypeLib1File_01434A0B_GetSize,
    CEcoTypeLib1File_01434A0B_SetSize
};



/* Object Instance */
CEcoTypeLib1File_01434A0B g_xCEcoTypeLib1File_01434A0B = {
    &g_x5517C25AA6C8477DB23728583771EA55VTbl_01434A0B,
    initCEcoTypeLib1File_01434A0B,
    createCEcoTypeLib1File_01434A0B,
    deleteCEcoTypeLib1File_01434A0B,
    1, /* m_cRef */
    0, /* m_pIMem */
    0, /* m_pISys */
    0, /* m_pIFileMgr */
    0, /* m_pIFile */
    0, /* m_pBuffer */
    0, /* m_Alloc */
    0, /* m_Size */
    0  /* m_Capacity */
};
