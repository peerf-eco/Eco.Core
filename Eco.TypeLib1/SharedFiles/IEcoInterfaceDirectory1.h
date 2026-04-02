/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoInterfaceDirectory1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoInterfaceDirectory1
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECO_INTERFACE_DIRECTORY_1_H__
#define __I_ECO_INTERFACE_DIRECTORY_1_H__

#include "IEcoBase1.h"
#include "IEcoInterfaceDirectoryEntry1.h"

/* IEcoInterfaceDirectory1 IID = {9E7246B8-9BCB-46A2-A016-B92DC3E97F27} */
#ifndef __IID_IEcoInterfaceDirectory1
static const UGUID IID_IEcoInterfaceDirectory1 = {0x01, 0x10, {0x9E, 0x72, 0x46, 0xB8, 0x9B, 0xCB, 0x46, 0xA2, 0xA0, 0x16, 0xB9, 0x2D, 0xC3, 0xE9, 0x7F, 0x27}};
#endif /* __IID_IEcoInterfaceDirectory1 */

typedef struct IEcoInterfaceDirectory1* IEcoInterfaceDirectory1Ptr_t;

typedef struct IEcoInterfaceDirectory1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoInterfaceDirectory1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoInterfaceDirectory1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoInterfaceDirectory1Ptr_t me);

    /* IEcoInterfaceDirectory1 */

    /* Работа с записями (InterfaceDirectoryEntry) */
    int16_t (ECOCALLMETHOD *AddEntry)(/* in */ IEcoInterfaceDirectory1Ptr_t me, uint16_t index, /* in */ struct IEcoInterfaceDirectoryEntry1* pIEntry);
    uint16_t (ECOCALLMETHOD *get_NumInterfaces)(/* in */ IEcoInterfaceDirectory1Ptr_t me);
    int16_t (ECOCALLMETHOD *GetEntryAtIndex)(/* in */ IEcoInterfaceDirectory1Ptr_t me, uint16_t index, IEcoInterfaceDirectoryEntry1** ppEntry);
    int16_t (ECOCALLMETHOD *GetEntryByIID)(/* in */ IEcoInterfaceDirectory1Ptr_t me, const UGUID* riid, IEcoInterfaceDirectoryEntry1** ppEntry);

     /* Работа с аннотациями */
    uint16_t (ECOCALLMETHOD *get_AnnotationCount)(/* in */ IEcoInterfaceDirectory1Ptr_t me);
    int16_t (ECOCALLMETHOD *GetAnnotationAtIndex)(/* in */ IEcoInterfaceDirectory1Ptr_t me, uint16_t index, struct IEcoTypeLibAnnotation1** ppAnnotation);
    int16_t (ECOCALLMETHOD *GetAnnotationByKey)(/* in */ IEcoInterfaceDirectory1Ptr_t me, const char* key, char** ppValue);
   
} IEcoInterfaceDirectory1VTbl, *IEcoInterfaceDirectory1VTblPtr_t;

interface IEcoInterfaceDirectory1 {
    struct IEcoInterfaceDirectory1VTbl *pVTbl;
} IEcoInterfaceDirectory1;


#endif /* __I_ECO_INTERFACE_DIRECTORY_1_H__ */

