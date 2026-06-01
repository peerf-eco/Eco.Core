
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTypeLib1Annotation
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CEcoTypeLib1Annotation
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECOTYPELIB1ANNOTATION_H__
#define __C_ECOTYPELIB1ANNOTATION_H__

#include "IEcoAnnotationDescriptor1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"

typedef struct CEcoTypeLib1Annotation_01434A0B* CEcoTypeLib1Annotation_01434A0BPtr_t;

typedef struct CEcoTypeLib1Annotation_01434A0B {

    /* Таблица функций интерфейса IEcoAnnotationDescriptor1 */
    IEcoAnnotationDescriptor1VTbl* m_pVTblIAnnotation;

    /* Инициализация экземпляра */
    int16_t (ECOCALLMETHOD *Init)(/* in */ CEcoTypeLib1Annotation_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Создание экземпляра */
    int16_t (ECOCALLMETHOD *Create)(/* in */ CEcoTypeLib1Annotation_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Удаление */
    void (ECOCALLMETHOD *Delete)(/* in */ CEcoTypeLib1Annotation_01434A0BPtr_t pCMe);

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Данные экземпляра */
    char_t* m_Key;
    char_t* m_Value;

} CEcoTypeLib1Annotation_01434A0B;

#endif /* __C_ECOTYPELIB1ANNOTATION_H__ */

