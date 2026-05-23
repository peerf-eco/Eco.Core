/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTestC1Factory
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию фабрики для компонента
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOTESTC1_FACTORY_H__
#define __C_ECOTESTC1_FACTORY_H__

#include "IEcoSystem1.h"

typedef struct CEcoTestC1_816BDCCAFactory {

    /* Таблица функций интерфейса IEcoComponentFactory */
    IEcoComponentFactoryVTbl* m_pVTblICF;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Данные компонентов для фабрики */
    char_t m_Name[64];
    char_t m_Version[16];
    char_t m_Manufacturer[64];

} CEcoTestC1_816BDCCAFactory;

#endif /* __C_ECOTESTC1_FACTORY_H__ */
