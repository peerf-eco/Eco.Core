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


// Mapping of Eco type tags to `CPython format characters` and `Python type names`
EcoPythonTypeMap ECO_TYPE_MAP[] = {
    { 0, "",  ""       }, /* ECO_TYPE_UNDEFINED */
    { 1, "b", "int"    }, /* ECO_TYPE_INT8 */
    { 1, "h", "int"    }, /* ECO_TYPE_INT16 */
    { 1, "i", "int"    }, /* ECO_TYPE_INT32 */
    { 1, "L", "int"    }, /* ECO_TYPE_INT64 */
    { 1, "B", "int"    }, /* ECO_TYPE_UINT8 */
    { 1, "H", "int"    }, /* ECO_TYPE_UINT16 */
    { 1, "I", "int"    }, /* ECO_TYPE_UINT32 */
    { 1, "K", "int"    }, /* ECO_TYPE_UINT64 */
    { 1, "f", "float"  }, /* ECO_TYPE_FLOAT */
    { 1, "d", "float"  }, /* ECO_TYPE_DOUBLE */
    { 1, "B", "bool"   }, /* ECO_TYPE_BOOLEAN */
    { 1, "c", "bytes"  }, /* ECO_TYPE_CHAR */
    { 1, "C", "str"    }, /* ECO_TYPE_WCHAR */
    { 0, "s", "str"    }, /* ECO_TYPE_ASTRING */
    { 0, "u", "str"    }, /* ECO_TYPE_WSTRING */
    { 0, "O", "object" }, /* ECO_TYPE_INTERFACE */
    { 0, "O", "object" }, /* ECO_TYPE_UGUID */
    { 0, "O", "object" }, /* ECO_TYPE_VOIDPTR */
    { 1, "",  "None"   }  /* ECO_TYPE_VOID */
};

// Mapping of Eco type tags to `libffi` types
ffi_type* ECO_FFI_TYPES[] = { 
    &ffi_type_void,       /* ECO_TYPE_UNDEFINED */
    &ffi_type_void,       /* ECO_TYPE_INT8 */
    &ffi_type_sint8,      /* ECO_TYPE_INT16 */
    &ffi_type_sint16,     /* ECO_TYPE_INT32 */
    &ffi_type_sint32,     /* ECO_TYPE_INT64 */
    &ffi_type_sint64,     /* ECO_TYPE_UINT8 */
    &ffi_type_uint8,      /* ECO_TYPE_UINT16 */
    &ffi_type_uint16,     /* ECO_TYPE_UINT32 */
    &ffi_type_uint32,     /* ECO_TYPE_UINT64 */
    &ffi_type_uint64,     /* ECO_TYPE_FLOAT */
    &ffi_type_float,      /* ECO_TYPE_DOUBLE */
    &ffi_type_double,     /* ECO_TYPE_BOOLEAN */
    &ffi_type_uint8,      /* ECO_TYPE_CHAR */
    &ffi_type_sint8,      /* ECO_TYPE_WCHAR */
    &ffi_type_uint16,     /* ECO_TYPE_ASTRING */
    &ffi_type_pointer,    /* ECO_TYPE_WSTRING */
    &ffi_type_pointer,    /* ECO_TYPE_INTERFACE */
    &ffi_type_pointer,    /* ECO_TYPE_UGUID */
    &ffi_type_pointer,    /* ECO_TYPE_VOIDPTR */
    &ffi_type_void        /* ECO_TYPE_VOID */
};


/* -------------------------------------------------------------------------
 * Forward declarations
 * ------------------------------------------------------------------------- */

static int16_t BootPythonInterpreter(int16_t* pInitialised);
static void TeardownPythonInterpreter(int16_t initialised);
static int16_t LoadPythonComponentClass(const char_t* pathName, const char_t* className, PyObject** ppClass);
static PyObject* ImportModuleAttr(const char_t* moduleName, const char_t* attrName);

static char_t* UGUIDPtrToTypeLibFileName(const UGUID* uguid);
static IEcoInterfaceDescriptor1* GetInterfaceDescriptorByUGUID(IEcoTypeLib1* pITypeLib, const UGUID* riid);

static PyObject* UGUIDToPyObject(const UGUID* uguid);
static UGUID PyObjectToUGUID(PyObject* pObj);
static PyObject* WrapAsPtr(const char_t* moduleName, const char_t* className, void* addr);
static PyObject* WrapAsPtrFromCls(PyObject* pPtrCls, PyObject* pTargetCls, void* addr);
static PyObject* MakeEmptyPtrOfPtr(const char_t* moduleName, const char_t* className);
static void* UnwrapVoidPtr(PyObject* pPtr);
static void* UnwrapTypedPtr(PyObject* pPtr);

static PyObject* WrapAsCType(const char_t* className, PyObject* pValue);
static PyObject* UnwrapCTypeValue(PyObject* pCType);

static void ParamToPyObject(void* arg, uint16_t typeTag, uint8_t flags, PyObject** ppArg);
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
 *   UGUIDPtrToTypeLibFileName
 * </summary>
 *
 * <description>
 *   Build the `<HEX>.etl` file name for a UGUID
 * </description>
 *
 */
static char_t* UGUIDPtrToTypeLibFileName(const UGUID* uguid) {
    char_t* result = (char_t*)malloc(uguid->Length * 2 + strlen(".etl") + 1);
    byte_t i = 0;
    byte_t byte = 0;

    for (i = 0; i < uguid->Length; i++) {
        byte = uguid->Data[i] >> 4;
        result[i * 2] = (byte < 10) ? (byte + '0') : ((byte - 10) + 'A');
        byte = uguid->Data[i] & 0xF;
        result[i * 2 + 1] = (byte < 10) ? (byte + '0') : ((byte - 10) + 'A');
    }
    strcat(result, ".etl");
    return result;
}

/*
 *
 * <summary>
 *   GetInterfaceDescriptorByUGUID
 * </summary>
 *
 * <description>
 *   Load the `.etl` produced for `riid` and return the `IEcoInterfaceDescriptor1`
 * </description>
 *
 */
static IEcoInterfaceDescriptor1* GetInterfaceDescriptorByUGUID(IEcoTypeLib1* pITypeLib, const UGUID* riid) {
    IEcoInterfaceDirectory1* pIDirectory = NULL;
    IEcoInterfaceDirectoryEntry1* pIEntry = NULL;
    IEcoInterfaceDescriptor1* pIDesc = NULL;
    char_t* fileName = NULL;
    int16_t result = 0;

    if (pITypeLib == NULL || riid == NULL) return NULL;

    fileName = UGUIDPtrToTypeLibFileName(riid);
    if (fileName == NULL) return NULL;

    result = pITypeLib->pVTbl->LoadFile(pITypeLib, fileName, &pIDirectory);
    free(fileName);
    if (result != 0 || pIDirectory == NULL) return NULL;

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
 *   UGUIDToPyObject
 * </summary>
 *
 * <description>
 *   Build a Python `UGUID` 18-byte instance
 * </description>
 *
 */
static PyObject* UGUIDToPyObject(const UGUID* uguid) {
    PyObject* pUGUIDCls = NULL;
    PyObject* pBytes = NULL;
    PyObject* pInstance = NULL;

    if (uguid == NULL) Py_RETURN_NONE;

    pUGUIDCls = ImportModuleAttr("eco_python2acom.types.guid", "UGUID");
    if (pUGUIDCls == NULL) return NULL;
    pBytes = PyBytes_FromStringAndSize((const char*)uguid, (Py_ssize_t)sizeof(UGUID));
    if (pBytes == NULL) {
        Py_DECREF(pUGUIDCls);
        return NULL;
    }
    pInstance = PyObject_CallMethod(pUGUIDCls, "from_buffer_copy", "O", pBytes);
    Py_DECREF(pBytes);
    Py_DECREF(pUGUIDCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   PyObjectToUGUID
 * </summary>
 *
 * <description>
 *   Read the 18-byte layout of a Python `UGUID` instance back into a C `UGUID`
 * </description>
 *
 */
static UGUID PyObjectToUGUID(PyObject* pObj) {
    UGUID result;
    PyObject* pBytes = NULL;
    char* buffer = NULL;
    Py_ssize_t length = 0;

    memset(&result, 0, sizeof(UGUID));
    if (pObj == NULL || pObj == Py_None) return result;

    pBytes = PyObject_Bytes(pObj);
    if (pBytes == NULL) {
        PyErr_Clear();
        return result;
    }

    if (PyBytes_AsStringAndSize(pBytes, &buffer, &length) == 0 && length == (Py_ssize_t)sizeof(UGUID)) {
        memcpy(&result, buffer, sizeof(UGUID));
    } else {
        PyErr_Clear();
    }
    Py_DECREF(pBytes);
    return result;
}

/* -------------------------------------------------------------------------
 * Wrapping and unwrapping helpers
 * ------------------------------------------------------------------------- */

/*
 *
 * <summary>
 *   WrapAsPtr
 * </summary>
 *
 * <description>
 *   Build a `Ptr[T]` instance whose backing memory is the C address `addr`
 * </description>
 *
 */
static PyObject* WrapAsPtr(const char_t* moduleName, const char_t* className, void* addr) {
    PyObject* pPtrCls = NULL;
    PyObject* pTargetCls = NULL;
    PyObject* pInstance = NULL;

    pPtrCls = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pTargetCls = ImportModuleAttr(moduleName, className);
    if (pPtrCls == NULL || pTargetCls == NULL) goto Cleanup;

    pInstance = WrapAsPtrFromCls(pPtrCls, pTargetCls, addr);

Cleanup:
    Py_XDECREF(pTargetCls);
    Py_XDECREF(pPtrCls);
    return pInstance;
}

/*
 *
 * <summary>
 *   WrapAsPtrFromCls
 * </summary>
 *
 * <description>
 *   Like `WrapAsPtr`, but the `Ptr` metaclass and the target type class
 *   are supplied directly as live `PyObject*` references
 * </description>
 *
 */
static PyObject* WrapAsPtrFromCls(PyObject* pPtrCls, PyObject* pTargetCls, void* addr) {
    PyObject* pSpecialised = NULL;
    PyObject* pInstance = NULL;

    if (pPtrCls == NULL || pTargetCls == NULL) return NULL;

    pSpecialised = PyObject_GetItem(pPtrCls, pTargetCls);
    if (pSpecialised == NULL) return NULL;

    pInstance = PyObject_CallMethod(pSpecialised, "from_address", "k", (uintptr_t)addr);
    Py_DECREF(pSpecialised);
    return pInstance;
}

/*
 *
 * <summary>
 *   MakeEmptyPtrOfPtr
 * </summary>
 *
 * <description>
 *   Build a fresh, NULL-valued `Ptr[Ptr[T]]` instance, where `T` is
 *   resolved as `<moduleName>.<className>`
 * </description>
 *
 */
static PyObject* MakeEmptyPtrOfPtr(const char_t* moduleName, const char_t* className) {
    PyObject* pPtrCls = NULL;
    PyObject* pTargetCls = NULL;
    PyObject* pPtrTarget = NULL;
    PyObject* pPtrPtrTarget = NULL;
    PyObject* pInstance = NULL;

    pPtrCls = ImportModuleAttr("eco_python2acom.types.pointer", "Ptr");
    pTargetCls = ImportModuleAttr(moduleName, className);
    if (pPtrCls == NULL || pTargetCls == NULL) goto Cleanup;

    pPtrTarget = PyObject_GetItem(pPtrCls, pTargetCls);
    if (pPtrTarget == NULL) goto Cleanup;

    pPtrPtrTarget = PyObject_GetItem(pPtrCls, pPtrTarget);
    if (pPtrPtrTarget == NULL) goto Cleanup;

    pInstance = PyObject_CallObject(pPtrPtrTarget, NULL);

Cleanup:
    Py_XDECREF(pPtrPtrTarget);
    Py_XDECREF(pPtrTarget);
    Py_XDECREF(pTargetCls);
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

/*
 *
 * <summary>
 *   UnwrapTypedPtr
 * </summary>
 *
 * <description>
 *   Read the raw address out of a typed `Ptr[T]` instance via `addressof()`
 * </description>
 *
 */
static void* UnwrapTypedPtr(PyObject* pPtr) {
    PyObject* pObj = NULL;
    PyObject* pAddressOf = NULL;
    PyObject* pAddrLong = NULL;
    void* addr = NULL;

    if (pPtr == NULL || pPtr == Py_None) return NULL;

    pObj = PyObject_GetAttrString(pPtr, "obj");
    if (pObj == NULL) {
        PyErr_Clear();
        return NULL;
    }

    pAddressOf = ImportModuleAttr("eco_python2acom.types.utils", "addressof");
    if (pAddressOf == NULL) {
        Py_DECREF(pObj);
        return NULL;
    }
    pAddrLong = PyObject_CallFunctionObjArgs(pAddressOf, pObj, NULL);
    Py_DECREF(pAddressOf);
    Py_DECREF(pObj);
    if (pAddrLong == NULL) {
        PyErr_Clear();
        return NULL;
    }

    addr = PyLong_AsVoidPtr(pAddrLong);
    Py_DECREF(pAddrLong);
    if (PyErr_Occurred()) {
        PyErr_Clear();
        return NULL;
    }
    return addr;
}


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
    EcoPythonTypeMap* ecoType = NULL;

    if (ppArg == NULL) return;
    *ppArg = NULL;
    if (typeTag >= sizeof(ECO_TYPE_MAP) / sizeof(ECO_TYPE_MAP[0])) {
        Py_INCREF(Py_None);
        *ppArg = Py_None;
        return;
    }
    ecoType = &ECO_TYPE_MAP[typeTag];

    /* Primitive paths */
    if (ecoType->isPrimitive) {
        PyObject* pTmp = NULL;
        const char_t* clsName = NULL;

        if (!(flags & ECO_PARAM_IN)) {
            Py_INCREF(Py_None);
            *ppArg = Py_None;
            return;
        }
        switch (typeTag) {
            case ECO_TYPE_INT8:    pTmp = Py_BuildValue("b", *(int8_t*)arg);                       clsName = "Int8";   break;
            case ECO_TYPE_UINT8:   pTmp = Py_BuildValue("B", *(uint8_t*)arg);                      clsName = "UInt8";  break;
            case ECO_TYPE_INT16:   pTmp = Py_BuildValue("h", *(int16_t*)arg);                      clsName = "Int16";  break;
            case ECO_TYPE_UINT16:  pTmp = Py_BuildValue("H", *(uint16_t*)arg);                     clsName = "UInt16"; break;
            case ECO_TYPE_INT32:   pTmp = Py_BuildValue("i", *(int32_t*)arg);                      clsName = "Int32";  break;
            case ECO_TYPE_UINT32:  pTmp = Py_BuildValue("I", *(uint32_t*)arg);                     clsName = "UInt32"; break;
            case ECO_TYPE_INT64:   pTmp = Py_BuildValue("L", *(int64_t*)arg);                      clsName = "Int64";  break;
            case ECO_TYPE_UINT64:  pTmp = Py_BuildValue("K", *(uint64_t*)arg);                     clsName = "UInt64"; break;
            case ECO_TYPE_FLOAT:   pTmp = Py_BuildValue("f", (double)*(float*)arg);                clsName = "Float";  break;
            case ECO_TYPE_DOUBLE:  pTmp = Py_BuildValue("d", *(double*)arg);                       clsName = "Double"; break;
            case ECO_TYPE_BOOLEAN: pTmp = PyBool_FromLong((long)*(uint8_t*)arg);                   clsName = "Bool";   break;
            case ECO_TYPE_CHAR:    pTmp = Py_BuildValue("y#", (const char*)arg, (Py_ssize_t)1);    clsName = "Char";   break;
            case ECO_TYPE_WCHAR:   pTmp = Py_BuildValue("u#", (const wchar_t*)arg, (Py_ssize_t)1); clsName = "WChar";  break;
            default:               break;
        }

        if (pTmp != NULL && clsName != NULL) {
            *ppArg = WrapAsCType(clsName, pTmp);
            Py_DECREF(pTmp);
        }
        if (*ppArg == NULL) {
            Py_INCREF(Py_None);
            *ppArg = Py_None;
        }
        return;
    }

    /* Non-primitive paths */
    switch (typeTag) {
        case ECO_TYPE_ASTRING:
            if (flags & ECO_PARAM_IN) {
                *ppArg = PyUnicode_FromString(*(const char_t**)arg);
            } else {
                *ppArg = PyUnicode_FromString("");
            }
            break;
        case ECO_TYPE_WSTRING:
            if (flags & ECO_PARAM_IN) {
                *ppArg = PyUnicode_FromWideChar(*(const wchar_t**)arg, -1);
            } else {
                *ppArg = PyUnicode_FromWideChar(L"", 0);
            }
            break;
        case ECO_TYPE_INTERFACE:
            if (flags & ECO_PARAM_IN) {
                *ppArg = WrapAsPtr("eco_python2acom.interfaces.unknown", "IEcoUnknown", *(void**)arg);
            } else {
                Py_INCREF(Py_None);
                *ppArg = Py_None;
            }
            break;
        case ECO_TYPE_UGUID:
            if (flags & ECO_PARAM_IN) {
                *ppArg = UGUIDToPyObject(*(const UGUID**)arg);
            } else {
                Py_INCREF(Py_None);
                *ppArg = Py_None;
            }
            break;
        case ECO_TYPE_VOIDPTR:
            if (flags & ECO_PARAM_IN) {
                *ppArg = WrapAsPtr("eco_python2acom.types.core", "Void", *(void**)arg);
            } else {
                Py_INCREF(Py_None);
                *ppArg = Py_None;
            }
            break;
        default:
            Py_INCREF(Py_None);
            *ppArg = Py_None;
            break;
    }
}

/*
 *
 * <summary>
 *   PyObjectToParam
 * </summary>
 *
 * <description>
 *   Reverse of `ParamToPyObject`: write a Python value back into the C slot
 * </description>
 *
 */
static void PyObjectToParam(EcoPythonComponentContext* component, PyObject* pVal, uint16_t typeTag, void** arg) {
    PyObject* pInner = NULL;

    if (arg == NULL || *arg == NULL || pVal == NULL) return;

    switch (typeTag) {
        case ECO_TYPE_INT8:
        case ECO_TYPE_UINT8:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(int8_t**)arg = (int8_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT16:
        case ECO_TYPE_UINT16:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(int16_t**)arg = (int16_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT32:
        case ECO_TYPE_UINT32:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(int32_t**)arg = (int32_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT64:
        case ECO_TYPE_UINT64:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(int64_t**)arg = (int64_t)PyLong_AsLongLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_FLOAT:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(float**)arg = (float)PyFloat_AsDouble(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_DOUBLE:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(double**)arg = PyFloat_AsDouble(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_BOOLEAN:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL) {
                **(uint8_t**)arg = (uint8_t)(PyObject_IsTrue(pInner) ? 1 : 0);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_CHAR:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyBytes_Check(pInner) && PyBytes_Size(pInner) > 0) {
                **(char_t**)arg = PyBytes_AsString(pInner)[0];
            }
            Py_XDECREF(pInner);
            break;
        case ECO_TYPE_WCHAR:
            pInner = UnwrapCTypeValue(pVal);
            if (pInner != NULL && PyUnicode_Check(pInner) && PyUnicode_GetLength(pInner) > 0) {
                wchar_t buf[2];
                if ( PyUnicode_AsWideChar(pInner, buf, 1) >= 0 ) {
                    **(wchar_t**)arg = buf[0];
                }
            }
            Py_XDECREF(pInner);
            break;
        case ECO_TYPE_ASTRING: {
            /* TODO */
            const char* utf8 = PyUnicode_AsUTF8(pVal);
            if (utf8 != NULL) **(const char_t***) arg = utf8;
            break;
        }
        case ECO_TYPE_WSTRING: {
            /* TODO */
            wchar_t* wide = PyUnicode_AsWideCharString(pVal, NULL);
            if (wide != NULL) **(const wchar_t***) arg = wide;
            break;
        }
        case ECO_TYPE_INTERFACE:
            /* TODO */
            **(void***) arg = UnwrapTypedPtr(pVal);
            break;
        case ECO_TYPE_UGUID:
            **(UGUID**)arg = PyObjectToUGUID(pVal);
            break;
        case ECO_TYPE_VOIDPTR:
            **(void***)arg = UnwrapVoidPtr(pVal);
            break;
        default:
            break;
    }

    if (PyErr_Occurred()) PyErr_Clear();
}

/*
 *
 * <summary>
 *   CallPythonMethod
 * </summary>
 *
 * <description>
 *   Invoke method with the prepared args tuple and unpack the result
 *   into the storage according to `typeTag`
 * </description>
 *
 */
static void CallPythonMethod(EcoPythonComponentContext* component, PyObject* pMethod, PyObject* pArgs, uint16_t typeTag, void* ret) {
    PyObject* pResult = NULL;
    PyObject* pInner = NULL;

    if (pMethod == NULL || ret == NULL) return;

    pResult = PyObject_Call(pMethod, pArgs != NULL ? pArgs : PyTuple_New(0), NULL);
    if (pResult == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
            PyErr_Clear();
        }
        return;
    }

    /* Return `None` for void-returning methods */
    if (pResult == Py_None || typeTag == ECO_TYPE_VOID) {
        Py_DECREF(pResult);
        return;
    }

    switch (typeTag) {
        case ECO_TYPE_INT8:
        case ECO_TYPE_UINT8:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(int8_t*)ret = (int8_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT16:
        case ECO_TYPE_UINT16:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(int16_t*)ret = (int16_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT32:
        case ECO_TYPE_UINT32:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(int32_t*)ret = (int32_t)PyLong_AsLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_INT64:
        case ECO_TYPE_UINT64:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(int64_t*)ret = (int64_t)PyLong_AsLongLong(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_FLOAT:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(float*)ret = (float)PyFloat_AsDouble(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_DOUBLE:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(double*)ret = PyFloat_AsDouble(pInner);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_BOOLEAN:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL) {
                *(uint8_t*)ret = (uint8_t)(PyObject_IsTrue(pInner) ? 1 : 0);
                Py_DECREF(pInner);
            }
            break;
        case ECO_TYPE_CHAR:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL && PyBytes_Check(pInner) && PyBytes_Size(pInner) > 0) {
                *(char_t*)ret = PyBytes_AsString(pInner)[0];
            }
            Py_XDECREF(pInner);
            break;
        case ECO_TYPE_WCHAR:
            pInner = UnwrapCTypeValue(pResult);
            if (pInner != NULL && PyUnicode_Check(pInner) && PyUnicode_GetLength(pInner) > 0) {
                wchar_t buf[2];
                if (PyUnicode_AsWideChar(pInner, buf, 1) >= 0) {
                    *(wchar_t*)ret = buf[0];
                }
            }
            Py_XDECREF(pInner);
            break;
        case ECO_TYPE_ASTRING: {
            /* TODO */
            const char* utf8 = PyUnicode_AsUTF8(pResult);
            if (utf8 != NULL) *(const char_t**) ret = utf8;
            break;
        }
        case ECO_TYPE_WSTRING: {
            /* TODO */
            wchar_t* wide = PyUnicode_AsWideCharString(pResult, NULL);
            if (wide != NULL) *(const wchar_t**) ret = wide;
            break;
        }
        case ECO_TYPE_INTERFACE:
            /* TODO */
            *(void**)ret = UnwrapTypedPtr(pResult);
            break;
        case ECO_TYPE_UGUID:
            *(UGUID*)ret = PyObjectToUGUID(pResult);
            break;
        case ECO_TYPE_VOIDPTR:
            *(void**)ret = UnwrapVoidPtr(pResult);
            break;
        default:
            break;
    }

    if (PyErr_Occurred()) PyErr_Clear();
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
    IEcoInterfaceDescriptor1* pIDesc = NULL;
    int16_t status = ERR_ECO_FAIL;

    if (me == NULL || riid == NULL || ppv == NULL) {
        return ERR_ECO_POINTER;
    }
    *ppv = NULL;

    gil = PyGILState_Ensure();

    pIid = WrapAsPtr("eco_python2acom.types.guid", "UGUID", (void*)riid);
    pOut = MakeEmptyPtrOfPtr("eco_python2acom.types.core", "Void");
    if (pIid == NULL || pOut == NULL) goto Cleanup;

    pCallResult = PyObject_CallMethod(proxy->m_obj, "QueryInterface", "OO", pIid, pOut);
    if (pCallResult == NULL) goto Cleanup;

    pStatusValue = UnwrapCTypeValue(pCallResult);
    if (pStatusValue == NULL) goto Cleanup;
    status = (int16_t)PyLong_AsLong(pStatusValue);
    if (status != ERR_ECO_SUCCESES) goto Cleanup;

    /* For a same-component QueryInterface the Python implementation returns the same `self` */
    pIDesc = GetInterfaceDescriptorByUGUID(proxy->m_pITypeLib, riid);
    *(EcoPythonComponentContext**) ppv = CreateEcoPythonComponentContext(proxy->m_obj, pIDesc, proxy->m_pIMem, proxy->m_pITypeLib);
    if (*ppv == NULL) status = ERR_ECO_OUTOFMEMORY;

Cleanup:
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
    }
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

    if (me == NULL) return (uint32_t)-1;

    gil = PyGILState_Ensure();
    pResult = PyObject_CallMethod(proxy->m_obj, "Release", NULL);
    if (pResult != NULL) Py_DECREF(pResult);

    if (PyErr_Occurred()) PyErr_Clear();
    PyGILState_Release(gil);

    if (--proxy->m_cRef == 0) {
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
        gil = PyGILState_Ensure();
        Py_XDECREF(proxy->m_obj);
        PyGILState_Release(gil);
        if (proxy->m_pVTbl != NULL && proxy->m_pIMem != NULL) {
            proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy->m_pVTbl);
        }
        if (proxy->m_pIMem != NULL) {
            proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy);
        }
        return 0;
    }
    return proxy->m_cRef;
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
        PyTuple_SET_ITEM(pTuple, i, pArg); /* steals reference */
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
        PyObjectToParam(proxy, PyTuple_GET_ITEM(pTuple, i), typeTag, &args[i + 1]);
    }

Cleanup:
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
    }
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
    memset(proxy, 0, sizeof(EcoPythonComponentContext));

    if (pIDesc != NULL) mCount = pIDesc->pVTbl->get_MethodCount(pIDesc);

    proxy->m_pVTbl = (void**) pIMem->pVTbl->Alloc(pIMem, sizeof(void*) * (mCount + 3));
    if (proxy->m_pVTbl == NULL) {
        pIMem->pVTbl->Free(pIMem, proxy);
        return NULL;
    }
    memset(proxy->m_pVTbl, 0, sizeof(void*) * (mCount + 3));

    proxy->m_cRef = 1;
    proxy->m_pIMem = pIMem;
    proxy->m_pITypeLib = pITypeLib;
    proxy->m_pIDesc = pIDesc;
    Py_INCREF(pObj);
    proxy->m_obj = pObj;

    proxy->m_pVTbl[0] = (void*) EcoPythonComponentContext_IEcoUnknown_QueryInterface;
    proxy->m_pVTbl[1] = (void*) EcoPythonComponentContext_IEcoUnknown_AddRef;
    proxy->m_pVTbl[2] = (void*) EcoPythonComponentContext_IEcoUnknown_Release;
    if (mCount == 0) return proxy;

    proxy->m_pListMethods = (EcoPythonMethodContext*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoPythonMethodContext) * mCount);
    if (proxy->m_pListMethods == NULL) {
        proxy->m_cRef = 1;
        EcoPythonComponentContext_IEcoUnknown_Release((IEcoUnknownPtr_t) proxy);
        return NULL;
    }
    memset(proxy->m_pListMethods, 0, sizeof(EcoPythonMethodContext) * mCount);

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
    
        ctx->argTypes[0] = &ffi_type_pointer; /* `me` self */
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
                ctx->argTypes[pIndex + 1] = ECO_FFI_TYPES[typeTag];
            }
        }

        if (pIMethod->pVTbl->get_Result(pIMethod, &pIParam) == 0 && pIParam != NULL) {
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            retType = ECO_FFI_TYPES[typeTag];
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
        proxy->m_pVTbl[mIndex + 3] = execAddr;
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
 *   Removes the (rcid, class) pair from list and drop
 *   the reference to the Python class
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

    pIid = WrapAsPtr("eco_python2acom.types.guid", "UGUID", (void*) riid);
    pOut = MakeEmptyPtrOfPtr("eco_python2acom.types.core", "Void");
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
        status = ERR_ECO_OUTOFMEMORY;
        goto Cleanup;
    }

    *(EcoPythonComponentContext**) ppv = proxy;
    status = ERR_ECO_SUCCESES;

Cleanup:
    if (PyErr_Occurred()) {
        PyErr_Print();
        PyErr_Clear();
    }
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
    if (home != NULL && home[0] != '\0') {
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
    if (PyErr_Occurred()) PyErr_Clear();
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
