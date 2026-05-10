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

/* Use Python's stable ABI (Limited API) since Python 3.6 */
#define Py_LIMITED_API 0x03060000
#include <Python.h>

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

    /* Flat registry of triples (UGUID*, IEcoUnknownPtr_t factory, PyObject* factoryHolder) */
    IEcoList1* m_pIListComponents;

    /* Set to non-zero after a successful `Py_Initialize` */
    int16_t m_pyInitialised;

} CEcoACOM2Python_566F1CC3;

#endif /* __C_ECOACOM2PYTHON_H__ */
