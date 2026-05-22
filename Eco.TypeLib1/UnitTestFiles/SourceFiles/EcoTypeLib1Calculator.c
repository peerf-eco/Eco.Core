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

#include "EcoTypeLib1Common.h"
#include "EcoTypeLib1Calculator.h"

const UGUID IID_IEcoCalculatorX = {0x01, 0x10, {0x93, 0x22, 0x11, 0x16, 0x22, 0x48, 0x47, 0x42, 0xAE, 0x06, 0x82, 0x81, 0x94, 0x47, 0x84, 0x3D}};
const UGUID IID_IEcoCalculatorY = {0x01, 0x10, {0xBD, 0x64, 0x14, 0xC2, 0x90, 0x96, 0x42, 0x3E, 0xA9, 0x0C, 0x04, 0xD7, 0x7A, 0xFD, 0x1C, 0xAD}};

/*
 *
 * <summary>
 *   SaveTypeLib_IEcoCalculatorX
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoCalculatorX` (Addition,
 *   Subtraction) and persist it as the corresponding `.etl` file.
 * </description>
 *
 */
void SaveTypeLib_IEcoCalculatorX(IEcoTypeLib1* pTypeLib) {
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

    /* --- int32_t Addition([in] int16_t a, [in] int16_t b) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Addition", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "a", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "b", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t Subtraction([in] int16_t a, [in] int16_t b) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Subtraction", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "a", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "b", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* 3. Create the directory entry */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoCalculatorX", "Eco.Calculator", &IID_IEcoCalculatorX, pDesc, &pEntry);

    /* 4. Build the final directory and add the entry */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Persist via the type-library main interface */
    filePath = UGUIDPtrToTypeLibFilePath(&IID_IEcoCalculatorX);
    pTypeLib->pVTbl->SaveFile(pTypeLib, filePath, pDir);
    free(filePath);

    /* Release */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}

/*
 *
 * <summary>
 *   SaveTypeLib_IEcoCalculatorY
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoCalculatorY` (Multiplication,
 *   Division) and persist it as the corresponding `.etl` file.
 * </description>
 *
 */
void SaveTypeLib_IEcoCalculatorY(IEcoTypeLib1* pTypeLib) {
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

    /* --- int32_t Multiplication([in] int16_t a, [in] int16_t b) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Multiplication", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "a", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "b", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t Division([in] int16_t a, [in] int16_t b) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "Division", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "a", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "b", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* 3. Create the directory entry */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoCalculatorY", "Eco.Calculator", &IID_IEcoCalculatorY, pDesc, &pEntry);

    /* 4. Build the final directory and add the entry */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Persist via the type-library main interface */
    filePath = UGUIDPtrToTypeLibFilePath(&IID_IEcoCalculatorY);
    pTypeLib->pVTbl->SaveFile(pTypeLib, filePath, pDir);
    free(filePath);

    /* Release */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}
