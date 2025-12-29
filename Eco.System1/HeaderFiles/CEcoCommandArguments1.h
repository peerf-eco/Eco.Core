/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoCommandArguments1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoCommandArguments1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_COMMAND_ARGUMENTS_1_H__
#define __C_ECO_COMMAND_ARGUMENTS_1_H__

#include "IEcoCommandArguments1.h"

typedef struct CEcoCommandArguments1 {

    /* Таблица функций интерфейса IEcoCommandArguments1 */
    IEcoCommandArguments1VTbl* m_pVTblIArgs;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    //IEcoMemoryAllocator1* m_pIMem;

    /* Интерфейс для работы со строкой */
    //IEcoString1* m_pIString;

    /* Интерфейс для работы со списком */
    //IEcoList1* m_pIList;

} CEcoCommandArguments1, *CEcoCommandArguments1Ptr;

/* Создание экземпляра */
int createCEcoCommandArguments1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoCommandArguments1** ppIArgs);
/* Удаление */
void deleteCEcoCommandArguments1(/* in */ IEcoCommandArguments1* pIArgs);

#endif /* __C_ECO_COMMAND_ARGUMENTS_1_H__ */
