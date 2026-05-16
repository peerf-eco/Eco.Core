/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTypeLib1Annotation_01434A0B
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoTypeLib1Annotation_01434A0B
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoTypeLib1Annotation.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoAnnotationDescriptor1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_QueryInterface(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoAnnotationDescriptor1) ) {
        *ppv = &pCMe->m_pVTblIAnnotation;
        pCMe->m_pVTblIAnnotation->AddRef((IEcoAnnotationDescriptor1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIAnnotation;
        pCMe->m_pVTblIAnnotation->AddRef((IEcoAnnotationDescriptor1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoAnnotationDescriptor1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_AddRef(/* in */ IEcoAnnotationDescriptor1Ptr_t me) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    /* Проверка указателя */
    if ( me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoAnnotationDescriptor1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_Release(/* in */ IEcoAnnotationDescriptor1Ptr_t me) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    /* Проверка указателя */
    if ( me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;
    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция get_Key
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_get_Key(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* out */ char_t** ppKey) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    if (me == 0 || ppKey == 0) {
        return ERR_ECO_POINTER;
    }

    *ppKey = pCMe->m_Key;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция set_Key
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_set_Key(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ char_t* key) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Key = key;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция get_Value
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_get_Value(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* out */ char_t** ppValue) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    if (me == 0 || ppValue == 0) {
        return ERR_ECO_POINTER;
    }

    *ppValue = pCMe->m_Value;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция set_Value
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTypeLib1Annotation_01434A0B_set_Value(/* in */ IEcoAnnotationDescriptor1Ptr_t me, /* in */ char_t* value) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    pCMe->m_Value = value;
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD initCEcoTypeLib1Annotation_01434A0B(/*in*/ CEcoTypeLib1Annotation_01434A0BPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTypeLib1Annotation_01434A0B* pCMe = (CEcoTypeLib1Annotation_01434A0B*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;	

    /* Проверка указателей */
    if ( me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Проверка */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);
	
    return result;
}

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD createCEcoTypeLib1Annotation_01434A0B(/* in */ CEcoTypeLib1Annotation_01434A0BPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;
	
    /* Проверка указателей */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
static void ECOCALLMETHOD deleteCEcoTypeLib1Annotation_01434A0B(/* in */ CEcoTypeLib1Annotation_01434A0BPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if ( pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}


/* Виртуальная таблица IEcoAnnotationDescriptor1 */
IEcoAnnotationDescriptor1VTbl g_x0E5A9C4503E14B13B48ADA043835D696VTbl_01434A0B = {
    CEcoTypeLib1Annotation_01434A0B_QueryInterface,
    CEcoTypeLib1Annotation_01434A0B_AddRef,
    CEcoTypeLib1Annotation_01434A0B_Release,
    CEcoTypeLib1Annotation_01434A0B_get_Key,
    CEcoTypeLib1Annotation_01434A0B_set_Key,
    CEcoTypeLib1Annotation_01434A0B_get_Value,
    CEcoTypeLib1Annotation_01434A0B_set_Value
};

/* Экземпляр объекта */
CEcoTypeLib1Annotation_01434A0B g_xCEcoTypeLib1Annotation_01434A0B = {
    &g_x0E5A9C4503E14B13B48ADA043835D696VTbl_01434A0B,
    initCEcoTypeLib1Annotation_01434A0B,
    createCEcoTypeLib1Annotation_01434A0B,
    deleteCEcoTypeLib1Annotation_01434A0B,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0, /* m_Key */
    0  /* m_Value */
};