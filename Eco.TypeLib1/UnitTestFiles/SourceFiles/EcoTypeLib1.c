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

/* IID для IEcoList1 */
static const UGUID IID_IEcoList1 = {0x01, 0x10, {0x5A, 0xAD, 0xBC, 0xB4, 0x84, 0x6C, 0x45, 0x76, 0x82, 0x7B, 0x28, 0x7B, 0x5E, 0x67, 0xA1, 0x52}};

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
    pBuilder->pVTbl->CreateInterfaceDescriptor(pBuilder, 0, &pDesc);
    pDesc->pVTbl->set_Flags(pDesc, ECO_INTERFACE_FLAG_SCRIPTABLE); /* is_scriptable = true */

    /* --- Описываем метод: voidptr_t Item([in] uint32_t index) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Item", 0, &pMethod);
    
    /* Параметр 'index' */
    pBuilder->pVTbl->CreateParameter(pBuilder, "index", ECO_TYPE_UINT32, ECO_PARAM_IN, 0, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    
    /* Результат метода */
    pBuilder->pVTbl->CreateParameter(pBuilder, "result", ECO_TYPE_VOIDPTR, 0, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);

    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

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

/* Загрузка и чтение (Deserialization / Introspection) */
void LoadAndInspectList(IEcoTypeLib1* pTypeLib, const char* fileName) {
    IEcoInterfaceDirectory1* pDir = 0;
    IEcoInterfaceDirectoryEntry1* pEntry = 0;
    IEcoInterfaceDescriptor1* pDesc = 0;
    IEcoMethodDescriptor1* pMethod = 0;
    uint16_t mCount = 0;
    uint8_t pCount = 0;
    uint16_t i = 0;
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
            for (i = 0; i < mCount; i++) {
                pDesc->pVTbl->get_MethodAtIndex(pDesc, i, &pMethod);
                pMethod->pVTbl->get_Name(pMethod, &name);

                pCount = pMethod->pVTbl->get_ParamCount(pMethod);
                printf("  Method: %s, Params: %d\n", name, pCount);
                
                pMethod->pVTbl->Release(pMethod);
            }
            pDesc->pVTbl->Release(pDesc);
            pEntry->pVTbl->Release(pEntry);
        }
        pDir->pVTbl->Release(pDir);
    }
}

