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
 *   This header describes the implementation of the CEcoACOM2Python_566F1CC3 component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOACOM2PYTHON_H__
#define __C_ECOACOM2PYTHON_H__

#include "IEcoACOM2Python.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoList1.h"
#include "IEcoTypeLib1.h"
#include "IdEcoTypeLib1.h"
#include "IEcoInterfaceDescriptor1.h"
#include "IEcoMethodDescriptor1.h"

/* Use Python's stable ABI (Limited API) since Python 3.6 */
#define Py_LIMITED_API 0x03060000
#include <Python.h>
#include <ffi.h>

typedef struct CEcoACOM2Python_566F1CC3* CEcoACOM2Python_566F1CC3Ptr_t;

typedef struct CEcoACOM2Python_566F1CC3 {

    /* IEcoACOM2Python interface function table */
    IEcoACOM2PythonVTbl* m_pVTblIEcoACOM2Python;

    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/* in */ CEcoACOM2Python_566F1CC3Ptr_t pCMe);

    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Type library used to load `.etl` descriptions of interfaces */
    IEcoTypeLib1* m_pITypeLib;

    /* Flat registry of pairs (UGUID*, PyObject* factory) */
    IEcoList1* m_pIListComponents;

    /* Set to non-zero after a successful `Py_Initialize` */
    int16_t m_pyInitialised;

} CEcoACOM2Python_566F1CC3;


/*
 * <summary>
 *   EcoPythonMethodContext
 * </summary>
 *
 * <description>
 *   Per-method context attached to every libffi closure inside an `EcoPythonComponentContext`.
 * </description>
 */
typedef struct EcoPythonMethodContext {

    /* Back-reference to the parent component context */
    struct EcoPythonComponentContext* component;

    /* Method index in the interface */
    uint16_t methodIndex;
    
    /* Method descriptor from the type library */
    IEcoMethodDescriptor1* methodDesc;
    
    /* Method name in the interface */
    char_t* methodName;

    /* libffi closure and call interface */
    ffi_closure* closure;
    ffi_cif cif;
    ffi_type** argTypes;

    /* Count of parameters */
    uint8_t paramCount;

} EcoPythonMethodContext;


/*
 * <summary>
 *   EcoPythonComponentContext
 * </summary>
 *
 * <description>
 *   C-side identity of a single Python component instance.
 * </description>
 */
typedef struct EcoPythonComponentContext {
    
    /* Virtual function table for the implemented interfaces */
    void** m_pVTbl;
    
    /* Reference counter */
    uint32_t m_cRef;
    
    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;
    
    /* Interface for type library operations */
    IEcoTypeLib1* m_pITypeLib;
    
    /* Interface descriptor of the implemented interface */
    IEcoInterfaceDescriptor1* m_pIDesc;
    
    /* Python object implementing the component's functionality */
    PyObject* m_obj;
    
    /* Array of method contexts, one per method in the implemented interface */
    EcoPythonMethodContext* m_pListMethods;

} EcoPythonComponentContext;


/*
 * <summary>
 *   EcoPythonTypeMap
 * </summary>
 *
 * <description>
 *   A mapping from Eco type tags to Python types.
 * </description>
 */
typedef struct EcoPythonTypeMap {

    /* Corresponding format characters from CPython documentation */
    const char_t* pyFormat;

    /* Corresponding Python type name */
    const char_t* pyTypeName;

} EcoPythonTypeMap;

#endif /* __C_ECOACOM2PYTHON_H__ */
