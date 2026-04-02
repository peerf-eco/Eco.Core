/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Method
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoTypeLib1Method component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOTYPELIB1METHOD_H__
#define __C_ECOTYPELIB1METHOD_H__

#include "IEcoMethodDescriptor1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoTypeLib1Method_01434A0B* CEcoTypeLib1Method_01434A0BPtr_t;

typedef struct CEcoTypeLib1Method_01434A0B {

    /* IEcoMethodDescriptor1 interface function table */
    IEcoMethodDescriptor1VTbl* m_pVTblIMethod;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoTypeLib1Method_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoTypeLib1Method_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoTypeLib1Method_01434A0BPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    char_t* m_Name;

} CEcoTypeLib1Method_01434A0B;

#endif /* __C_ECOTYPELIB1METHOD_H__ */
