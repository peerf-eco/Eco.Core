/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoSystemInformation1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoSystemInformation1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_SYSTEM_INFORMATION_1_H__
#define __C_ECO_SYSTEM_INFORMATION_1_H__

#include "IEcoSystemInformation1.h"

typedef struct CEcoSystemInformation1 {

    /* Таблица функций интерфейса IEcoSystemInformation1 */
    IEcoSystemInformation1VTbl* m_pVTblIInfo;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    //IEcoMemoryAllocator1* m_pIMem;

    /* Данные экземпляря */
    char_t* m_Name;
    char_t* m_Id;

} CEcoSystemInformation1, *CEcoSystemInformation1Ptr;

/* Создание экземпляра */
int16_t createCEcoSystemInformation1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoSystemInformation1** ppIInfo);
/* Удаление */
void deleteCEcoSystemInformation1(/* in */ IEcoSystemInformation1* pIInfo);


#endif /* __C_ECO_SYSTEM_INFORMATION_1_H__ */
