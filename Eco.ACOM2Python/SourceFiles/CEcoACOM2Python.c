/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoACOM2Python_566F1CC3
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoACOM2Python_566F1CC3
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <ffi.h>

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoList1.h"
#include "IEcoTypeLib1Builder.h"
#include "IEcoInterfaceDirectory1.h"
#include "IEcoInterfaceDirectoryEntry1.h"
#include "IEcoParamDescriptor1.h"

#include "CEcoACOM2Python.h"

#define HALF_BYTE_SIZE 4
#define IECOUNKNOWN_METHOD_COUNT 3 /* QueryInterface, AddRef, Release */

/* -------------------------------------------------------------------------
 * Type mapping tables
 * ------------------------------------------------------------------------- */


// Mapping of Eco type tags to `CPython format characters` and `Python type names`
EcoPythonTypeMap ECO_TYPE_MAP[] = {
    { "",   ""        }, /* ECO_TYPE_UNDEFINED */
    { "b",  "Int8"    }, /* ECO_TYPE_INT8 */
    { "h",  "Int16"   }, /* ECO_TYPE_INT16 */
    { "i",  "Int32"   }, /* ECO_TYPE_INT32 */
    { "L",  "Int64"   }, /* ECO_TYPE_INT64 */
    { "B",  "UInt8"   }, /* ECO_TYPE_UINT8 */
    { "H",  "UInt16"  }, /* ECO_TYPE_UINT16 */
    { "I",  "UInt32"  }, /* ECO_TYPE_UINT32 */
    { "K",  "UInt64"  }, /* ECO_TYPE_UINT64 */
    { "f",  "Float"   }, /* ECO_TYPE_FLOAT */
    { "d",  "Double"  }, /* ECO_TYPE_DOUBLE */
    { "B",  "Bool"    }, /* ECO_TYPE_BOOLEAN */
    { "y#", "Char"    }, /* ECO_TYPE_CHAR */
    { "u#", "WChar"   }, /* ECO_TYPE_WCHAR */
    { "y",  "CString" }, /* ECO_TYPE_ASTRING */
    { "u",  "WString" }, /* ECO_TYPE_WSTRING */
    { "O",  ""        }, /* ECO_TYPE_INTERFACE — handled via WrapAsInterfacePtr */
    { "O",  ""        }, /* ECO_TYPE_UGUID — handled via WrapAsUGUIDPtr */
    { "O",  ""        }, /* ECO_TYPE_VOIDPTR — handled via WrapAsVoidPtr */
    { "",  "None"   }  /* ECO_TYPE_VOID */
};

// Mapping of Eco type tags to `libffi` types
static ffi_type* ECO_FFI_TYPES[] = {
    &ffi_type_void,    /* ECO_TYPE_UNDEFINED */
    &ffi_type_sint8,   /* ECO_TYPE_INT8 */
    &ffi_type_sint16,  /* ECO_TYPE_INT16 */
    &ffi_type_sint32,  /* ECO_TYPE_INT32 */
    &ffi_type_sint64,  /* ECO_TYPE_INT64 */
    &ffi_type_uint8,   /* ECO_TYPE_UINT8 */
    &ffi_type_uint16,  /* ECO_TYPE_UINT16 */
    &ffi_type_uint32,  /* ECO_TYPE_UINT32 */
    &ffi_type_uint64,  /* ECO_TYPE_UINT64 */
    &ffi_type_float,   /* ECO_TYPE_FLOAT */
    &ffi_type_double,  /* ECO_TYPE_DOUBLE */
    &ffi_type_uint8,   /* ECO_TYPE_BOOLEAN */
    &ffi_type_sint8,   /* ECO_TYPE_CHAR */
    &ffi_type_uint16,  /* ECO_TYPE_WCHAR */
    &ffi_type_pointer, /* ECO_TYPE_ASTRING */
    &ffi_type_pointer, /* ECO_TYPE_WSTRING */
    &ffi_type_pointer, /* ECO_TYPE_INTERFACE */
    &ffi_type_pointer, /* ECO_TYPE_UGUID */
    &ffi_type_pointer, /* ECO_TYPE_VOIDPTR */
    &ffi_type_void     /* ECO_TYPE_VOID */
};

static ffi_type* GetFfiType(uint16_t typeTag) {
    if (typeTag >= sizeof(ECO_FFI_TYPES) / sizeof(*ECO_FFI_TYPES)) return NULL;
    return ECO_FFI_TYPES[typeTag];
}


/* -------------------------------------------------------------------------
 * Forward declarations
 * ------------------------------------------------------------------------- */

static int16_t BootPythonInterpreter(int16_t* pInitialised);
static void TeardownPythonInterpreter(int16_t initialised);
static int16_t LoadPythonComponentClass(const char_t* pathName, const char_t* className, PyObject** ppClass);
static PyObject* ImportModuleAttr(const char_t* moduleName, const char_t* attrName);

static void UGUIDToHexString(const UGUID* uguid, char_t* buffer);
static IEcoInterfaceDescriptor1* GetInterfaceDescriptorByUGUID(IEcoTypeLib1* pITypeLib, const UGUID* riid);

static PyObject* WrapAsUGUIDPtr(const UGUID* uguid);
static UGUID UnwrapUGUIDPtr(PyObject* pPtr);
static PyObject* WrapAsVoidPtr(void* addr);
static void* UnwrapVoidPtr(PyObject* pPtr);
static PyObject* WrapAsInterfacePtr(void* iface);
static IEcoUnknown* UnwrapInterfacePtr(PyObject* pPtr);

static PyObject* WrapAsCType(const char_t* className, PyObject* pValue);
static PyObject* WrapAsPtr(PyObject* pValue);
static PyObject* UnwrapPtrValue(PyObject* pPtr);
static PyObject* UnwrapCTypeValue(PyObject* pCType);

static void ParamToPyObject(void* arg, uint16_t typeTag, uint8_t flags, PyObject** ppArg);
static void PyObjectToCSlot(EcoPythonComponentContext* component, PyObject* pVal, uint16_t typeTag, void* dst);
static void PyObjectToParam(EcoPythonComponentContext* component, PyObject* pVal, uint16_t typeTag, void** arg);
static void CallPythonMethod(EcoPythonComponentContext* component, PyObject* pMethod, PyObject* pArgs, uint16_t typeTag, void* ret);

static EcoPythonComponentContext* CreateEcoPythonComponentContext(PyObject* pObj, IEcoInterfaceDescriptor1* pIDesc, IEcoMemoryAllocator1* pIMem, IEcoTypeLib1* pITypeLib);
static void EcoPythonGlobalDispatcher(ffi_cif* cif, void* ret, void** args, void* userData);


/* -------------------------------------------------------------------------
 * UGUID / type-library helpers
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   UGUIDToHexString
 * </summary>
 *
 * <description>
 *   Write the HEX representation of a UGUID's into buffer.
 * </description>
 *
 */
static void UGUIDToHexString(const UGUID* uguid, char_t* buffer) {
    byte_t i = 0;
    byte_t high = 0;
    byte_t low = 0;

    for (i = 0; i < uguid->Length; i++) {
        high = uguid->Data[i] >> HALF_BYTE_SIZE;
        low = uguid->Data[i] & 0x0F;
        buffer[i * 2]     = (high < 10) ? (high + '0') : ((high - 10) + 'A');
        buffer[i * 2 + 1] = (low < 10) ? (low + '0') : ((low - 10) + 'A');
    }
    buffer[uguid->Length * 2] = '\0';
}

/*
 *
 * <summary>
 *   GetInterfaceDescriptorByUGUID
 * </summary>
 *
 * <description>
 *   Load `<IID>.etl` first from the CWD, then from `ECO_FRAMEWORK_RT`
 * </description>
 *
 */
static IEcoInterfaceDescriptor1* GetInterfaceDescriptorByUGUID(IEcoTypeLib1* pITypeLib, const UGUID* riid) {
    IEcoInterfaceDirectory1* pIDirectory = NULL;
    IEcoInterfaceDirectoryEntry1* pIEntry = NULL;
    IEcoInterfaceDescriptor1* pIDesc = NULL;
    char_t iidHex[64];
    char_t cidHex[64];
    char_t fileName[1024];
    const char_t* envRoot = NULL;
    int16_t result = 0;

    if (pITypeLib == NULL || riid == NULL) return NULL;

    UGUIDToHexString(riid, iidHex);

    /* 1. Try `<IID>.etl` in the current working directory */
    snprintf(fileName, sizeof(fileName), "%s.etl", iidHex);
    result = pITypeLib->pVTbl->LoadFile(pITypeLib, fileName, &pIDirectory);

    /* 2. Fallback to `%ECO_FRAMEWORK_RT%\<GID_IEcoSystem>\<IID>.etl` */
    if (result != 0 || pIDirectory == NULL) {
        envRoot = getenv("ECO_FRAMEWORK_RT");
        if (envRoot == NULL) return NULL;

        UGUIDToHexString(&GID_IEcoSystem, cidHex);
        snprintf(fileName, sizeof(fileName), "%s\\%s\\%s.etl", envRoot, cidHex, iidHex);

        result = pITypeLib->pVTbl->LoadFile(pITypeLib, fileName, &pIDirectory);
        if (result != 0 || pIDirectory == NULL) return NULL;
    }

    result = pIDirectory->pVTbl->GetEntryByIID(pIDirectory, riid, &pIEntry);
    if (result == 0 && pIEntry != NULL) {
        pIEntry->pVTbl->get_Descriptor(pIEntry, &pIDesc);
        pIEntry->pVTbl->Release(pIEntry);
    }
    pIDirectory->pVTbl->Release(pIDirectory);

    return pIDesc;
}


/* -------------------------------------------------------------------------
 * UGUID <-> Python conversion
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   WrapAsUGUIDPtr  (C UGUID → Python Ptr[UGUID])
 * </summary>
 *
 * <description>
 *   Build a Python pointer to a `UGUID` 18-byte instance
 * </description>
 *
 */
static PyObject* WrapAsUGUIDPtr(const UGUID* uguid) {
    PyObject* pPtrCls = NULL;
    PyObject* pUGUIDCls = NULL;
    PyObject* pData = NULL;
    PyObject* pUGUIDObj = NULL;
    PyObject* pPtrUGUID = NULL;
    PyObject* pInstance = NULL;

    if (uguid == NULL) Py_RETURN_NONE;

    pPtrCls   = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pUGUIDCls = ImportModuleAttr("eco_python2acom.types.guid", "UGUID");
    if (pPtrCls == NULL || pUGUIDCls == NULL) goto Cleanup;

    pData = PyBytes_FromStringAndSize((const char*)uguid->Data, (Py_ssize_t)sizeof(uguid->Data));
    if (pData == NULL) goto Cleanup;

    pUGUIDObj = PyObject_CallFunction(pUGUIDCls, "OB", pData, (uint8_t)uguid->Preamble);
    if (pUGUIDObj == NULL) goto Cleanup;

    pPtrUGUID = PyObject_GetItem(pPtrCls, pUGUIDCls);
    if (pPtrUGUID == NULL) goto Cleanup;

    pInstance = PyObject_CallFunctionObjArgs(pPtrUGUID, pUGUIDObj, NULL);

Cleanup:
    Py_XDECREF(pPtrUGUID);
    Py_XDECREF(pUGUIDObj);
    Py_XDECREF(pData);
    Py_XDECREF(pUGUIDCls);
    Py_XDECREF(pPtrCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   UnwrapUGUIDPtr  (Python Ptr[UGUID] → C UGUID)
 * </summary>
 *
 * <description>
 *   Read the 18-byte layout of a Python `UGUID` instance under pointer back into a C `UGUID`
 * </description>
 *
 */
static UGUID UnwrapUGUIDPtr(PyObject* pPtr) {
    UGUID result = { 0};
    PyObject* pObj = NULL;
    PyObject* pBytes = NULL;
    char* buffer = NULL;
    Py_ssize_t length = 0;

    if (pPtr == NULL || pPtr == Py_None) return result;

    pObj = PyObject_GetAttrString(pPtr, "obj");
    if (pObj == NULL) { PyErr_Clear(); return result; }

    pBytes = PyObject_Bytes(pObj);
    Py_DECREF(pObj);
    if (pBytes == NULL) { PyErr_Clear(); return result; }

    if (PyBytes_AsStringAndSize(pBytes, &buffer, &length) == 0 && length == (Py_ssize_t)sizeof(UGUID)) {
        memcpy(&result, buffer, sizeof(UGUID));
    } else {
        PyErr_Clear();
    }
    Py_DECREF(pBytes);
    return result;
}


/* -------------------------------------------------------------------------
 * Void pointer <-> Python conversion
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   WrapAsVoidPtr
 * </summary>
 *
 * <description>
 *   Build a `Ptr[Void]` instance whose backing memory is the C address `addr`
 * </description>
 *
 */
static PyObject* WrapAsVoidPtr(void* addr) {
    PyObject* pPtrCls = NULL;
    PyObject* pVoidCls = NULL;
    PyObject* pPtrVoid = NULL;
    PyObject* pAddrInt = NULL;
    PyObject* pInstance = NULL;

    pPtrCls  = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pVoidCls = ImportModuleAttr("eco_python2acom.types.core", "Void");
    if (pPtrCls == NULL || pVoidCls == NULL) goto Cleanup;

    pPtrVoid = PyObject_GetItem(pPtrCls, pVoidCls);
    if (pPtrVoid == NULL) goto Cleanup;

    pAddrInt = PyLong_FromVoidPtr(addr);
    if (pAddrInt == NULL) goto Cleanup;

    pInstance = PyObject_CallFunctionObjArgs(pPtrVoid, pAddrInt, NULL);

Cleanup:
    Py_XDECREF(pAddrInt);
    Py_XDECREF(pPtrVoid);
    Py_XDECREF(pVoidCls);
    Py_XDECREF(pPtrCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   UnwrapVoidPtr
 * </summary>
 *
 * <description>
 *   Read the raw address out of a `Ptr[Void]` instance.
 * </description>
 *
 */
static void* UnwrapVoidPtr(PyObject* pPtr) {
    PyObject* pValue = NULL;
    void* addr = NULL;

    if (pPtr == NULL || pPtr == Py_None) return NULL;

    pValue = PyObject_GetAttrString(pPtr, "value");
    if (pValue == NULL) {
        PyErr_Clear();
        return NULL;
    }
    addr = PyLong_AsVoidPtr(pValue);
    Py_DECREF(pValue);
    if (PyErr_Occurred()) {
        PyErr_Clear();
        return NULL;
    }
    return addr;
}


/* -------------------------------------------------------------------------
 * Interface <-> Python conversion
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   WrapAsInterfacePtr
 * </summary>
 *
 * <description>
 *   Build a `Ptr[IEcoUnknown]` instance whose backing memory is the COM interface pointer `iface`
 * </description>
 *
 */
static PyObject* WrapAsInterfacePtr(void* iface) {
    PyObject* pCastFn = NULL;
    PyObject* pPtrCls = NULL;
    PyObject* pIfaceCls = NULL;
    PyObject* pPtrIface = NULL;
    PyObject* pVoidPtr = NULL;
    PyObject* pInstance = NULL;

    pCastFn   = ImportModuleAttr("eco_python2acom.types.utils", "cast");
    pPtrCls   = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pIfaceCls = ImportModuleAttr("eco_python2acom.interfaces.unknown", "IEcoUnknown");
    if (pCastFn == NULL || pPtrCls == NULL || pIfaceCls == NULL) goto Cleanup;

    pPtrIface = PyObject_GetItem(pPtrCls, pIfaceCls);
    if (pPtrIface == NULL) goto Cleanup;

    pVoidPtr = WrapAsVoidPtr(iface);
    if (pVoidPtr == NULL) goto Cleanup;

    pInstance = PyObject_CallFunctionObjArgs(pCastFn, pVoidPtr, pPtrIface, NULL);

Cleanup:
    Py_XDECREF(pVoidPtr);
    Py_XDECREF(pPtrIface);
    Py_XDECREF(pIfaceCls);
    Py_XDECREF(pPtrCls);
    Py_XDECREF(pCastFn);
    return pInstance;
}

/*
 *
 * <summary>
 *   UnwrapInterfacePtr
 * </summary>
 *
 * <description>
 *   Read the ACOM interface pointer out of a `Ptr[IEcoUnknown]` instance
 * </description>
 *
 */
static void* UnwrapInterfacePtr(PyObject* pPtr) {
    PyObject* pCastFn = NULL;
    PyObject* pPtrCls = NULL;
    PyObject* pVoidCls = NULL;
    PyObject* pPtrVoid = NULL;
    PyObject* pVoidPtr = NULL;
    void* addr = NULL;

    if (pPtr == NULL || pPtr == Py_None) return NULL;

    pCastFn  = ImportModuleAttr("eco_python2acom.types.utils", "cast");
    pPtrCls  = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pVoidCls = ImportModuleAttr("eco_python2acom.types.core", "Void");
    if (pCastFn == NULL || pPtrCls == NULL || pVoidCls == NULL) goto Cleanup;

    pPtrVoid = PyObject_GetItem(pPtrCls, pVoidCls);
    if (pPtrVoid == NULL) goto Cleanup;

    pVoidPtr = PyObject_CallFunctionObjArgs(pCastFn, pPtr, pPtrVoid, NULL);
    if (pVoidPtr == NULL) goto Cleanup;

    addr = UnwrapVoidPtr(pVoidPtr);

Cleanup:
    Py_XDECREF(pVoidPtr);
    Py_XDECREF(pPtrVoid);
    Py_XDECREF(pVoidCls);
    Py_XDECREF(pPtrCls);
    Py_XDECREF(pCastFn);
    if (PyErr_Occurred()) PyErr_Clear();
    return addr;
}


/* -------------------------------------------------------------------------
 * Wrapping and unwrapping helpers
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   WrapAsCType
 * </summary>
 *
 * <description>
 *   Construct a `<className>(pValue)` instance
 * </description>
 *
 */
static PyObject* WrapAsCType(const char_t* className, PyObject* pValue) {
    PyObject* pCls = NULL;
    PyObject* pInstance = NULL;

    if (className == NULL || pValue == NULL) return NULL;

    pCls = ImportModuleAttr("eco_python2acom.types.core", className);
    if (pCls == NULL) return NULL;

    pInstance = PyObject_CallFunctionObjArgs(pCls, pValue, NULL);
    Py_DECREF(pCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   UnwrapCTypeValue
 * </summary>
 *
 * <description>
 *   Read `.value` off a scalar instance
 * </description>
 *
 */
static PyObject* UnwrapCTypeValue(PyObject* pCType) {
    PyObject* pValue = NULL;

    if (pCType == NULL || pCType == Py_None) return NULL;

    pValue = PyObject_GetAttrString(pCType, "value");
    if (pValue == NULL) {
        PyErr_Clear();
        return NULL;
    }
    return pValue;
}


/*
 *
 * <summary>
 *   WrapAsPtr
 * </summary>
 *
 * <description>
 *   Wrap object with type T in Ptr[T]
 * </description>
 *
 */
static PyObject* WrapAsPtr(PyObject* pValue) {
    PyObject* pPtrCls = NULL;
    PyObject* pValueType = NULL;
    PyObject* pPtrT = NULL;
    PyObject* pInstance = NULL;

    if (pValue == NULL) return NULL;

    pPtrCls = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    if (pPtrCls == NULL) goto Cleanup;

    pValueType = PyObject_Type(pValue);
    if (pValueType == NULL) goto Cleanup;
    pPtrT = PyObject_GetItem(pPtrCls, pValueType);
    if (pPtrT == NULL) goto Cleanup;

    pInstance = PyObject_CallFunctionObjArgs(pPtrT, pValue, NULL);

Cleanup:
    Py_XDECREF(pPtrT);
    Py_XDECREF(pValueType);
    Py_XDECREF(pPtrCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   UnwrapPtrValue
 * </summary>
 *
 * <description>
 *   Dereference a Ptr[T] and return the pointed-to object
 * </description>
 *
 */
static PyObject* UnwrapPtrValue(PyObject* pPtr) {
    PyObject* pResult = NULL;

    if (pPtr == NULL || pPtr == Py_None) return NULL;

    pResult = PyObject_GetAttrString(pPtr, "obj");
    if (pResult == NULL) {
        PyErr_Clear();
        return NULL;
    }
    return pResult;
}

/* -------------------------------------------------------------------------
 * Marshalling
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   ParamToPyObject
 * </summary>
 *
 * <description>
 *   Convert one C argument slot into a `PyObject*` for the args tuple of a Python method call
 * </description>
 *
 */
static void ParamToPyObject(void* arg, uint16_t typeTag, uint8_t flags, PyObject** ppArg) {
    EcoPythonTypeMap* meta = NULL;
    PyObject* pTmp = NULL;
    int hasValue = 0;

    if (ppArg == NULL) return;
    *ppArg = NULL;

    if (typeTag >= sizeof(ECO_TYPE_MAP) / sizeof(*ECO_TYPE_MAP)) goto Done;
    meta = &ECO_TYPE_MAP[typeTag];

    /* For OUT-only parameters, create an empty placeholder. For IN-OUT, use actual value.
       For IN-only, just use the value. Pointer types are handled directly. */
    hasValue = (flags & ECO_PARAM_IN) != 0;

    switch (typeTag) {
        case ECO_TYPE_INT8:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(int8_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (int8_t)0);
            break;
        case ECO_TYPE_UINT8:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(uint8_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (uint8_t)0);
            break;
        case ECO_TYPE_INT16:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(int16_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (int16_t)0);
            break;
        case ECO_TYPE_UINT16:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(uint16_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (uint16_t)0);
            break;
        case ECO_TYPE_INT32:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(int32_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (int32_t)0);
            break;
        case ECO_TYPE_UINT32:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(uint32_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (uint32_t)0);
            break;
        case ECO_TYPE_INT64:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(int64_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (int64_t)0);
            break;
        case ECO_TYPE_UINT64:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(uint64_t*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (uint64_t)0);
            break;
        case ECO_TYPE_FLOAT:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, (double)*(float*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (double)0.0f);
            break;
        case ECO_TYPE_DOUBLE:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(double*)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, (double)0.0);
            break;
        case ECO_TYPE_BOOLEAN:
            if (hasValue) pTmp = PyBool_FromLong((long)*(uint8_t*)arg);
            else pTmp = PyBool_FromLong(0L);
            break;
        case ECO_TYPE_CHAR:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, (const char*)arg, (Py_ssize_t)1);
            else pTmp = Py_BuildValue(meta->pyFormat, "", (Py_ssize_t)0);
            break;
        case ECO_TYPE_WCHAR:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, (const wchar_t*)arg, (Py_ssize_t)1);
            else pTmp = Py_BuildValue(meta->pyFormat, L"", (Py_ssize_t)0);
            break;
        case ECO_TYPE_ASTRING:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(const char_t**)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, "");
            break;
        case ECO_TYPE_WSTRING:
            if (hasValue) pTmp = Py_BuildValue(meta->pyFormat, *(const wchar_t**)arg);
            else pTmp = Py_BuildValue(meta->pyFormat, L"");
            break;
        case ECO_TYPE_UGUID:
            if (hasValue) pTmp = WrapAsUGUIDPtr(*(const UGUID**)arg);
            else pTmp = WrapAsUGUIDPtr(NULL);
            break;
        case ECO_TYPE_VOIDPTR:
            if (hasValue) pTmp = WrapAsVoidPtr(*(void**)arg);
            else pTmp = WrapAsVoidPtr(NULL);
            break;
        case ECO_TYPE_INTERFACE:
            if (hasValue) pTmp = WrapAsInterfacePtr(*(void**)arg);
            else pTmp = WrapAsInterfacePtr(NULL);
            break;

        default: break;
    }

    /* Wrap scalar in ctypes class if needed */
    if (pTmp != NULL && *meta->pyTypeName != '\0') {
        *ppArg = WrapAsCType(meta->pyTypeName, pTmp);
        Py_DECREF(pTmp);
    } else if (pTmp != NULL) {
        *ppArg = pTmp;
    }

    /* For OUT parameters (IN-OUT or OUT-only), wrap in Ptr[T] */
    if (*ppArg != NULL && (flags & ECO_PARAM_OUT)) {
        PyObject* pPtr = WrapAsPtr(*ppArg);
        Py_DECREF(*ppArg);
        *ppArg = pPtr;
    }

Done:
    if (*ppArg == NULL) {
        Py_INCREF(Py_None);
        *ppArg = Py_None;
    }
}

/*
 *
 * <summary>
 *   PyObjectToCSlot
 * </summary>
 *
 * <description>
 *   Unpack a Python value and write it into a raw C memory slot
 * </description>
 *
 */
static void PyObjectToCSlot(EcoPythonComponentContext* component, PyObject* pVal, uint16_t typeTag, void* dst) {
    PyObject* pInner = NULL;

    if (dst == NULL || pVal == NULL) return;

    switch (typeTag) {
        /* Scalar primitives — unwrap typed ctypes scalar via `.value` */
        case ECO_TYPE_INT8:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(int8_t*)dst = (int8_t)PyLong_AsLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_UINT8:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(uint8_t*)dst = (uint8_t)PyLong_AsLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_INT16:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(int16_t*)dst = (int16_t)PyLong_AsLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_UINT16:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(uint16_t*)dst = (uint16_t)PyLong_AsLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_INT32:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(int32_t*)dst = (int32_t)PyLong_AsLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_UINT32:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(uint32_t*)dst = (uint32_t)PyLong_AsUnsignedLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_INT64:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(int64_t*)dst = (int64_t)PyLong_AsLongLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_UINT64:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(uint64_t*)dst = (uint64_t)PyLong_AsUnsignedLongLong(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_FLOAT:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(float*)dst = (float)PyFloat_AsDouble(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_DOUBLE:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(double*)dst = PyFloat_AsDouble(pInner); Py_DECREF(pInner); } break;
        case ECO_TYPE_BOOLEAN:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) { *(uint8_t*)dst = (uint8_t)(PyObject_IsTrue(pInner) ? 1 : 0); Py_DECREF(pInner); } break;
        case ECO_TYPE_CHAR:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyBytes_Check(pInner)) { *(char_t*)dst = *PyBytes_AsString(pInner); }
            Py_XDECREF(pInner);
            break;
        case ECO_TYPE_WCHAR:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyUnicode_Check(pInner)) { PyUnicode_AsWideChar(pInner, (wchar_t*)dst, 1); }
            Py_XDECREF(pInner);
            break;

        /* Pointer types */
        case ECO_TYPE_UGUID:    *(UGUID*)dst  = UnwrapUGUIDPtr(pVal); break;
        case ECO_TYPE_VOIDPTR:  *(void**)dst  = UnwrapVoidPtr(pVal);  break;

        case ECO_TYPE_INTERFACE: *(void**)dst = UnwrapInterfacePtr(pVal); break;

        case ECO_TYPE_ASTRING: {
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyBytes_Check(pInner)) {
                char* src = PyBytes_AsString(pInner);
                Py_ssize_t len = PyBytes_Size(pInner);
                if (src != NULL && component != NULL && component->m_pIMem != NULL) {
                    char_t* buf = (char_t*)component->m_pIMem->pVTbl->Alloc(component->m_pIMem, (uint32_t)(len + 1));
                    if (buf != NULL) {
                        component->m_pIMem->pVTbl->Copy(component->m_pIMem, buf, src, (uint32_t)len);
                        buf[len] = '\0';
                        *(const char_t**)dst = buf;
                    }
                }
            }
            Py_XDECREF(pInner);
            break;
        }
        case ECO_TYPE_WSTRING: {
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyUnicode_Check(pInner)) {
                Py_ssize_t len = 0;
                wchar_t* src = PyUnicode_AsWideCharString(pInner, &len);
                if (src != NULL && component != NULL && component->m_pIMem != NULL) {
                    wchar_t* buf = (wchar_t*)component->m_pIMem->pVTbl->Alloc(component->m_pIMem, (uint32_t)((len + 1) * sizeof(wchar_t)));
                    if (buf != NULL) {
                        component->m_pIMem->pVTbl->Copy(component->m_pIMem, buf, src, (uint32_t)(len * sizeof(wchar_t)));
                        buf[len] = L'\0';
                        *(const wchar_t**)dst = buf;
                    }
                }
                if (src != NULL) PyMem_Free(src);
            }
            Py_XDECREF(pInner);
            break;
        }

        default: break;
    }

    if (PyErr_Occurred()) PyErr_Clear();
}

/*
 *
 * <summary>
 *   PyObjectToParam
 * </summary>
 *
 * <description>
 *   Reverse of `ParamToPyObject`: dereference `Ptr[T]` and write the value into the C slot
 * </description>
 *
 */
static void PyObjectToParam(EcoPythonComponentContext* component, PyObject* pVal, uint16_t typeTag, void** arg) {
    PyObject* pDeref = NULL;

    if (arg == NULL || *arg == NULL || pVal == NULL) return;

    /* OUT params come in as Ptr[T] — dereference to get the underlying T */
    pDeref = UnwrapPtrValue(pVal);
    if (pDeref != NULL) pVal = pDeref;

    PyObjectToCSlot(component, pVal, typeTag, *arg);

    Py_XDECREF(pDeref);
}

/*
 *
 * <summary>
 *   CallPythonMethod
 * </summary>
 *
 * <description>
 *   Invoke method with the prepared args tuple and unpack the result
 * </description>
 *
 */
static void CallPythonMethod(EcoPythonComponentContext* component, PyObject* pMethod, PyObject* pArgs, uint16_t typeTag, void* ret) {
    PyObject* pResult = NULL;
    PyObject* pEmptyArgs = NULL;

    if (pMethod == NULL || ret == NULL) return;

    if (pArgs == NULL) {
        pEmptyArgs = PyTuple_New(0);
        pArgs = pEmptyArgs;
    }
    pResult = PyObject_Call(pMethod, pArgs, NULL);
    Py_XDECREF(pEmptyArgs);

    if (pResult == NULL) {
        if (PyErr_Occurred()) PyErr_Clear();
        return;
    }

    /* Return `None` for void-returning methods */
    if (pResult == Py_None || typeTag == ECO_TYPE_VOID) {
        Py_DECREF(pResult);
        return;
    }
    PyObjectToCSlot(component, pResult, typeTag, ret);
    Py_DECREF(pResult);
}


/* -------------------------------------------------------------------------
 * EcoPythonComponentContext — IEcoUnknown triple
 * ------------------------------------------------------------------------- */

static int16_t ECOCALLMETHOD EcoPythonComponentContext_IEcoUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoPythonComponentContext* proxy = (EcoPythonComponentContext*) me;
    PyGILState_STATE gil;
    PyObject* pIid = NULL;
    PyObject* pOut = NULL;
    PyObject* pCallResult = NULL;
    PyObject* pStatusValue = NULL;
    int16_t status = ERR_ECO_FAIL;

    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;

    gil = PyGILState_Ensure();

    pIid = WrapAsUGUIDPtr(riid);
    pOut = WrapAsPtr(WrapAsVoidPtr(NULL));
    if (pIid == NULL || pOut == NULL) goto Cleanup;

    pCallResult = PyObject_CallMethod(proxy->m_obj, "QueryInterface", "OO", pIid, pOut);
    if (pCallResult == NULL) goto Cleanup;

    pStatusValue = UnwrapCTypeValue(pCallResult);
    if (pStatusValue == NULL) goto Cleanup;
    status = (int16_t)PyLong_AsLong(pStatusValue);
    if (status != ERR_ECO_SUCCESES) goto Cleanup;

    ++proxy->m_cRef;
    *ppv = proxy;

Cleanup:
    if (PyErr_Occurred()) PyErr_Clear();
    Py_XDECREF(pStatusValue);
    Py_XDECREF(pCallResult);
    Py_XDECREF(pOut);
    Py_XDECREF(pIid);
    PyGILState_Release(gil);
    return status;
}

static uint32_t ECOCALLMETHOD EcoPythonComponentContext_IEcoUnknown_AddRef(IEcoUnknownPtr_t me) {
    EcoPythonComponentContext* proxy = (EcoPythonComponentContext*) me;
    PyGILState_STATE gil;
    PyObject* pResult = NULL;

    if (me == NULL) return (uint32_t)-1;

    gil = PyGILState_Ensure();
    pResult = PyObject_CallMethod(proxy->m_obj, "AddRef", NULL);
    if (pResult != NULL) Py_DECREF(pResult);

    if (PyErr_Occurred()) PyErr_Clear();
    PyGILState_Release(gil);

    return ++proxy->m_cRef;
}

static uint32_t ECOCALLMETHOD EcoPythonComponentContext_IEcoUnknown_Release(IEcoUnknownPtr_t me) {
    EcoPythonComponentContext* proxy = (EcoPythonComponentContext*) me;
    PyGILState_STATE gil;
    PyObject* pResult = NULL;
    uint16_t i = 0;
    uint16_t mCount = 0;
    uint32_t cRef = 0;

    if (me == NULL) return (uint32_t)-1;

    gil = PyGILState_Ensure();
    pResult = PyObject_CallMethod(proxy->m_obj, "Release", NULL);
    if (pResult != NULL) Py_DECREF(pResult);
    if (PyErr_Occurred()) PyErr_Clear();

    cRef = --proxy->m_cRef;
    if (cRef == 0) {
        Py_XDECREF(proxy->m_obj);
        proxy->m_obj = NULL;
    }
    PyGILState_Release(gil);

    if (cRef > 0) return cRef;

    /* Tear down every libffi closure */
    if (proxy->m_pListMethods != NULL && proxy->m_pIDesc != NULL) {
        mCount = proxy->m_pIDesc->pVTbl->get_MethodCount(proxy->m_pIDesc);
        for (i = 0; i < mCount; i++) {
            EcoPythonMethodContext* ctx = &proxy->m_pListMethods[i];
            if (ctx->closure != NULL) {
                ffi_closure_free(ctx->closure);
                ctx->closure = NULL;
            }
            if (ctx->argTypes != NULL) {
                proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, ctx->argTypes);
                ctx->argTypes = NULL;
            }
        }
        proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy->m_pListMethods);
        proxy->m_pListMethods = NULL;
    }
    if (proxy->m_pIDesc != NULL) {
        proxy->m_pIDesc->pVTbl->Release(proxy->m_pIDesc);
        proxy->m_pIDesc = NULL;
    }
    if (proxy->m_pVTbl != NULL && proxy->m_pIMem != NULL) {
        proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy->m_pVTbl);
    }
    if (proxy->m_pIMem != NULL) {
        proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy);
    }
    return 0;
}


/* -------------------------------------------------------------------------
 * EcoPythonComponentContext — global dispatcher
 *
 * `libffi` calls this routine through every closure attached to a proxy's v-table
 * ------------------------------------------------------------------------- */

static void EcoPythonGlobalDispatcher(ffi_cif* cif, void* ret, void** args, void* userData) {
    EcoPythonMethodContext* ctx = (EcoPythonMethodContext*) userData;
    EcoPythonComponentContext* proxy = NULL;
    IEcoMethodDescriptor1* pIMethod = NULL;
    IEcoParamDescriptor1* pIParam = NULL;
    PyObject* pMethod = NULL;
    PyObject* pTuple = NULL;
    PyObject* pArg = NULL;
    PyGILState_STATE gil;
    uint16_t typeTag = 0;
    uint16_t resultTag = 0;
    uint8_t flags = 0;
    uint8_t i = 0;
    /* `cif` is required by the `libffi` callback contract but unused */
    (void) cif;

    if (ctx == NULL || ctx->component == NULL || ctx->methodDesc == NULL || ctx->methodName == NULL) return;
    proxy = ctx->component;
    pIMethod = ctx->methodDesc;

    gil = PyGILState_Ensure();
    pMethod = PyObject_GetAttrString(proxy->m_obj, ctx->methodName);
    if (pMethod == NULL) goto Cleanup;

    pTuple = PyTuple_New(ctx->paramCount);
    if (pTuple == NULL) goto Cleanup;

    for (i = 0; i < ctx->paramCount; i++) {
        pArg = NULL;
        if (pIMethod->pVTbl->GetParamAtIndex(pIMethod, i, &pIParam) == 0 && pIParam != NULL) {
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            flags = pIParam->pVTbl->get_Flags(pIParam);
            ParamToPyObject(args[i + 1], typeTag, flags, &pArg);
        }
        if (pArg == NULL) {
            Py_INCREF(Py_None);
            pArg = Py_None;
        }
        PyTuple_SetItem(pTuple, i, pArg); /* steals reference */
    }

    if (pIMethod->pVTbl->get_Result(pIMethod, &pIParam) == 0 && pIParam != NULL) {
        pIParam->pVTbl->get_Type(pIParam, &resultTag);
    }

    CallPythonMethod(proxy, pMethod, pTuple, resultTag, ret);

    /* Sweep OUT-flagged params back into the C-side slots. */
    for (i = 0; i < ctx->paramCount; i++) {
        if (pIMethod->pVTbl->GetParamAtIndex(pIMethod, i, &pIParam) != 0 || pIParam == NULL) {
            continue;
        }
        flags = pIParam->pVTbl->get_Flags(pIParam);
        if (!(flags & ECO_PARAM_OUT)) {
            continue;
        }
        pIParam->pVTbl->get_Type(pIParam, &typeTag);
        PyObjectToParam(proxy, PyTuple_GetItem(pTuple, i), typeTag, &args[i + 1]);
    }

Cleanup:
    if (PyErr_Occurred()) PyErr_Clear();
    Py_XDECREF(pTuple);
    Py_XDECREF(pMethod);
    PyGILState_Release(gil);
}


/* -------------------------------------------------------------------------
 * EcoPythonComponentContext — construction
 * ------------------------------------------------------------------------- */

static EcoPythonComponentContext* CreateEcoPythonComponentContext(PyObject* pObj, IEcoInterfaceDescriptor1* pIDesc, IEcoMemoryAllocator1* pIMem, IEcoTypeLib1* pITypeLib) {
    EcoPythonComponentContext* proxy = NULL;
    IEcoMethodDescriptor1* pIMethod = NULL;
    IEcoParamDescriptor1* pIParam = NULL;
    void* execAddr = NULL;
    uint16_t typeTag = 0;
    uint16_t mIndex = 0;
    uint16_t mCount = 0;
    uint8_t flags = 0;
    uint8_t pCount = 0;
    uint8_t pIndex = 0;

    if (pObj == NULL || pIMem == NULL) return NULL;

    proxy = (EcoPythonComponentContext*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoPythonComponentContext));
    if (proxy == NULL) return NULL;

    if (pIDesc != NULL) mCount = pIDesc->pVTbl->get_MethodCount(pIDesc);
    proxy->m_pVTbl = (void**) pIMem->pVTbl->Alloc(pIMem, sizeof(void*) * (mCount + IECOUNKNOWN_METHOD_COUNT));
    if (proxy->m_pVTbl == NULL) {
        pIMem->pVTbl->Free(pIMem, proxy);
        return NULL;
    }

    proxy->m_cRef = 1;
    proxy->m_pIMem = pIMem;
    proxy->m_pITypeLib = pITypeLib;
    proxy->m_pIDesc = pIDesc;
    Py_INCREF(pObj);
    proxy->m_obj = pObj;

    proxy->m_pVTbl[IECOUNKNOWN_METHOD_COUNT - 3] = (void*) EcoPythonComponentContext_IEcoUnknown_QueryInterface;
    proxy->m_pVTbl[IECOUNKNOWN_METHOD_COUNT - 2] = (void*) EcoPythonComponentContext_IEcoUnknown_AddRef;
    proxy->m_pVTbl[IECOUNKNOWN_METHOD_COUNT - 1] = (void*) EcoPythonComponentContext_IEcoUnknown_Release;
    if (mCount == 0) return proxy;

    proxy->m_pListMethods = (EcoPythonMethodContext*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoPythonMethodContext) * mCount);
    if (proxy->m_pListMethods == NULL) {
        proxy->m_cRef = 1;
        EcoPythonComponentContext_IEcoUnknown_Release((IEcoUnknownPtr_t) proxy);
        return NULL;
    }

    for (mIndex = 0; mIndex < mCount; mIndex++) {
        EcoPythonMethodContext* ctx = &proxy->m_pListMethods[mIndex];
        ffi_status status = FFI_OK;
        ffi_type* retType = &ffi_type_void;

        if (pIDesc->pVTbl->get_MethodAtIndex(pIDesc, mIndex, &pIMethod) != 0 || pIMethod == NULL) {
            continue;
        }
        ctx->component = proxy;
        ctx->methodIndex = mIndex;
        ctx->methodDesc = pIMethod;
        pIMethod->pVTbl->get_Name(pIMethod, &ctx->methodName);
        pCount = pIMethod->pVTbl->get_ParamCount(pIMethod);
        ctx->paramCount = pCount;

        ctx->argTypes = (ffi_type**)pIMem->pVTbl->Alloc(pIMem, sizeof(ffi_type*) * (pCount + 1));
        if (ctx->argTypes == NULL) continue;
    
        *ctx->argTypes = &ffi_type_pointer; /* `me` self */
        for (pIndex = 0; pIndex < pCount; pIndex++) {
            ctx->argTypes[pIndex + 1] = &ffi_type_pointer;
            if (pIMethod->pVTbl->GetParamAtIndex(pIMethod, pIndex, &pIParam) != 0 || pIParam == NULL) {
                continue;
            }
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            flags = pIParam->pVTbl->get_Flags(pIParam);
            if (flags & ECO_PARAM_OUT) {
                ctx->argTypes[pIndex + 1] = &ffi_type_pointer;
            } else if (flags & ECO_PARAM_IN) {
                ctx->argTypes[pIndex + 1] = GetFfiType(typeTag);
            }
        }

        if (pIMethod->pVTbl->get_Result(pIMethod, &pIParam) == 0 && pIParam != NULL) {
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            retType = GetFfiType(typeTag);
        }

        ctx->closure = (ffi_closure*) ffi_closure_alloc(sizeof(ffi_closure), &execAddr);
        if (ctx->closure == NULL) continue;

        status = ffi_prep_cif(&ctx->cif, FFI_DEFAULT_ABI, (unsigned int)(pCount + 1), retType, ctx->argTypes);
        if (status != FFI_OK) {
            ffi_closure_free(ctx->closure);
            ctx->closure = NULL;
            continue;
        }
        status = ffi_prep_closure_loc(ctx->closure, &ctx->cif, EcoPythonGlobalDispatcher, ctx, execAddr);
        if (status != FFI_OK) {
            ffi_closure_free(ctx->closure);
            ctx->closure = NULL;
            continue;
        }
        proxy->m_pVTbl[mIndex + IECOUNKNOWN_METHOD_COUNT] = execAddr;
    }

    return proxy;
}


/* -------------------------------------------------------------------------
 * IEcoUnknown
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoACOM2Python interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_QueryInterface(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if (IsEqualUGUID(riid, &IID_IEcoACOM2Python) || IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoACOM2Python;
        pCMe->m_pVTblIEcoACOM2Python->AddRef((IEcoACOM2Python*)pCMe);
    } else {
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
 *   AddRef function for the IEcoACOM2Python interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_AddRef(/* in */ IEcoACOM2PythonPtr_t me) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;

    if (me == 0) {
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
 *   Release function for the IEcoACOM2Python interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_Release(/* in */ IEcoACOM2PythonPtr_t me) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;

    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    atomicdecrement_int32_t(&pCMe->m_cRef);
    if (pCMe->m_cRef == 0) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}


/* -------------------------------------------------------------------------
 * IEcoACOM2Python
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   RegisterComponent Function
 * </summary>
 *
 * <description>
 *   Imports the Python module at `pathName`, resolves the attribute
 *   named `className` on it, and stores the resulting class object
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_RegisterComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ char_t* pathName, /* in */ char_t* className, /* in */ const UGUID* rcid) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    PyGILState_STATE gil;
    PyObject* pClass = NULL;
    int16_t result = ERR_ECO_POINTER;

    if (me == 0 || pathName == 0 || className == 0 || rcid == 0) return ERR_ECO_POINTER;
    if (!Py_IsInitialized()) return ERR_ECO_FAIL;

    gil = PyGILState_Ensure();
    result = LoadPythonComponentClass(pathName, className, &pClass);
    PyGILState_Release(gil);
    if (result != ERR_ECO_SUCCESES) return result;

    pCMe->m_pIListComponents->pVTbl->Add(pCMe->m_pIListComponents, (void*) rcid);
    pCMe->m_pIListComponents->pVTbl->Add(pCMe->m_pIListComponents, (void*) pClass);

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   UnRegisterComponent Function
 * </summary>
 *
 * <description>
 *   Removes the (rcid, class) pair from list and drop the reference to the Python class
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_UnRegisterComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* rcid) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    IEcoList1* list = NULL;
    PyObject* pClass = NULL;
    uint32_t index = 0;

    if (me == 0 || rcid == 0) return ERR_ECO_POINTER;

    list = pCMe->m_pIListComponents;
    index = list->pVTbl->IndexOf(list, (void*) rcid);
    if (index == (uint32_t)-1) return ERR_ECO_INVALIDARG;

    /* Pair: [index] = rcid, [index + 1] = class */
    list->pVTbl->RemoveAt(list, index);
    pClass = (PyObject*) list->pVTbl->Item(list, index);
    list->pVTbl->RemoveAt(list, index);

    if (pClass != NULL) {
        PyGILState_STATE gil = PyGILState_Ensure();
        Py_DECREF(pClass);
        PyGILState_Release(gil);
    }

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   QueryComponent Function
 * </summary>
 *
 * <description>
 *   Create an instance of the Python class registered with `rcid`, call its
 *   `QueryInterface` method with `riid` and return a proxy context
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_QueryComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* rcid, /* in */ IEcoUnknownPtr_t pIUnkOuter, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    PyGILState_STATE gil;
    PyObject* pClass = NULL;
    PyObject* pInstance = NULL;
    PyObject* pIid = NULL;
    PyObject* pOut = NULL;
    PyObject* pCallResult = NULL;
    PyObject* pStatusValue = NULL;
    IEcoInterfaceDescriptor1* pIDesc = NULL;
    EcoPythonComponentContext* proxy = NULL;
    int16_t status = ERR_ECO_FAIL;
    uint32_t index = 0;

    if (me == 0 || rcid == 0 || riid == 0 || ppv == 0) return ERR_ECO_POINTER;
    *ppv = NULL;

    /* TODO: Implement aggregation support */
    (void) pIUnkOuter;

    index = pCMe->m_pIListComponents->pVTbl->IndexOf(pCMe->m_pIListComponents, (void*) rcid);
    if (index == (uint32_t)-1) return ERR_ECO_COMPONENT_NOTFOUND;

    pClass = (PyObject*)pCMe->m_pIListComponents->pVTbl->Item(pCMe->m_pIListComponents, index + 1);
    if (pClass == NULL) return ERR_ECO_COMPONENT_NOTFOUND;

    gil = PyGILState_Ensure();

    pInstance = PyObject_CallObject(pClass, NULL);
    if (pInstance == NULL) goto Cleanup;

    pIid = WrapAsUGUIDPtr(riid);
    pOut = WrapAsPtr(WrapAsVoidPtr(NULL));
    if (pIid == NULL || pOut == NULL) goto Cleanup;

    pCallResult = PyObject_CallMethod(pInstance, "QueryInterface", "OO", pIid, pOut);
    if (pCallResult == NULL) goto Cleanup;

    pStatusValue = UnwrapCTypeValue(pCallResult);
    if (pStatusValue == NULL) goto Cleanup;
    status = (int16_t) PyLong_AsLong(pStatusValue);
    if (status != ERR_ECO_SUCCESES) goto Cleanup;

    pIDesc = GetInterfaceDescriptorByUGUID(pCMe->m_pITypeLib, riid);
    proxy = CreateEcoPythonComponentContext(pInstance, pIDesc, pCMe->m_pIMem, pCMe->m_pITypeLib);
    if (proxy == NULL) {
        if (pIDesc != NULL) pIDesc->pVTbl->Release(pIDesc);
        status = ERR_ECO_OUTOFMEMORY;
        goto Cleanup;
    }

    *(EcoPythonComponentContext**) ppv = proxy;
    status = ERR_ECO_SUCCESES;

Cleanup:
    if (PyErr_Occurred()) PyErr_Clear();
    Py_XDECREF(pStatusValue);
    Py_XDECREF(pCallResult);
    Py_XDECREF(pOut);
    Py_XDECREF(pIid);
    Py_XDECREF(pInstance);
    PyGILState_Release(gil);
    return status;
}


/* -------------------------------------------------------------------------
 * Python interpreter helpers
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   BootPythonInterpreter
 * </summary>
 *
 * <description>
 *   Brings up an embedded `CPython` interpreter
 * </description>
 *
 */
static int16_t BootPythonInterpreter(int16_t* pInitialised) {
    PyConfig config;
    PyStatus status;
    const char_t* home = NULL;
    wchar_t* wHome = NULL;
    size_t homeLen = 0;
    size_t converted = 0;

    if (pInitialised == NULL) return ERR_ECO_POINTER;
    *pInitialised = 0;

    if (Py_IsInitialized()) return ERR_ECO_SUCCESES;

    PyConfig_InitPythonConfig(&config);

    home = getenv("PYTHON_HOME");
    if (home != NULL && *home != '\0') {
        homeLen = strlen(home) + 1;
        wHome = (wchar_t*)malloc(homeLen * sizeof(wchar_t));
        if (wHome == NULL) {
            PyConfig_Clear(&config);
            return ERR_ECO_OUTOFMEMORY;
        }
        if (mbstowcs_s(&converted, wHome, homeLen, home, homeLen - 1) != 0) {
            free(wHome);
            PyConfig_Clear(&config);
            return ERR_ECO_FAIL;
        }
        status = PyConfig_SetString(&config, &config.home, wHome);
        free(wHome);
        if (PyStatus_Exception(status)) {
            PyConfig_Clear(&config);
            return ERR_ECO_FAIL;
        }
    }

    status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    if (PyStatus_Exception(status)) {
        return ERR_ECO_FAIL;
    }

    PyEval_SaveThread();

    *pInitialised = 1;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   TeardownPythonInterpreter
 * </summary>
 *
 * <description>
 *   Finalises `CPython`.
 * </description>
 *
 */
static void TeardownPythonInterpreter(int16_t initialised) {
    if (initialised && Py_IsInitialized()) {
        PyGILState_Ensure();
        Py_Finalize();
    }
}

/*
 *
 * <summary>
 *   ImportModuleAttr
 * </summary>
 *
 * <description>
 *   Convenience wrapper for attribute lookups on imported modules
 * </description>
 *
 */
static PyObject* ImportModuleAttr(const char_t* moduleName, const char_t* attrName) {
    PyObject* pModule = NULL;
    PyObject* pAttr = NULL;

    if (moduleName == NULL || attrName == NULL) return NULL;

    pModule = PyImport_ImportModule(moduleName);
    if (pModule == NULL) return NULL;

    pAttr = PyObject_GetAttrString(pModule, attrName);
    Py_DECREF(pModule);
    return pAttr;
}

/*
 *
 * <summary>
 *   LoadPythonComponentClass
 * </summary>
 *
 * <description>
 *   Imports the Python source file at `pathName`
 * </description>
 *
 */
static int16_t LoadPythonComponentClass(const char_t* pathName, const char_t* className, PyObject** ppClass) {
    PyObject* pImportlibUtil = NULL;
    PyObject* pSpecFromFile = NULL;
    PyObject* pModuleFromSpec = NULL;
    PyObject* pSpec = NULL;
    PyObject* pModule = NULL;
    PyObject* pLoader = NULL;
    PyObject* pExecResult = NULL;
    PyObject* pClass = NULL;
    int16_t result = ERR_ECO_FAIL;

    if (pathName == NULL || className == NULL || ppClass == NULL) return ERR_ECO_POINTER;
    *ppClass = NULL;

    /* Add the directory of `pathName` to `sys.path` so sibling modules are importable */
    {
        const char_t* lastSep = NULL;
        const char_t* p = pathName;
        while (*p != '\0') {
            if (*p == '/' || *p == '\\') lastSep = p;
            p++;
        }
        if (lastSep != NULL && lastSep > pathName) {
            PyObject* pSysPath = PySys_GetObject("path");
            PyObject* pDir = PyUnicode_FromStringAndSize(pathName, lastSep - pathName);
            if (pSysPath != NULL && pDir != NULL && PySequence_Contains(pSysPath, pDir) == 0) {
                PyList_Insert(pSysPath, 0, pDir);
            }
            Py_XDECREF(pDir);
            if (PyErr_Occurred()) PyErr_Clear();
        }
    }

    pImportlibUtil = PyImport_ImportModule("importlib.util");
    if (pImportlibUtil == NULL) goto Cleanup;

    pSpecFromFile = PyObject_GetAttrString(pImportlibUtil, "spec_from_file_location");
    if (pSpecFromFile == NULL) goto Cleanup;

    pModuleFromSpec = PyObject_GetAttrString(pImportlibUtil, "module_from_spec");
    if (pModuleFromSpec == NULL) goto Cleanup;

    pSpec = PyObject_CallFunction(pSpecFromFile, "ss", className, pathName);
    if (pSpec == NULL || pSpec == Py_None) {
        result = ERR_ECO_COMPONENT_NOTFOUND;
        goto Cleanup;
    }

    pModule = PyObject_CallFunctionObjArgs(pModuleFromSpec, pSpec, NULL);
    if (pModule == NULL) goto Cleanup;

    pLoader = PyObject_GetAttrString(pSpec, "loader");
    if (pLoader == NULL) goto Cleanup;

    pExecResult = PyObject_CallMethod(pLoader, "exec_module", "O", pModule);
    if (pExecResult == NULL) goto Cleanup;

    pClass = PyObject_GetAttrString(pModule, className);
    if (pClass == NULL) {
        result = ERR_ECO_COMPONENT_NOTFOUND;
        goto Cleanup;
    }

    *ppClass = pClass; /* Reference ownership transfers to the caller. */
    pClass = NULL;
    result = ERR_ECO_SUCCESES;

Cleanup:
    if (PyErr_Occurred()) { PyErr_Clear(); }
    Py_XDECREF(pClass);
    Py_XDECREF(pExecResult);
    Py_XDECREF(pLoader);
    Py_XDECREF(pModule);
    Py_XDECREF(pSpec);
    Py_XDECREF(pModuleFromSpec);
    Py_XDECREF(pSpecFromFile);
    Py_XDECREF(pImportlibUtil);
    return result;
}


/* -------------------------------------------------------------------------
 * Lifecycle (Init / Create / Delete)
 * ------------------------------------------------------------------------- */

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
static int16_t ECOCALLMETHOD initCEcoACOM2Python_566F1CC3(/* in */ CEcoACOM2Python_566F1CC3Ptr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    IEcoInterfaceBus1* pIBus = NULL;
    IEcoInterfaceBus1MemExt* pIMemExt = NULL;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
    int16_t result = ERR_ECO_POINTER;

    if (me == 0) return result;

    pCMe->m_pISys = (IEcoSystem1*) pIUnkSystem;

    /* Interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == NULL) {
        return ERR_ECO_NOBUS;
    }

    /* Resolve the configured memory manager CID, falling back to the default. */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != NULL) {
        rcid = (UGUID*) pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Memory allocator */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    if (result != 0 || pCMe->m_pIMem == NULL) {
        pIBus->pVTbl->Release(pIBus);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Type library */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, (void**) &pCMe->m_pITypeLib);
    if (result != 0 || pCMe->m_pITypeLib == NULL) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }

    /* CID-keyed component registry */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_pIListComponents);
    if (result != 0 || pCMe->m_pIListComponents == NULL) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }

    /* Embedded `CPython` */
    result = BootPythonInterpreter(&pCMe->m_pyInitialised);
    if (result != ERR_ECO_SUCCESES) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }

    pIBus->pVTbl->Release(pIBus);
    return ERR_ECO_SUCCESES;
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
static int16_t ECOCALLMETHOD createCEcoACOM2Python_566F1CC3(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
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
static void ECOCALLMETHOD deleteCEcoACOM2Python_566F1CC3(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = NULL;
    PyObject* pClass = NULL;
    uint32_t count = 0;
    uint32_t i = 0;

    if (pCMe == NULL) return;

    pIMem = pCMe->m_pIMem;
    if (pCMe->m_pIListComponents != NULL) {
        count = pCMe->m_pIListComponents->pVTbl->Count(pCMe->m_pIListComponents);
        for (i = 1; i < count; i += 2) {
            pClass = (PyObject*) pCMe->m_pIListComponents->pVTbl->Item(pCMe->m_pIListComponents, i);
            if (pClass != NULL) Py_DECREF(pClass);
        }
        pCMe->m_pIListComponents->pVTbl->Clear(pCMe->m_pIListComponents);
        pCMe->m_pIListComponents->pVTbl->Release(pCMe->m_pIListComponents);
    }

    TeardownPythonInterpreter(pCMe->m_pyInitialised);

    if (pCMe->m_pITypeLib != NULL) {
        pCMe->m_pITypeLib->pVTbl->Release(pCMe->m_pITypeLib);
    }
    if (pCMe->m_pISys != NULL) {
        pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
    }

    if (pIMem != NULL) {
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}


/* -------------------------------------------------------------------------
 * Static layout
 * ------------------------------------------------------------------------- */

/* IEcoACOM2Python virtual table */
IEcoACOM2PythonVTbl g_x107F4B5C1FEF47CCA7E63167ABB9E0A7VTbl_566F1CC3 = {
    CEcoACOM2Python_566F1CC3_QueryInterface,
    CEcoACOM2Python_566F1CC3_AddRef,
    CEcoACOM2Python_566F1CC3_Release,
    CEcoACOM2Python_566F1CC3_RegisterComponent,
    CEcoACOM2Python_566F1CC3_UnRegisterComponent,
    CEcoACOM2Python_566F1CC3_QueryComponent
};

/* Object instance */
CEcoACOM2Python_566F1CC3 g_xCEcoACOM2Python_566F1CC3 = {
    &g_x107F4B5C1FEF47CCA7E63167ABB9E0A7VTbl_566F1CC3,
    initCEcoACOM2Python_566F1CC3,
    createCEcoACOM2Python_566F1CC3,
    deleteCEcoACOM2Python_566F1CC3,
    1,    /* m_cRef */
    0,    /* m_pIMem */
    0,    /* m_pISys */
    0,    /* m_pITypeLib */
    0,    /* m_pIListComponents */
    0     /* m_pyInitialised */
};
