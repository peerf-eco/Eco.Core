/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoParamDescriptor1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoParamDescriptor1
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

#ifndef __I_ECO_PARAM_DESCRIPTOR_1_H__
#define __I_ECO_PARAM_DESCRIPTOR_1_H__

#include "IEcoBase1.h"

/* IEcoParamDescriptor1 IID = {10607C56-73DD-4C5C-840D-F80393F68696} */
#ifndef __IID_IEcoParamDescriptor1
static const UGUID IID_IEcoParamDescriptor1 = {0x01, 0x10, {0x10, 0x60, 0x7C, 0x56, 0x73, 0xDD, 0x4C, 0x5C, 0x84, 0x0D, 0xF8, 0x03, 0x93, 0xF6, 0x86, 0x96}};
#endif /* __IID_IEcoParamDescriptor1 */

typedef struct IEcoParamDescriptor1* IEcoParamDescriptor1Ptr_t;

typedef struct IEcoParamDescriptor1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoParamDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoParamDescriptor1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoParamDescriptor1Ptr_t me);

    /* IEcoParamDescriptor1 */
 
    /* get_/set_ для базовых свойств */
    int16_t (ECOCALLMETHOD *get_Name)(/* in */ IEcoParamDescriptor1Ptr_t me, /* out */ char_t** ppName);
    int16_t (ECOCALLMETHOD *set_Name)(/* in */ IEcoParamDescriptor1Ptr_t me, /* in */ char_t* name);
    int16_t (ECOCALLMETHOD *get_Type)(/* in */ IEcoParamDescriptor1Ptr_t me, /* out */ uint16_t* pTypeTag);
    int16_t (ECOCALLMETHOD *set_Type)(/* in */ IEcoParamDescriptor1Ptr_t me, /* in */ uint16_t typeTag);
    
    /* Работа с флагами через маску */
    uint8_t (ECOCALLMETHOD *get_Flags)(/* in */ IEcoParamDescriptor1Ptr_t me);
    int16_t (ECOCALLMETHOD *set_Flags)(/* in */ IEcoParamDescriptor1Ptr_t me, /* in */ uint8_t flags);

} IEcoParamDescriptor1VTbl, *IEcoParamDescriptor1VTblPtr_t;

interface IEcoParamDescriptor1 {
    struct IEcoParamDescriptor1VTbl *pVTbl;
} IEcoParamDescriptor1;


#endif /* __I_ECO_PARAM_DESCRIPTOR_1_H__ */

