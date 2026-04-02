/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoInterfaceDirectoryEntry1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoInterfaceDirectoryEntry1
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

#ifndef __I_ECO_INTERFACE_DIRECTORY_ENTRY_1_H__
#define __I_ECO_INTERFACE_DIRECTORY_ENTRY_1_H__

#include "IEcoBase1.h"

/* IEcoInterfaceDirectoryEntry1 IID = {F3D1C80F-34C2-478F-8D93-EB7ED39977E5} */
#ifndef __IID_IEcoInterfaceDirectoryEntry1
static const UGUID IID_IEcoInterfaceDirectoryEntry1 = {0x01, 0x10, {0xF3, 0xD1, 0xC8, 0x0F, 0x34, 0xC2, 0x47, 0x8F, 0x8D, 0x93, 0xEB, 0x7E, 0xD3, 0x99, 0x77, 0xE5}};
#endif /* __IID_IEcoInterfaceDirectoryEntry1 */

typedef struct IEcoInterfaceDirectoryEntry1* IEcoInterfaceDirectoryEntry1Ptr_t;

typedef struct IEcoInterfaceDirectoryEntry1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoInterfaceDirectoryEntry1Ptr_t me);

    /* IEcoInterfaceDirectoryEntry1 */
    int16_t (ECOCALLMETHOD *get_IID)(IEcoInterfaceDirectoryEntry1Ptr_t me, UGUID* pIID);
    int16_t (ECOCALLMETHOD *get_Name)(IEcoInterfaceDirectoryEntry1Ptr_t me, char** ppName);
    int16_t (ECOCALLMETHOD *get_Namespace)(IEcoInterfaceDirectoryEntry1Ptr_t me, char** ppNamespace);
    int16_t (ECOCALLMETHOD *get_Descriptor)(IEcoInterfaceDirectoryEntry1Ptr_t me, struct IEcoInterfaceDescriptor1** ppDescriptor);

} IEcoInterfaceDirectoryEntry1VTbl, *IEcoInterfaceDirectoryEntry1VTblPtr_t;

interface IEcoInterfaceDirectoryEntry1 {
    struct IEcoInterfaceDirectoryEntry1VTbl *pVTbl;
} IEcoInterfaceDirectoryEntry1;


#endif /* __I_ECO_INTERFACE_DIRECTORY_ENTRY_1_H__ */

