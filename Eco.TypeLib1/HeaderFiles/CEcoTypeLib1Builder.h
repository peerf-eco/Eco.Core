/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Builder
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoTypeLib1Builder component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOTYPELIB1BUILDER_H__
#define __C_ECOTYPELIB1BUILDER_H__

#include "IEcoTypeLib1Builder.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoTypeLib1Builder_01434A0B* CEcoTypeLib1Builder_01434A0BPtr_t;

typedef struct CEcoTypeLib1Builder_01434A0B {

    /* IEcoTypeLib1Builder interface function table */
    IEcoTypeLib1BuilderVTbl* m_pVTblIBuilder;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoTypeLib1Builder_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoTypeLib1Builder_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoTypeLib1Builder_01434A0BPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    IEcoInterfaceBus1* m_pIBus;

} CEcoTypeLib1Builder_01434A0B;

#endif /* __C_ECOTYPELIB1BUILDER_H__ */
