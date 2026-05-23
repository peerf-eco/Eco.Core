/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoTypeLib1Test1
 * </summary>
 *
 * <description>
 *   Implementation of the hand-written `.etl` writer for `IEcoTest1`.
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
#include "IEcoTest1.h"

#include "EcoTypeLib1Common.h"
#include "EcoTypeLib1Test1.h"

/*
 *
 * <summary>
 *   SaveTypeLib_IEcoTest1
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoTest1`
 *   and persist it as the corresponding `.etl` file.
 * </description>
 *
 */
void SaveTypeLib_IEcoTest1(IEcoTypeLib1* pTypeLib) {
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

    /* --- int8_t TestInt8([in] int8_t in, [in+out] int8_t inOut, [out] int8_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestInt8", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INT8, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_INT8, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_INT8, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT8, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int16_t TestInt16([in] int16_t in, [in+out] int16_t inOut, [out] int16_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestInt16", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_INT16, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_INT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT16, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int32_t TestInt32([in] int32_t in, [in+out] int32_t inOut, [out] int32_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestInt32", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_INT32, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_INT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT32, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int64_t TestInt64([in] int64_t in, [in+out] int64_t inOut, [out] int64_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestInt64", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INT64, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_INT64, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_INT64, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT64, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- uint8_t TestUInt8([in] uint8_t in, [in+out] uint8_t inOut, [out] uint8_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestUInt8", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_UINT8, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_UINT8, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_UINT8, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT8, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- uint16_t TestUInt16([in] uint16_t in, [in+out] uint16_t inOut, [out] uint16_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestUInt16", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_UINT16, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_UINT16, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_UINT16, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT16, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- uint32_t TestUInt32([in] uint32_t in, [in+out] uint32_t inOut, [out] uint32_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestUInt32", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_UINT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_UINT32, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_UINT32, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT32, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- uint64_t TestUInt64([in] uint64_t in, [in+out] uint64_t inOut, [out] uint64_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestUInt64", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_UINT64, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_UINT64, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_UINT64, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UINT64, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- float_t TestFloat([in] float_t in, [in+out] float_t inOut, [out] float_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestFloat", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_FLOAT, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_FLOAT, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_FLOAT, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_FLOAT, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- double_t TestDouble([in] double_t in, [in+out] double_t inOut, [out] double_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestDouble", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_DOUBLE, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_DOUBLE, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_DOUBLE, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_DOUBLE, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- bool_t TestBoolean([in] bool_t in, [in+out] bool_t inOut, [out] bool_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestBoolean", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_BOOLEAN, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_BOOLEAN, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_BOOLEAN, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_BOOLEAN, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- char_t TestChar([in] char_t in, [in+out] char_t inOut, [out] char_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestChar", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_CHAR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_CHAR, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_CHAR, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_CHAR, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- wchar_t TestWChar([in] wchar_t in, [in+out] wchar_t inOut, [out] wchar_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestWChar", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_WCHAR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_WCHAR, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_WCHAR, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_WCHAR, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- char_t* TestAString([in] char_t* in, [in+out] char_t* inOut, [out] char_t* out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestAString", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_ASTRING, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_ASTRING, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_ASTRING, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_ASTRING, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- wchar_t* TestWString([in] wchar_t* in, [in+out] wchar_t* inOut, [out] wchar_t* out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestWString", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_WSTRING, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_WSTRING, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_WSTRING, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_WSTRING, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- UGUIDPtr_t TestUGUID([in] UGUIDPtr_t in, [in+out] UGUIDPtr_t inOut, [out] UGUIDPtr_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestUGUID", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_UGUID, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_UGUID, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_UGUID, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_UGUID, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- IEcoUnknownPtr_t TestInterface([in] IEcoUnknownPtr_t in, [in+out] IEcoUnknownPtr_t inOut, [out] IEcoUnknownPtr_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestInterface", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INTERFACE, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_INTERFACE, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_INTERFACE, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INTERFACE, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- voidptr_t TestVoidPtr([in] voidptr_t in, [in+out] voidptr_t inOut, [out] voidptr_t out) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestVoidPtr", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_VOIDPTR, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "inOut", ECO_TYPE_VOIDPTR, ECO_PARAM_IN | ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "out", ECO_TYPE_VOIDPTR, ECO_PARAM_OUT, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOIDPTR, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- void TestVoid([in] int32_t in) --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "TestVoid", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "in", ECO_TYPE_INT32, ECO_PARAM_IN, &pParam);
    pMethod->pVTbl->AddParameter(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_VOID, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* --- int32_t GetLastVoidInput() --- */
    pBuilder->pVTbl->CreateMethod(pBuilder, "GetLastVoidInput", 0, &pMethod);
    pBuilder->pVTbl->CreateParameter(pBuilder, "", ECO_TYPE_INT32, ECO_PARAM_RETVAL, &pParam);
    pMethod->pVTbl->set_Result(pMethod, pParam);
    pParam->pVTbl->Release(pParam);
    pDesc->pVTbl->AddMethod(pDesc, pMethod);
    pMethod->pVTbl->Release(pMethod);

    /* 3. Create the directory entry */
    pBuilder->pVTbl->CreateInterfaceDirectoryEntry(pBuilder, "IEcoTest1", "Eco.Test1", &IID_IEcoTest1, pDesc, &pEntry);

    /* 4. Build the final directory and add the entry */
    pBuilder->pVTbl->CreateInterfaceDirectory(pBuilder, &pDir);
    pDir->pVTbl->AddEntry(pDir, -1, pEntry);

    /* 5. Persist via the type-library main interface */
    filePath = UGUIDPtrToTypeLibFilePath(&IID_IEcoTest1);
    pTypeLib->pVTbl->SaveFile(pTypeLib, filePath, pDir);
    free(filePath);

    /* Release */
    pEntry->pVTbl->Release(pEntry);
    pDesc->pVTbl->Release(pDesc);
    pDir->pVTbl->Release(pDir);
    pBuilder->pVTbl->Release(pBuilder);
}
