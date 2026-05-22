/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1_01434A0B
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoTypeLib1_01434A0B
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */


#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoTypeLib1.h"
#include "CEcoTypeLib1File.h"
#include "CEcoTypeLib1Builder.h"

extern CEcoTypeLib1File_01434A0B g_xCEcoTypeLib1File_01434A0B;
extern CEcoTypeLib1Builder_01434A0B g_xCEcoTypeLib1Builder_01434A0B;

static void AllocEcoTypeLib1File(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoTypeLib1File** ppIFile) {
    CEcoTypeLib1File_01434A0B* pCFile = (CEcoTypeLib1File_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1File_01434A0B));
    pIMem->pVTbl->Copy(pIMem, (void*)pCFile, &g_xCEcoTypeLib1File_01434A0B, sizeof(CEcoTypeLib1File_01434A0B));
    pCFile->Create(pCFile, pIUnkSystem, 0);
    pCFile->Init(pCFile, pIUnkSystem);
    *ppIFile = (IEcoTypeLib1File*) pCFile;
}

static int16_t AllocEcoTypeLib1Builder(IEcoMemoryAllocator1* pIMem, IEcoUnknown* pIUnkSystem, IEcoTypeLib1Builder** ppIBuilder) {
    CEcoTypeLib1Builder_01434A0B* pCBuilder = (CEcoTypeLib1Builder_01434A0B*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTypeLib1Builder_01434A0B));
    pIMem->pVTbl->Copy(pIMem, (void*)pCBuilder, &g_xCEcoTypeLib1Builder_01434A0B, sizeof(CEcoTypeLib1Builder_01434A0B));
    pCBuilder->Create(pCBuilder, pIUnkSystem, 0);
    pCBuilder->Init(pCBuilder, pIUnkSystem);
    *ppIBuilder = (IEcoTypeLib1Builder*) pCBuilder;
}

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoTypeLib1 interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_QueryInterface(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoTypeLib1) ) {
        *ppv = &pCMe->m_pVTblIEcoTypeLib1;
        pCMe->m_pVTblIEcoTypeLib1->AddRef((IEcoTypeLib1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoTypeLib1;
        pCMe->m_pVTblIEcoTypeLib1->AddRef((IEcoTypeLib1*)pCMe);
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
 *   AddRef function for the IEcoTypeLib1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_AddRef(/* in */ IEcoTypeLib1Ptr_t me) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    /* Pointer Validation */
    if ( me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoTypeLib1 interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_Release(/* in */ IEcoTypeLib1Ptr_t me) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    /* Pointer Validation */
    if ( me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    atomicdecrement_int32_t(&pCMe->m_cRef);
    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
		
        return 0;
    }
    return pCMe->m_cRef;
}

static int16_t LoadParameter(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, uint32_t* pOffset, IEcoParamDescriptor1** ppIParam) {
    char_t* name = 0;
    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 2, &typeTag);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, &flags);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    return pIBuilder->pVTbl->CreateParameter(pIBuilder, name, typeTag, flags, ppIParam);
}

static int16_t LoadConstant(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoConstDescriptor1** ppIConst) {
    char_t* name = 0;
    uint16_t typeTag = 0;
    uint32_t size = 0;
    voidptr_t value = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 2, &typeTag);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    size = ECO_TYPE_SIZE[typeTag];
    value = pIMem->pVTbl->Alloc(pIMem, size);
    result = pIFile->pVTbl->Read(pIFile, *pOffset, size, value);
    if (result != 0) {
        pIMem->pVTbl->Free(pIMem, value);
        return result;
    }
    *pOffset += size;

    result = pIBuilder->pVTbl->CreateConstant(pIBuilder, name, typeTag, value, ppIConst);
    pIMem->pVTbl->Free(pIMem, value);
    return result;
}

static int16_t LoadMethod(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, uint32_t* pOffset, IEcoMethodDescriptor1** ppIMethod) {
    IEcoParamDescriptor1* pIParam = 0;
    char_t* name = 0;
    uint8_t flags = 0;
    uint8_t count = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, &flags);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    result = pIBuilder->pVTbl->CreateMethod(pIBuilder, name, flags, ppIMethod);
    if (result != 0) {
        return result;
    }

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, &count);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    while (count != 0) {
        result = LoadParameter(pIFile, pIBuilder, pOffset, &pIParam);
        if (result != 0) {
            return result;
        }
        result = (*ppIMethod)->pVTbl->AddParameter(*ppIMethod, pIParam);
        if (result != 0) {
            return result;
        }
        count--;
    }

    result = LoadParameter(pIFile, pIBuilder, pOffset, &pIParam);
    if (result != 0) {
        return result;
    }

    return (*ppIMethod)->pVTbl->set_Result(*ppIMethod, pIParam);
}

static int16_t LoadAnnotation(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, uint32_t* pOffset, IEcoAnnotationDescriptor1** ppIAnnotation) {
    char_t* key = 0;
    char_t* value = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &key);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &value);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    return pIBuilder->pVTbl->CreateAnnotation(pIBuilder, key, value, ppIAnnotation);
}

static int16_t LoadInterfaceDescriptor(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoInterfaceDescriptor1** ppIDesc) {
    uint16_t parentIndex = 0;
    uint8_t flags = 0;
    uint16_t count = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 2, (void*)&parentIndex);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, (void*)&flags);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    result = pIBuilder->pVTbl->CreateInterfaceDescriptor(pIBuilder, parentIndex, flags, ppIDesc);
    if (result != 0) {
        return result;
    }

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 2, (void*)&count);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    while (count != 0) {
        IEcoMethodDescriptor1* pIMethod = 0;
        result = LoadMethod(pIFile, pIBuilder, pOffset, &pIMethod);
        if (result != 0) {
            return result;
        }
        result = (*ppIDesc)->pVTbl->AddMethod(*ppIDesc, pIMethod);
        if (result != 0) {
            return result;
        }
        count--;
    }

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 2, (void*)&count);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    while (count != 0) {
        IEcoConstDescriptor1* pIConst = 0;
        result = LoadConstant(pIFile, pIBuilder, pIMem, pOffset, &pIConst);
        if (result != 0) {
            return result;
        }
        result = (*ppIDesc)->pVTbl->AddConstant(*ppIDesc, pIConst);
        if (result != 0) {
            return result;
        }
        count--;
    }
    return result;
}

static int16_t LoadInterfaceDirectoryEntry(IEcoTypeLib1File* pIFile, IEcoTypeLib1Builder* pIBuilder, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoInterfaceDirectoryEntry1** ppIEntry) {
    IEcoInterfaceDescriptor1* pIDesc = 0;
    UGUID iid = {0};
    char_t* name = 0;
    char_t* namespace = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, (void*)&iid.Preamble);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;
    result = pIFile->pVTbl->Read(pIFile, *pOffset, 1, (void*)&iid.Length);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;
    result = pIFile->pVTbl->Read(pIFile, *pOffset, iid.Length, (void*)&iid.Data);
    if (result != 0) {
        return result;
    }
    *pOffset += iid.Length;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIFile->pVTbl->ReadString(pIFile, *pOffset, &size, &namespace);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = LoadInterfaceDescriptor(pIFile, pIBuilder, pIMem, pOffset, &pIDesc);
    if (result != 0) {
        return result;
    }

    return pIBuilder->pVTbl->CreateInterfaceDirectoryEntry(pIBuilder, name, namespace, &iid, pIDesc, ppIEntry);
}

/*
 *
 * <summary>
 *   LoadFile Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_LoadFile(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const char_t* path, /* out */ IEcoInterfaceDirectory1** ppIDirectory) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;
    uint32_t offset = 2;
    uint32_t size = 0;
    uint16_t count = 0;
    int16_t result = 0;

    if (me == 0 || ppIDirectory == 0) {
        return ERR_ECO_POINTER;
    }

    result = pCMe->m_pIFile->pVTbl->Open(pCMe->m_pIFile, path, ECO_OPEN_MODE_READ_FILE);
    if (result != 0) {
        return result;
    }

    result = pCMe->m_pIBuilder->pVTbl->CreateInterfaceDirectory(pCMe->m_pIBuilder, ppIDirectory);
    if (result != 0) {
        goto LoadRelease;
    }

    result = pCMe->m_pIFile->pVTbl->Read(pCMe->m_pIFile, 0, 2, &count);
    if (result != 0) {
        goto LoadRelease;
    }

    while (count != 0) {
        IEcoInterfaceDirectoryEntry1* pIEntry = 0;
        result = LoadInterfaceDirectoryEntry(pCMe->m_pIFile, pCMe->m_pIBuilder, pCMe->m_pIMem, &offset, &pIEntry);
        if (result != 0) {
            goto LoadRelease;
        }
        result = (*ppIDirectory)->pVTbl->AddEntry(*ppIDirectory, -1, pIEntry);
        if (result != 0) {
            goto LoadRelease;
        }
        count--;
    }

    result = pCMe->m_pIFile->pVTbl->Read(pCMe->m_pIFile, offset, 2, &count);
    if (result != 0) {
        goto LoadRelease;
    }
    offset += 2;

    while (count != 0) {
        IEcoAnnotationDescriptor1* pIAnnotation = 0;
        result = LoadAnnotation(pCMe->m_pIFile, pCMe->m_pIBuilder, &offset, &pIAnnotation);
        if (result != 0) {
            goto LoadRelease;
        }
        result = (*ppIDirectory)->pVTbl->AddAnnotation(*ppIDirectory, -1, pIAnnotation);
        if (result != 0) {
            goto LoadRelease;
        }
        count--;
    }

LoadRelease:
    pCMe->m_pIFile->pVTbl->Close(pCMe->m_pIFile);
    return result;
}

static int16_t SaveParameter(IEcoTypeLib1File* pIFile, uint32_t* pOffset, IEcoParamDescriptor1* pIParam) {
    char_t* name = 0;
    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIParam->pVTbl->get_Name(pIParam, &name);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIParam->pVTbl->get_Type(pIParam, &typeTag);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 2, &typeTag);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    flags = pIParam->pVTbl->get_Flags(pIParam);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, &flags);
    *pOffset += 1;

    return result;
}

static int16_t SaveConstant(IEcoTypeLib1File* pIFile, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoConstDescriptor1* pIConst) {
    char_t* name = 0;
    uint16_t typeTag = 0;
    uint32_t size = 0;
    voidptr_t value = 0;
    int16_t result = 0;

    result = pIConst->pVTbl->get_Name(pIConst, &name);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIConst->pVTbl->get_Type(pIConst, &typeTag);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 2, &typeTag);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    size = ECO_TYPE_SIZE[typeTag];
    value = pIMem->pVTbl->Alloc(pIMem, size);
    pIConst->pVTbl->get_Value(pIConst, &size, value);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, size, value);
    *pOffset += size;
    pIMem->pVTbl->Free(pIMem, value);
    return result;
}

static int16_t SaveMethod(IEcoTypeLib1File* pIFile, uint32_t* pOffset, IEcoMethodDescriptor1* pIMethod) {
    IEcoParamDescriptor1* pIParam = 0;
    char_t* name = 0;
    uint8_t flags = 0;
    uint8_t index = 0;
    uint8_t count = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIMethod->pVTbl->get_Name(pIMethod, &name);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    flags = pIMethod->pVTbl->get_Flags(pIMethod);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, &flags);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    count = pIMethod->pVTbl->get_ParamCount(pIMethod);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, &count);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    for (index = 0; index < count; index++) {
        result = pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        if (result != 0) {
            return result;
        }
        result = SaveParameter(pIFile, pOffset, pIParam);
        if (result != 0) {
            return result;
        }
    }

    result = pIMethod->pVTbl->get_Result(pIMethod, &pIParam);
    if (result != 0) {
        return result;
    }
    return SaveParameter(pIFile, pOffset, pIParam);
}

static int16_t SaveAnnotation(IEcoTypeLib1File* pIFile, uint32_t* pOffset, IEcoAnnotationDescriptor1* pIAnnotation) {
    char_t* string = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIAnnotation->pVTbl->get_Key(pIAnnotation, &string);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, string);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIAnnotation->pVTbl->get_Value(pIAnnotation, &string);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, string);
    *pOffset += size;

    return result;
}

static int16_t SaveInterfaceDescriptor(IEcoTypeLib1File* pIFile, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoInterfaceDescriptor1* pIDesc) {
    uint16_t parentIndex = 0;
    uint8_t flags = 0;
    uint16_t index = 0;
    uint16_t count = 0;
    int16_t result = 0;

    parentIndex = pIDesc->pVTbl->get_ParentIndex(pIDesc);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 2, (void*)&parentIndex);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    flags = pIDesc->pVTbl->get_Flags(pIDesc);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, (void*)&flags);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;

    count = pIDesc->pVTbl->get_MethodCount(pIDesc);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 2, (void*)&count);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    for (index = 0; index < count; index++) {
        IEcoMethodDescriptor1* pIMethod = 0;
        result = pIDesc->pVTbl->get_MethodAtIndex(pIDesc, index, &pIMethod);
        if (result != 0) {
            return result;
        }
        result = SaveMethod(pIFile, pOffset, pIMethod);
        if (result != 0) {
            return result;
        }
    }

    count = pIDesc->pVTbl->get_ConstantCount(pIDesc);
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 2, (void*)&count);
    if (result != 0) {
        return result;
    }
    *pOffset += 2;

    for (index = 0; index < count; index++) {
        IEcoConstDescriptor1* pIConst = 0;
        result = pIDesc->pVTbl->get_ConstantAtIndex(pIDesc, index, &pIConst);
        if (result != 0) {
            return result;
        }
        result = SaveConstant(pIFile, pIMem, pOffset, pIConst);
        if (result != 0) {
            return result;
        }
    }
    return result;
}

static int16_t SaveInterfaceDirectoryEntry(IEcoTypeLib1File* pIFile, IEcoMemoryAllocator1* pIMem, uint32_t* pOffset, IEcoInterfaceDirectoryEntry1* pIEntry) {
    IEcoInterfaceDescriptor1* pIDesc = 0;
    UGUID iid = {0};
    char_t* name = 0;
    char_t* namespace = 0;
    uint32_t size = 0;
    int16_t result = 0;

    result = pIEntry->pVTbl->get_IID(pIEntry, &iid);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, (void*)&iid.Preamble);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;
    result = pIFile->pVTbl->Write(pIFile, *pOffset, 1, (void*)&iid.Length);
    if (result != 0) {
        return result;
    }
    *pOffset += 1;
    result = pIFile->pVTbl->Write(pIFile, *pOffset, iid.Length, (void*)&iid.Data);
    if (result != 0) {
        return result;
    }
    *pOffset += iid.Length;

    result = pIEntry->pVTbl->get_Name(pIEntry, &name);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, name);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIEntry->pVTbl->get_Namespace(pIEntry, &namespace);
    if (result != 0) {
        return result;
    }
    result = pIFile->pVTbl->WriteString(pIFile, *pOffset, &size, namespace);
    if (result != 0) {
        return result;
    }
    *pOffset += size;

    result = pIEntry->pVTbl->get_Descriptor(pIEntry, &pIDesc);
    if (result != 0) {
        return result;
    }
    return SaveInterfaceDescriptor(pIFile, pIMem, pOffset, pIDesc);
}

/*
 *
 * <summary>
 *   SaveFile Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_SaveFile(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const char_t* path, /* in */ IEcoInterfaceDirectory1* pIDirectory) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;
    uint32_t offset = 2;
    uint16_t index = 0;
    uint16_t count = 0;
    int16_t result = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    result = pCMe->m_pIFile->pVTbl->Open(pCMe->m_pIFile, path, ECO_OPEN_MODE_WRITE_FILE);
    if (result != 0) {
        return result;
    }

    count = pIDirectory->pVTbl->get_NumInterfaces(pIDirectory);
    result = pCMe->m_pIFile->pVTbl->Write(pCMe->m_pIFile, 0, 2, &count);
    if (result != 0) {
        goto SaveRelease;
    }

    for (index = 0; index < count; index++) {
        IEcoInterfaceDirectoryEntry1* pIEntry = 0;
        result = pIDirectory->pVTbl->GetEntryAtIndex(pIDirectory, index, &pIEntry);
        if (result != 0) {
            goto SaveRelease;
        }
        result = SaveInterfaceDirectoryEntry(pCMe->m_pIFile, pCMe->m_pIMem, &offset, pIEntry);
        if (result != 0) {
            goto SaveRelease;
        }
    }

    count = pIDirectory->pVTbl->get_AnnotationCount(pIDirectory);
    result = pCMe->m_pIFile->pVTbl->Write(pCMe->m_pIFile, offset, 2, &count);
    if (result != 0) {
        goto SaveRelease;
    }
    offset += 2;

    for (index = 0; index < count; index++) {
        IEcoAnnotationDescriptor1* pIAnnotation = 0;
        result = pIDirectory->pVTbl->GetAnnotationAtIndex(pIDirectory, index, &pIAnnotation);
        if (result != 0) {
            goto SaveRelease;
        }
        result = SaveAnnotation(pCMe->m_pIFile, &offset, pIAnnotation);
        if (result != 0) {
            goto SaveRelease;
        }
    }

SaveRelease:
    pCMe->m_pIFile->pVTbl->Close(pCMe->m_pIFile);
    return result;
}

/*
 *
 * <summary>
 *   CreateBuilder Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_CreateBuilder(/* in */ IEcoTypeLib1Ptr_t me, /* out */ struct IEcoTypeLib1Builder** ppIBuilder) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    if (me == 0 || ppIBuilder == 0) {
        return ERR_ECO_POINTER;
    }

    AllocEcoTypeLib1Builder(pCMe->m_pIMem, pCMe->m_pISys, ppIBuilder);
    return ERR_ECO_SUCCESES;
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
static int16_t ECOCALLMETHOD CEcoTypeLib1_01434A0B_Open(/* in */ IEcoTypeLib1Ptr_t me, const char_t* path, struct IEcoTypeLib1File** ppIFile) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    if (me == 0 || ppIFile == 0) {
        return ERR_ECO_POINTER;
    }

    AllocEcoTypeLib1File(pCMe->m_pIMem, pCMe->m_pISys, ppIFile);
    return (*ppIFile)->pVTbl->Open(*ppIFile, path, ECO_OPEN_MODE_READ_FILE | ECO_OPEN_MODE_WRITE_FILE);
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
static int16_t ECOCALLMETHOD initCEcoTypeLib1_01434A0B(/*in*/ CEcoTypeLib1_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1_01434A0B* pCMe = (CEcoTypeLib1_01434A0B*)me;

    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    CEcoTypeLib1File_01434A0B* pCFile = 0;
    CEcoTypeLib1Builder_01434A0B* pCBuilder = 0;
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

    AllocEcoTypeLib1File(pCMe->m_pIMem, pIUnkSystem, &pCMe->m_pIFile);
    AllocEcoTypeLib1Builder(pCMe->m_pIMem, pIUnkSystem, &pCMe->m_pIBuilder);

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
static int16_t ECOCALLMETHOD createCEcoTypeLib1_01434A0B(/* in */ CEcoTypeLib1_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
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
static void ECOCALLMETHOD deleteCEcoTypeLib1_01434A0B(/* in */ CEcoTypeLib1_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if ( pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Freeing */
        if ( pCMe->m_pIFile != 0 ) {
            pCMe->m_pIFile->pVTbl->Release(pCMe->m_pIFile);
        }
        if ( pCMe->m_pIBuilder != 0 ) {
            pCMe->m_pIBuilder->pVTbl->Release(pCMe->m_pIBuilder);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoTypeLib1 Virtual Table */
IEcoTypeLib1VTbl g_x625A3D0EF218438FAAD3F85EB8E7E975VTbl_01434A0B = {
    CEcoTypeLib1_01434A0B_QueryInterface,
    CEcoTypeLib1_01434A0B_AddRef,
    CEcoTypeLib1_01434A0B_Release,
    CEcoTypeLib1_01434A0B_LoadFile,
    CEcoTypeLib1_01434A0B_SaveFile,
    CEcoTypeLib1_01434A0B_CreateBuilder,
    CEcoTypeLib1_01434A0B_Open
};



/* Object Instance */
CEcoTypeLib1_01434A0B g_xCEcoTypeLib1_01434A0B = {
    &g_x625A3D0EF218438FAAD3F85EB8E7E975VTbl_01434A0B,
    initCEcoTypeLib1_01434A0B,
    createCEcoTypeLib1_01434A0B,
    deleteCEcoTypeLib1_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0, /* m_pIFile */
    0  /* m_pIBuilder */
};
