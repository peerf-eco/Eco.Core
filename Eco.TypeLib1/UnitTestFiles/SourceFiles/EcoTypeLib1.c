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
#include "IdEcoTypeLib1.h"
#include "IdEcoList1.h"

void SaveListTypeLib(IEcoTypeLib1* pTypeLib, const char* fileName);
void LoadAndInspectList(IEcoTypeLib1* pTypeLib, const char* fileName);

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
    IEcoTypeLib1* pTypeLib = 0;

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
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
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
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, (void**) &pTypeLib);
    if (result != 0 || pTypeLib == 0) {
        /* Free interfaces in case of an error */
        goto Release;
    }

    /* Test */

    SaveListTypeLib(pTypeLib, "5AADBCB4846C4576827B287B5E67A152.etl");

    LoadAndInspectList(pTypeLib, "5AADBCB4846C4576827B287B5E67A152.etl");


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
    if (pTypeLib != 0) {
        pTypeLib->pVTbl->Release(pTypeLib);
    }


    /* Free the system interface */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

/*

import "IEcoBase1.idl"; 

[
    uuid(5AADBCB4-846C-4576-827B-287B5E67A152)
]
interface IEcoList1 : IEcoUnknown {

    uint32_t Count();
    voidptr_t Item([in] uint32_t index);
    uint32_t Add([in] voidptr_t value);
    uint32_t IndexOf([in] voidptr_t value);
    void InsertAt([in] uint32_t index, [in] voidptr_t value);
    void Remove([in] voidptr_t value);
    void RemoveAt([in] uint32_t index);
    void Clear();
};

*/

/*  Описание и сохранение интерфейса (Serialization) */
void SaveListTypeLib(IEcoTypeLib1* pTypeLib, const char* fileName) {
    IEcoTypeLib1Builder* pBuilder = 0;
    IEcoInterfaceDescriptor1* pDesc = 0;
    IEcoInterfaceDirectory1* pDir = 0;
    IEcoInterfaceDirectoryEntry1* pEntry = 0;
    IEcoMethodDescriptor1* pMethod = 0;
    IEcoParamDescriptor1* pParam = 0;

    /* 1. Получаем Билдер */
    pTypeLib->pVTbl->CreateBuilder(pTypeLib, &pBuilder);

    /* 2. Создаем дескриптор интерфейса (ParentIndex = 0 для IEcoUnknown) */
    pBuilder->pVTbl->CreateInterfaceDescriptor(pBuilder, 0, 0, &pDesc);
    pDesc->pVTbl->set_Flags(pDesc, ECO_INTERFACE_FLAG_SCRIPTABLE); /* is_scriptable = true */

    /* --- Описываем метод: uint32_t Count() --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Count", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: voidptr_t Item([in] uint32_t index) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Item", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "index", ECO_TYPE_UINT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOIDPTR, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: uint32_t Add([in] voidptr_t value) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Add", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "value", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: uint32_t IndexOf([in] voidptr_t value) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "IndexOf", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "value", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: void InsertAt([in] uint32_t index, [in] voidptr_t value) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "InsertAt", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "index", ECO_TYPE_UINT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "value", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOID, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: void Remove([in] voidptr_t value) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Remove", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "value", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOID, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: void RemoveAt([in] uint32_t index) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "RemoveAt", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "index", ECO_TYPE_UINT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOID, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* --- Описываем метод: void Clear() --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Clear", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOID, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);

    /* 3. Создаем запись в директории */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoList1", "Eco.Core", &IID_IEcoList1, pDesc, &pEntry);

    /* 4. Формируем финальную Директорию и добавляем запись */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Сохраняем через основной интерфейс (выполнит расчет смещений и запись в файл) */
    pTypeLib->pVTbl->SaveFile(pTypeLib, fileName, pDir);

    /* Освобождение ресурсов */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}

const char_t* ECO_TYPE_NAME[] = {
    "undefined", /* ECO_TYPE_UNDEFINED */
    "int8", /* ECO_TYPE_INT8 */
    "int16", /* ECO_TYPE_INT16 */
    "int32", /* ECO_TYPE_INT32 */
    "int64", /* ECO_TYPE_INT64 */
    "uint8", /* ECO_TYPE_UINT8 */
    "uint16", /* ECO_TYPE_UINT16 */
    "uint32", /* ECO_TYPE_UINT32 */
    "uint64", /* ECO_TYPE_UINT64 */
    "float", /* ECO_TYPE_FLOAT */
    "double", /* ECO_TYPE_DOUBLE */
    "bool", /* ECO_TYPE_BOOLEAN */
    "char", /* ECO_TYPE_CHAR */
    "wchar", /* ECO_TYPE_WCHAR */
    "astring", /* ECO_TYPE_ASTRING */
    "wstring", /* ECO_TYPE_WSTRING */
    "interface", /* ECO_TYPE_INTERFACE */
    "voidptr", /* ECO_TYPE_VOIDPTR */
    "void"  /* ECO_TYPE_VOID */
};

/* Загрузка и чтение (Deserialization / Introspection) */
void LoadAndInspectList(IEcoTypeLib1* pTypeLib, const char* fileName) {
    IEcoInterfaceDirectory1* pDir = 0;
    IEcoInterfaceDirectoryEntry1* pEntry = 0;
    IEcoInterfaceDescriptor1* pDesc = 0;
    IEcoMethodDescriptor1* pMethod = 0;
    IEcoParamDescriptor1* pParam = 0;
    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint16_t mCount = 0;
    uint8_t pCount = 0;
    uint16_t mIndex = 0;
    uint16_t pIndex = 0;
    char* name = 0;

    /* 1. Прямая загрузка файла в объектную модель Директории */
    if (pTypeLib->pVTbl->LoadFile(pTypeLib, fileName, &pDir) == 0) {
        
        /* 2. Ищем запись по IID */
        if (pDir->pVTbl->GetEntryByIID(pDir, &IID_IEcoList1, &pEntry) == 0) {
            pEntry->pVTbl->get_Name(pEntry, &name);
            printf("Interface: %s\n", name);

            /* 3. Получаем дескриптор интерфейса */
            pEntry->pVTbl->get_Descriptor(pEntry, &pDesc);
            
            /* 4. Читаем методы */
            mCount = pDesc->pVTbl->get_MethodCount(pDesc);
            for (mIndex = 0; mIndex < mCount; mIndex++) {
                pDesc->pVTbl->get_MethodAtIndex(pDesc, mIndex, &pMethod);
                pMethod->pVTbl->get_Name(pMethod, &name);

                pMethod->pVTbl->get_Result(pMethod, &pParam);
                pParam->pVTbl->get_Type(pParam, &typeTag);

                pCount = pMethod->pVTbl->get_ParamCount(pMethod);
                printf("  Method: %s (%s)\n", name, ECO_TYPE_NAME[typeTag]);

                for (pIndex = 0; pIndex < pCount; pIndex++) {
                    pMethod->pVTbl->GetParamAtIndex(pMethod, pIndex, &pParam);
                    pParam->pVTbl->get_Name(pParam, &name);
                    pParam->pVTbl->get_Type(pParam, &typeTag);
                    flags = pParam->pVTbl->get_Flags(pParam);
                    printf("    Param: %s (%s, 0x%.2X)\n", name, ECO_TYPE_NAME[typeTag], flags);
                    pParam->pVTbl->Release(pParam);
                }
                
                pMethod->pVTbl->Release(pMethod);
            }
            pDesc->pVTbl->Release(pDesc);
            pEntry->pVTbl->Release(pEntry);
        }
        pDir->pVTbl->Release(pDir);
    }
}

