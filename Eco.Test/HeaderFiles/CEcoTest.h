/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTest
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoTest
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_TEST_H__
#define __C_ECO_TEST_H__

#include "IEcoTest.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoTest {
    /* Таблица функций интерфейса IEcoTest */
    IEcoTestVTbl* m_pVTblIX;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

} CEcoTest, *CEcoTestPtr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoTest(/*in*/ IEcoTestPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoTest(/* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter, /* out */ IEcoTestPtr_t* ppIX);
/* Удаление */
void ECOCALLMETHOD deleteCEcoTest(/* in */ IEcoTestPtr_t pIX);

#endif /* __C_ECO_TEST_H__ */
