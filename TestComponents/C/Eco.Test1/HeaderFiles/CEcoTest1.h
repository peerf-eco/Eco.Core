
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTest1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoTest1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOTEST1_H__
#define __C_ECOTEST1_H__

#include "IEcoTest1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoTest1_816BDCCA* CEcoTest1_816BDCCAPtr_t;

typedef struct CEcoTest1_816BDCCA {

    /* Таблица функций интерфейса IEcoTest1 */
    IEcoTest1VTbl* m_pVTblIEcoTest1;


    /* Инициализация экземпляра */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoTest1_816BDCCAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Создание экземпляра */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoTest1_816BDCCAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Удаление */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoTest1_816BDCCAPtr_t pCMe);


    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Последнее значение, полученное методом TestVoid (для проверки в Java-тесте) */
    int32_t m_lastVoidInput;

} CEcoTest1_816BDCCA;

#endif /* __C_ECOTEST1_H__ */

