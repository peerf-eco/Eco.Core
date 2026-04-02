/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoMethodDescriptor1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoMethodDescriptor1
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

#ifndef __I_ECO_METHOD_DESCRIPTOR_1_H__
#define __I_ECO_METHOD_DESCRIPTOR_1_H__

#include "IEcoBase1.h"

/* IEcoMethodDescriptor1 IID = {74D2637A-E0CE-41B1-8876-038E40CBC64E} */
#ifndef __IID_IEcoMethodDescriptor1
static const UGUID IID_IEcoMethodDescriptor1 = {0x01, 0x10, {0x74, 0xD2, 0x63, 0x7A, 0xE0, 0xCE, 0x41, 0xB1, 0x88, 0x76, 0x03, 0x8E, 0x40, 0xCB, 0xC6, 0x4E}};
#endif /* __IID_IEcoMethodDescriptor1 */

typedef struct IEcoMethodDescriptor1* IEcoMethodDescriptor1Ptr_t;

typedef struct IEcoMethodDescriptor1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoMethodDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoMethodDescriptor1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoMethodDescriptor1Ptr_t me);

    /* IEcoMethodDescriptor1 */
 
    /* Интроспекция метода */
    int16_t (ECOCALLMETHOD *get_Name)(/* in */ IEcoMethodDescriptor1Ptr_t me, char** ppName);
    uint8_t (ECOCALLMETHOD *get_Flags)(/* in */ IEcoMethodDescriptor1Ptr_t me);

    /* Управление аргументами */
    int16_t (ECOCALLMETHOD *AddParameter)(/* in */ IEcoMethodDescriptor1Ptr_t me, struct IEcoParamDescriptor1* pIParam);
    uint8_t (ECOCALLMETHOD *get_ParamCount)(/* in */ IEcoMethodDescriptor1Ptr_t me);
    int16_t (ECOCALLMETHOD *GetParamAtIndex)(/* in */ IEcoMethodDescriptor1Ptr_t me, uint8_t index, struct IEcoParamDescriptor1** ppIParam);

    /* Управление результатом (result в MethodDescriptor) */
    int16_t (ECOCALLMETHOD *set_Result)(/* in */ IEcoMethodDescriptor1Ptr_t me, struct IEcoParamDescriptor1* pIParam);
    int16_t (ECOCALLMETHOD *get_Result)(/* in */ IEcoMethodDescriptor1Ptr_t me, struct IEcoParamDescriptor1** ppIParam);

} IEcoMethodDescriptor1VTbl, *IEcoMethodDescriptor1VTblPtr_t;

interface IEcoMethodDescriptor1 {
    struct IEcoMethodDescriptor1VTbl *pVTbl;
} IEcoMethodDescriptor1;


#endif /* __I_ECO_METHOD_DESCRIPTOR_1_H__ */

