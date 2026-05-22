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
#include "IEcoInterfaceBus1.h"

#include "EcoTypeLib1Common.h"
#include "EcoTypeLib1InterfaceBus1.h"

/*
 *
 * <summary>
 *   SaveTypeLib_IEcoInterfaceBus1
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoInterfaceBus1`
 *   and persist it as the corresponding `.etl` file.
 * </description>
 *
 */
void SaveTypeLib_IEcoInterfaceBus1(IEcoTypeLib1* pTypeLib) {
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

    /* --- int16_t Init() --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Init", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t InitWith([in] voidptr_t heapStartAddress, [in] uint32_t size) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "InitWith", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "heapStartAddress", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "size", ECO_TYPE_UINT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t RegisterComponent([in] UGUIDPtr_t rcid, [in] IEcoUnknownPtr_t pIFactory) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "RegisterComponent", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "rcid", ECO_TYPE_UGUID, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "pIFactory", ECO_TYPE_INTERFACE, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t UnRegisterComponent([in] UGUIDPtr_t rcid) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "UnRegisterComponent", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "rcid", ECO_TYPE_UGUID, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t QueryComponent([in] UGUIDPtr_t rcid, [in] IEcoUnknownPtr_t pIUnkOuter, [in] UGUIDPtr_t riid, [out] voidptr_t ppv) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "QueryComponent", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "rcid", ECO_TYPE_UGUID, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "pIUnkOuter", ECO_TYPE_INTERFACE, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "riid", ECO_TYPE_UGUID, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "ppv", ECO_TYPE_INTERFACE, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* 3. Create the directory entry */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoInterfaceBus1", "Eco.InterfaceBus1", &IID_IEcoInterfaceBus1, pDesc, &pEntry);

    /* 4. Build the final directory and add the entry */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Persist via the type-library main interface */
    filePath = UGUIDPtrToTypeLibFilePath(&IID_IEcoInterfaceBus1);
    pTypeLib->pVTbl->SaveFile(pTypeLib, filePath, pDir);
    free(filePath);

    /* Release */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}
