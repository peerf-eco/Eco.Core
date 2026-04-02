/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoTypeLib1File
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoTypeLib1File
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

#ifndef __I_ECO_TYPE_LIB_1_FILE_H__
#define __I_ECO_TYPE_LIB_1_FILE_H__

#include "IEcoBase1.h"

/* IEcoTypeLib1File IID = {5517C25A-A6C8-477D-B237-28583771EA55} */
#ifndef __IID_IEcoTypeLib1File
static const UGUID IID_IEcoTypeLib1File = {0x01, 0x10, {0x55, 0x17, 0xC2, 0x5A, 0xA6, 0xC8, 0x47, 0x7D, 0xB2, 0x37, 0x28, 0x58, 0x37, 0x71, 0xEA, 0x55}};
#endif /* __IID_IEcoTypeLib1File */

typedef struct IEcoTypeLib1File* IEcoTypeLib1FilePtr_t;

typedef struct IEcoTypeLib1FileVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoTypeLib1FilePtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoTypeLib1FilePtr_t me);

    /* IEcoTypeLib1File */
    /* IEcoTypeLib1File: Управление ресурсом */

    /* Открытие физического носителя (path может быть именем файла или адресом в памяти) */
    int16_t (ECOCALLMETHOD *Open)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ const char* path, /* in */ uint32_t mode); 
    int16_t (ECOCALLMETHOD *Close)(/* in */ IEcoTypeLib1FilePtr_t me);

    /* IEcoTypeLib1File: Чтение и навигация (для Bridge и Directory) */

    /* Чтение блока данных фиксированного размера по смещению */
    int16_t (ECOCALLMETHOD *Read)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* in */ uint32_t size, /* out */ voidptr_t buffer);
    
    /* Специализированное чтение строки из data_pool (с учетом нуль-терминатора) */
    int16_t (ECOCALLMETHOD *ReadString)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* in | out */ uint32_t* pSize, /* out */ char* pBuffer);

    /* IEcoTypeLib1File: Запись (для Builder при сохранении) */

    /* Запись блока данных по смещению. Возвращает текущее смещение конца записи. */
    int16_t (ECOCALLMETHOD *Write)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t offset, /* in */ uint32_t size, /* in */ voidptr_t buffer);

    /* Получение/установка текущего размера файла (для выделения места под Data Pool) */
    uint32_t (ECOCALLMETHOD *GetSize)(/* in */ IEcoTypeLib1FilePtr_t me);
    int16_t (ECOCALLMETHOD *SetSize)(/* in */ IEcoTypeLib1FilePtr_t me, /* in */ uint32_t size);

} IEcoTypeLib1FileVTbl, *IEcoTypeLib1FileVTblPtr_t;

interface IEcoTypeLib1File {
    struct IEcoTypeLib1FileVTbl *pVTbl;
} IEcoTypeLib1File;


#endif /* __I_ECO_TYPE_LIB_1_FILE_H__ */

