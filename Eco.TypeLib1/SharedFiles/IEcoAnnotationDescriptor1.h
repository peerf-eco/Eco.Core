
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoAnnotationDescriptor1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoAnnotationDescriptor1
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECOANNOTATIONDESCRIPTOR1_H__
#define __I_ECOANNOTATIONDESCRIPTOR1_H__

#include "IEcoBase1.h"

/* IEcoAnnotationDescriptor1 IID = {0E5A9C45-03E1-4B13-B48A-DA043835D696} */
#ifndef __IID_IEcoAnnotationDescriptor1
static const UGUID IID_IEcoAnnotationDescriptor1 = {0x01, 0x10, {0x0E, 0x5A, 0x9C, 0x45, 0x03, 0xE1, 0x4B, 0x13, 0xB4, 0x8A, 0xDA, 0x04, 0x38, 0x35, 0xD6, 0x96}};
#endif /* __IID_IEcoAnnotationDescriptor1 */

typedef struct IEcoAnnotationDescriptor1* IEcoAnnotationDescriptor1Ptr_t;

typedef struct IEcoAnnotationDescriptor1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoAnnotationDescriptor1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoAnnotationDescriptor1Ptr_t me);

    /* IEcoAnnotationDescriptor1 */
    int16_t (ECOCALLMETHOD *get_Key)(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* out */ char_t** ppKey);
    int16_t (ECOCALLMETHOD *set_Key)(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ char_t* key);
    int16_t (ECOCALLMETHOD *get_Value)(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* out */ char_t** ppValue);
    int16_t (ECOCALLMETHOD *set_Value)(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ char_t* value);

} IEcoAnnotationDescriptor1VTbl;

interface IEcoAnnotationDescriptor1 {
    struct IEcoAnnotationDescriptor1VTbl *pVTbl;
} IEcoAnnotationDescriptor1;

#endif /* __I_ECOTYPELIBANNOTATION1_H__ */

