/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1Interface
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoTypeLib1Interface component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOTYPELIB1INTERFACE_H__
#define __C_ECOTYPELIB1INTERFACE_H__

#include "IEcoInterfaceDescriptor1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoList1.h"

typedef struct CEcoTypeLib1Interface_01434A0B* CEcoTypeLib1Interface_01434A0BPtr_t;

typedef struct CEcoTypeLib1Interface_01434A0B {

    /* IEcoInterfaceDescriptor1 interface function table */
    IEcoInterfaceDescriptor1VTbl* m_pVTblIInterface;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/* in */ CEcoTypeLib1Interface_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/* in */ CEcoTypeLib1Interface_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/* in */ CEcoTypeLib1Interface_01434A0BPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    uint16_t m_ParentIndex;
    uint8_t m_Flags;
    IEcoList1* m_pIMethodList;
    IEcoList1* m_pIConstantList;

} CEcoTypeLib1Interface_01434A0B;

#endif /* __C_ECOTYPELIB1INTERFACE_H__ */
