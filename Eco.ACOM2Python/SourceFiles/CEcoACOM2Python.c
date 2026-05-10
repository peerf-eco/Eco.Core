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

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoList1.h"

#include "CEcoACOM2Python.h"


/* Each registry slot occupies three consecutive `IEcoList1` entries. */
#define ECO_REGISTRY_STRIDE   3
#define MAX_SIZE              1000


/* -------------------------------------------------------------------------
 * Forward declarations
 * ------------------------------------------------------------------------- */

static int16_t BootPythonInterpreter(int16_t* pInitialised);
static void TeardownPythonInterpreter(int16_t initialised);
static int16_t LoadPythonFactoryInstance(const char_t* pathName, PyObject** ppModule, IEcoUnknownPtr_t* ppIUnk);
static void ECOCALLMETHOD deleteCEcoACOM2Python_566F1CC3(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe);


/* -------------------------------------------------------------------------
 * IEcoUnknown
 * ------------------------------------------------------------------------- */

static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_QueryInterface(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;

    if (me == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;

    if (IsEqualUGUID(riid, &IID_IEcoACOM2Python) || IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = (void*) me;
        pCMe->m_pVTblIEcoACOM2Python->AddRef(me);
        return ERR_ECO_SUCCESES;
    }

    *ppv = NULL;
    return ERR_ECO_NOINTERFACE;
}


static uint32_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_AddRef(/* in */ IEcoACOM2PythonPtr_t me) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    
    if (me == NULL) return (uint32_t)-1;

    return ++pCMe->m_cRef;
}


static uint32_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_Release(/* in */ IEcoACOM2PythonPtr_t me) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    
    if (me == NULL) return (uint32_t)-1;

    pCMe->m_cRef--;
    if (pCMe->m_cRef == 0) {
        deleteCEcoACOM2Python_566F1CC3((CEcoACOM2Python_566F1CC3*) me);
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
 *   RegisterComponent
 * </summary>
 *
 * <description>
 *   Loads a Python module from `pathName`, calls its `get_component_factory`
 *   attribute, and stores the resulting factory under `rcid`
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_RegisterComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ char_t* pathName, /* in */ const UGUID* rcid) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    PyGILState_STATE gil;
    PyObject* pModule = NULL;
    IEcoUnknownPtr_t pIUnk = NULL;
    int16_t result = ERR_ECO_POINTER;

    if (me == NULL || pathName == NULL || rcid == NULL) return ERR_ECO_POINTER;
    if (!Py_IsInitialized()) return ERR_ECO_FAIL;

    gil = PyGILState_Ensure();
    result = LoadPythonFactoryInstance(pathName, &pModule, &pIUnk);
    PyGILState_Release(gil);
    if (result != ERR_ECO_SUCCESES) return result;

    pCMe->m_pIListComponents->pVTbl->Add(pCMe->m_pIListComponents, (void*) rcid);
    pCMe->m_pIListComponents->pVTbl->Add(pCMe->m_pIListComponents, (void*) pIUnk);
    pCMe->m_pIListComponents->pVTbl->Add(pCMe->m_pIListComponents, (void*) pModule);

    return ERR_ECO_SUCCESES;
}


/*
 *
 * <summary>
 *   UnRegisterComponent
 * </summary>
 *
 * <description>
 *   Removes the registration for `rcid` and drops the reference holding the
 *   Python-side factory alive
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_UnRegisterComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* rcid) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    IEcoList1* list = NULL;
    PyObject* pModule = NULL;
    uint32_t index = 0;

    if (me == NULL || rcid == NULL) return ERR_ECO_POINTER;

    list = pCMe->m_pIListComponents;
    index = list->pVTbl->IndexOf(list, (void*) rcid);
    if (index == (uint32_t)-1) return ERR_ECO_INVALIDARG;

    /* Triple at [index, index + 1, index + 2] = (rcid, IEcoUnknown* of factory, PyObject* of module) */
    list->pVTbl->RemoveAt(list, index);  /* rcid */
    list->pVTbl->RemoveAt(list, index);  /* IEcoUnknown* — owned by Python module, no Release here */
    pModule = (PyObject*) list->pVTbl->Item(list, index);
    list->pVTbl->RemoveAt(list, index);

    if (pModule != NULL) {
        PyGILState_STATE gil = PyGILState_Ensure();
        Py_DecRef(pModule);
        PyGILState_Release(gil);
    }

    return ERR_ECO_SUCCESES;
}


/*
 *
 * <summary>
 *   QueryComponent
 * </summary>
 *
 * <description>
 *   Looks up the Python-side factory registered under `rcid`, queries its
 *   `IEcoComponentFactory` interface and delegates instantiation to
 *   `Alloc(system, outer, riid, ppv)`
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Python_566F1CC3_QueryComponent(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* rcid, /* in */ IEcoUnknownPtr_t pIUnkOuter, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoACOM2Python_566F1CC3* pCMe = (CEcoACOM2Python_566F1CC3*)me;
    IEcoUnknownPtr_t pIUnkFactory = NULL;
    IEcoComponentFactoryPtr_t pIFactory = NULL;
    uint32_t index = 0;
    int16_t status = ERR_ECO_FAIL;

    if (me == NULL || rcid == NULL || riid == NULL || ppv == NULL) return ERR_ECO_POINTER;
    *ppv = NULL;

    index = pCMe->m_pIListComponents->pVTbl->IndexOf(pCMe->m_pIListComponents, (void*) rcid);
    if (index == (uint32_t)-1) return ERR_ECO_COMPONENT_NOTFOUND;

    pIUnkFactory = (IEcoUnknownPtr_t) pCMe->m_pIListComponents->pVTbl->Item(pCMe->m_pIListComponents, index + 1);
    if (pIUnkFactory == NULL) return ERR_ECO_COMPONENT_NOTFOUND;

    status = pIUnkFactory->pVTbl->QueryInterface(pIUnkFactory, &IID_IEcoComponentFactory, (void**) &pIFactory);
    if (status != ERR_ECO_SUCCESES || pIFactory == NULL) return status;

    status = pIFactory->pVTbl->Alloc(pIFactory, (IEcoUnknownPtr_t) pCMe->m_pISys, pIUnkOuter, riid, ppv);

    pIFactory->pVTbl->Release(pIFactory);
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
    const char_t* home = NULL;
    wchar_t* wHome = NULL;
    size_t homeLen = 0;
    size_t i = 0;

    if (pInitialised == NULL) return ERR_ECO_POINTER;
    *pInitialised = 0;

    if (Py_IsInitialized()) return ERR_ECO_SUCCESES;

    home = getenv("PYTHON_HOME");
    if (home != NULL && *home != '\0') {
        homeLen = strlen(home);
        wHome = (wchar_t*)malloc((homeLen + 1) * sizeof(wchar_t));
        if (wHome == NULL) return ERR_ECO_OUTOFMEMORY;
        for (i = 0; i < homeLen; i++) wHome[i] = (wchar_t)(unsigned char)home[i];
        wHome[homeLen] = L'\0';
        Py_SetPythonHome(wHome);
    }

    Py_Initialize();
    if (!Py_IsInitialized()) {
        if (wHome != NULL) free(wHome);
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
 *   LoadPythonFactoryInstance
 * </summary>
 *
 * <description>
 *   Imports the Python source file at `pathName` and calls its
 *   `get_component_factory()` attribute
 * </description>
 *
 */
static int16_t LoadPythonFactoryInstance(const char_t* pathName, PyObject** ppModule, IEcoUnknownPtr_t* ppIUnk) {
    PyObject* pImportlibUtil = NULL;
    PyObject* pSpecFromFile = NULL;
    PyObject* pModuleFromSpec = NULL;
    PyObject* pSpec = NULL;
    PyObject* pModule = NULL;
    PyObject* pLoader = NULL;
    PyObject* pExecResult = NULL;
    PyObject* pAddrLong = NULL;
    PyObject* pFactoryCallable = NULL;
    const char_t* baseName = NULL;
    char_t modName[MAX_SIZE];
    size_t baseLen = 0;
    int16_t result = ERR_ECO_FAIL;

    if (pathName == NULL || ppModule == NULL || ppIUnk == NULL) return ERR_ECO_POINTER;
    *ppModule = NULL;
    *ppIUnk = NULL;

    /* Add the directory of `pathName` to `sys.path` so sibling modules are importable */
    {
        const char_t* lastSep = NULL;
        const char_t* q = pathName;
        while (*q != '\0') {
            if (*q == '/' || *q == '\\') lastSep = q;
            q++;
        }
        if (lastSep != NULL && lastSep > pathName) {
            PyObject* pSysPath = PySys_GetObject("path");
            PyObject* pDir = PyUnicode_FromStringAndSize(pathName, lastSep - pathName);
            if (pSysPath != NULL && pDir != NULL && PySequence_Contains(pSysPath, pDir) == 0) {
                PyList_Insert(pSysPath, 0, pDir);
            }
            Py_DecRef(pDir);
            if (PyErr_Occurred()) PyErr_Clear();
            baseName = lastSep + 1;
        } else {
            baseName = pathName;
        }
    }

    /* Synthesise a module name from the file's basename, stripping a trailing `.py` if present. */
    baseLen = strlen(baseName);
    if (baseLen > 3
        && baseName[baseLen - 3] == '.'
        && (baseName[baseLen - 2] == 'p' || baseName[baseLen - 2] == 'P')
        && (baseName[baseLen - 1] == 'y' || baseName[baseLen - 1] == 'Y')) {
        baseLen -= 3;
    }
    if (baseLen >= sizeof(modName)) baseLen = sizeof(modName) - 1;
    memcpy(modName, baseName, baseLen);
    modName[baseLen] = '\0';

    pImportlibUtil = PyImport_ImportModule("importlib.util");
    if (pImportlibUtil == NULL) goto Cleanup;

    pSpecFromFile = PyObject_GetAttrString(pImportlibUtil, "spec_from_file_location");
    if (pSpecFromFile == NULL) goto Cleanup;

    pModuleFromSpec = PyObject_GetAttrString(pImportlibUtil, "module_from_spec");
    if (pModuleFromSpec == NULL) goto Cleanup;

    pSpec = PyObject_CallFunction(pSpecFromFile, "ss", modName, pathName);
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

    pFactoryCallable = PyObject_GetAttrString(pModule, "get_component_factory");
    if (pFactoryCallable == NULL) {
        result = ERR_ECO_COMPONENT_NOTFOUND;
        goto Cleanup;
    }

    /* `get_component_factory()` returns the raw native address of the singleton. */
    pAddrLong = PyObject_CallObject(pFactoryCallable, NULL);
    if (pAddrLong == NULL) goto Cleanup;

    *ppIUnk = (IEcoUnknownPtr_t) (uintptr_t)PyLong_AsUnsignedLongLong(pAddrLong);
    if (PyErr_Occurred() || *ppIUnk == NULL) {
        result = ERR_ECO_FAIL;
        goto Cleanup;
    }

    *ppModule = pModule; /* Reference ownership transfers to the caller */
    pModule = NULL;
    result = ERR_ECO_SUCCESES;

Cleanup:
    if (PyErr_Occurred()) PyErr_Clear();
    Py_DecRef(pAddrLong);
    Py_DecRef(pFactoryCallable);
    Py_DecRef(pExecResult);
    Py_DecRef(pLoader);
    Py_DecRef(pModule);
    Py_DecRef(pSpec);
    Py_DecRef(pModuleFromSpec);
    Py_DecRef(pSpecFromFile);
    Py_DecRef(pImportlibUtil);
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
static int16_t ECOCALLMETHOD initCEcoACOM2Python_566F1CC3(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    IEcoInterfaceBus1* pIBus = NULL;
    IEcoInterfaceBus1MemExt* pIMemExt = NULL;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;
    int16_t result = ERR_ECO_POINTER;

    if (pCMe == NULL) return result;

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
    PyObject* pModule = NULL;
    uint32_t count = 0;
    uint32_t i = 0;

    if (pCMe == NULL) return;

    pIMem = pCMe->m_pIMem;
    if (pCMe->m_pIListComponents != NULL) {
        count = pCMe->m_pIListComponents->pVTbl->Count(pCMe->m_pIListComponents);
        /* PyObject* of imported module lives at every (ECO_REGISTRY_STRIDE * n + 2) slot. */
        for (i = 2; i < count; i += ECO_REGISTRY_STRIDE) {
            pModule = (PyObject*) pCMe->m_pIListComponents->pVTbl->Item(pCMe->m_pIListComponents, i);
            if (pModule != NULL) {
                PyGILState_STATE gil = PyGILState_Ensure();
                Py_DecRef(pModule);
                PyGILState_Release(gil);
            }
        }
        pCMe->m_pIListComponents->pVTbl->Clear(pCMe->m_pIListComponents);
        pCMe->m_pIListComponents->pVTbl->Release(pCMe->m_pIListComponents);
    }

    TeardownPythonInterpreter(pCMe->m_pyInitialised);

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
    0,    /* m_pIListComponents */
    0     /* m_pyInitialised */
};
