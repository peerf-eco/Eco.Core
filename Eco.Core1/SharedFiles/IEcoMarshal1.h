/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoMarshal
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает базовые определения данных
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_MARSHAL_1_H__
#define __I_ECO_MARSHAL_1_H__

/* Поддержка зависимостей */
#include "IEcoBase1.h"

/* IEcoMarshal IID = 00000000-0000-0000-0000-0000000000AA */
#ifndef __IID_IEcoMarshal
static const UGUID IID_IEcoMarshal = { 0x01, 0x10, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA} };
#endif /* __IID_IEcoMarshal */

#ifndef ECO_I_ECO_MARSHAL_1_PTR_T_DEFINED
typedef struct IEcoMarshal* IEcoMarshalPtr_t;
#define ECO_I_ECO_MARSHAL_1_PTR_T_DEFINED
#endif

typedef struct IEcoMarshalVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoMarshalPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoMarshalPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoMarshalPtr_t me);

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *GetUnmarshalClass)(/* in */ IEcoMarshalPtr_t me, /* in */ const UGUID* riid, /* in */ voidptr_t pv, /* in */ uint32_t dwDestContext, /* in */ voidptr_t pvDestContext, /* in */ uint32_t mshlflags, /* out */ CLSID *pCid);
    int16_t (ECOCALLMETHOD *GetMarshalSizeMax)(/* in */ IEcoMarshalPtr_t me, /* in */ const UGUID* riid, /* in */ voidptr_t pv, /* in */ uint32_t dwDestContext, /* in */ voidptr_t pvDestContext, /* in */ uint32_t mshlflags, /* out */ uint32_t *pSize);
    int16_t (ECOCALLMETHOD *MarshalInterface)(/* in */ IEcoMarshalPtr_t me, /* in */ IStream *pStm, /* in */ const UGUID* riid, /* in */  void *pv, /* in */ uint32_t dwDestContext, /* in */ voidptr_t pvDestContext, /* in */ uint32_t mshlflags);
    int16_t (ECOCALLMETHOD *UnmarshalInterface)(/* in */ IEcoMarshalPtr_t me, /* in */ IStream *pStm, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    int16_t (ECOCALLMETHOD *ReleaseMarshalData)( /* in */ IEcoMarshalPtr_t me, /* in */ IStream *pStm);
    int16_t (ECOCALLMETHOD *DisconnectObject)(/* in */ IEcoMarshalPtr_t me, /* in */ uint32_t dwReserved);

} IEcoMarshalVTbl;

interface IEcoMarshal {
    struct IIEcoMarshalVTbl *pVTbl;
} IEcoMarshal;

#endif /* __I_ECO_MARSHAL_1_H__ */
