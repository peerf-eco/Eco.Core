/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoTypeLib1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoTypeLib1
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECO_TYPE_LIB_1_H__
#define __I_ECO_TYPE_LIB_1_H__

#include "IEcoBase1.h"
#include "IEcoTypeLib1Builder.h"
#include "IEcoInterfaceDirectory1.h"

/* IEcoTypeLib1 IID = {625A3D0E-F218-438F-AAD3-F85EB8E7E975} */
#ifndef __IID_IEcoTypeLib1
static const UGUID IID_IEcoTypeLib1 = {0x01, 0x10, {0x62, 0x5A, 0x3D, 0x0E, 0xF2, 0x18, 0x43, 0x8F, 0xAA, 0xD3, 0xF8, 0x5E, 0xB8, 0xE7, 0xE9, 0x75}};
#endif /* __IID_IEcoTypeLib1 */

typedef struct IEcoTypeLib1* IEcoTypeLib1Ptr_t;

typedef struct IEcoTypeLib1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoTypeLib1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoTypeLib1Ptr_t me);

    /* IEcoTypeLib1 */

    /* Прямая загрузка: читает файл и возвращает готовый объект Директории */
    int16_t (ECOCALLMETHOD *LoadFile)(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const char_t* path, /* out */ IEcoInterfaceDirectory1** ppIDirectory);

    /* Прямое сохранение: берет объект Директории и пишет его в бинарный файл */
    int16_t (ECOCALLMETHOD *SaveFile)(/* in */ IEcoTypeLib1Ptr_t me, /* in */ const char_t* path, /* in */ IEcoInterfaceDirectory1* pIDirectory);

    /* Создание пустого билдера для ручного наполнения */
    int16_t (ECOCALLMETHOD *CreateBuilder)(/* in */ IEcoTypeLib1Ptr_t me, /* out */ struct IEcoTypeLib1Builder** ppIBuilder);

    /* Низкоуровневый доступ (для специфических задач на MCU) */
    int16_t (ECOCALLMETHOD *Open)(IEcoTypeLib1Ptr_t me, const char_t* path, struct IEcoTypeLib1File** ppIFile);

} IEcoTypeLib1VTbl, *IEcoTypeLib1VTblPtr_t;

interface IEcoTypeLib1 {
    struct IEcoTypeLib1VTbl *pVTbl;
} IEcoTypeLib1;


#endif /* __I_ECO_TYPE_LIB_1_H__ */

