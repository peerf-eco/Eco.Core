/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoConstDescriptor1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoConstDescriptor1
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

#ifndef __I_ECO_CONST_DESCRIPTOR_1_H__
#define __I_ECO_CONST_DESCRIPTOR_1_H__

#include "IEcoBase1.h"

/* IEcoConstDescriptor1 IID = {C2C30046-EE32-4672-858C-295132701507} */
#ifndef __IID_IEcoConstDescriptor1
static const UGUID IID_IEcoConstDescriptor1 = {0x01, 0x10, {0xC2, 0xC3, 0x00, 0x46, 0xEE, 0x32, 0x46, 0x72, 0x85, 0x8C, 0x29, 0x51, 0x32, 0x70, 0x15, 0x07}};
#endif /* __IID_IEcoConstDescriptor1 */

typedef struct IEcoConstDescriptor1* IEcoConstDescriptor1Ptr_t;

typedef struct IEcoConstDescriptor1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoConstDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoConstDescriptor1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoConstDescriptor1Ptr_t me);

    /* IEcoConstDescriptor1: Свойства константы */

    /* Имя константы (Identifier*) */
    int16_t (ECOCALLMETHOD *get_Name)(/* in */ IEcoConstDescriptor1Ptr_t me, /* out */ char** ppName);
    int16_t (ECOCALLMETHOD *set_Name)(/* in */ IEcoConstDescriptor1Ptr_t me, /* in */ const char* name);

    /* Тип данных (TypeDescriptor) */
    int16_t (ECOCALLMETHOD *get_Type)(/* in */ IEcoConstDescriptor1Ptr_t me, /* out */ uint16_t* pTypeTag);
    int16_t (ECOCALLMETHOD *set_Type)(/* in */ IEcoConstDescriptor1Ptr_t me, /* in */ uint16_t typeTag);

    /* Значение константы. Buffer должен быть достаточного размера для хранения типа. */
    int16_t (ECOCALLMETHOD *get_Value)(/* in */ IEcoConstDescriptor1Ptr_t me, /* in | out */ uint32_t* pSize, /* out */ voidptr_t pValue);
    int16_t (ECOCALLMETHOD *set_Value)(/* in */ IEcoConstDescriptor1Ptr_t me, /* in */ uint32_t size, /* in */ voidptr_t pValue);

} IEcoConstDescriptor1VTbl, *IEcoConstDescriptor1VTblPtr_t;

interface IEcoConstDescriptor1 {
    struct IEcoConstDescriptor1VTbl *pVTbl;
} IEcoConstDescriptor1;


#endif /* __I_ECO_CONST_DESCRIPTOR_1_H__ */

