/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoTypeLib1Calculator
 * </summary>
 *
 * <description>
 *   Implementation of the hand-written `.etl` writers for
 *   `IEcoCalculatorX` and `IEcoCalculatorY`.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"
#include "IEcoTypeLib1Builder.h"
#include "IEcoInterfaceDirectory1.h"
#include "IEcoInterfaceDirectoryEntry1.h"
#include "IEcoInterfaceDescriptor1.h"
#include "IEcoMethodDescriptor1.h"
#include "IEcoParamDescriptor1.h"
#include "IEcoList1.h"

#include "EcoTypeLib1Common.h"
#include "EcoTypeLib1List1.h"

/*
 *
 * <summary>
 *   SaveTypeLib_IEcoList1
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoList1`
 *   and persist it as the corresponding `.etl` file.
 * </description>
 *
 */
void SaveTypeLib_IEcoList1(IEcoTypeLib1* pTypeLib) {
    IEcoTypeLib1Builder* pBuilder = NULL;
    IEcoInterfaceDescriptor1* pDesc = NULL;
    IEcoInterfaceDirectory1* pDir = NULL;
    IEcoInterfaceDirectoryEntry1* pEntry = NULL;
    IEcoMethodDescriptor1* pMethod = NULL;
    IEcoParamDescriptor1* pParam = NULL;
    char_t* filePath = NULL;

    /* 1. Get the builder */
    pTypeLib->pVTbl->CreateBuilder(pTypeLib, &pBuilder);

    /* 2. Create the interface descriptor (ParentIndex = 0 for IEcoUnknown) */
    pBuilder->pVTbl->CreateInterfaceDescriptor(pBuilder, 0, 0, &pDesc);
    pDesc->pVTbl->set_Flags(pDesc, ECO_INTERFACE_FLAG_SCRIPTABLE);

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

    /* 3. Create the directory entry */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoList1", "Eco.List1", &IID_IEcoList1, pDesc, &pEntry);

    /* 4. Build the final directory and add the entry */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Persist via the type-library main interface */
    filePath = UGUIDPtrToTypeLibFilePath(&IID_IEcoList1);
    pTypeLib->pVTbl->SaveFile(pTypeLib, filePath, pDir);
    free(filePath);

    /* Release */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}
