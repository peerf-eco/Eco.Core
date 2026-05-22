/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1DirectoryEntry
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the CEcoTypeLib1DirectoryEntry component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOTYPELIB1DIRECTORYENTRY_H__
#define __C_ECOTYPELIB1DIRECTORYENTRY_H__

#include "IEcoInterfaceDirectoryEntry1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoInterfaceDescriptor1.h"

typedef struct CEcoTypeLib1DirectoryEntry_01434A0B* CEcoTypeLib1DirectoryEntry_01434A0BPtr_t;

typedef struct CEcoTypeLib1DirectoryEntry_01434A0B {

    /* IEcoInterfaceDirectoryEntry1 interface function table */
    IEcoInterfaceDirectoryEntry1VTbl* m_pVTblIEntry;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/* in */ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/* in */ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/* in */ CEcoTypeLib1DirectoryEntry_01434A0BPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    UGUID m_IID;
    char_t* m_Name;
    char_t* m_Namespace;
    IEcoInterfaceDescriptor1* m_pIDescriptor;

} CEcoTypeLib1DirectoryEntry_01434A0B;

#endif /* __C_ECOTYPELIB1DIRECTORYENTRY_H__ */
