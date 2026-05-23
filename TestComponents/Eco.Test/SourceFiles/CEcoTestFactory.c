/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTestFactory
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

#include "CEcoTest.h"
#include "CEcoTestFactory.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция возвращает указатель на интерфейс
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTestFactory_QueryInterface(IEcoComponentFactoryPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    if (IsEqualUGUID(riid, &IID_IEcoUnknown) || IsEqualUGUID(riid, &IID_IEcoComponentFactory)) {
        *ppv = me;
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    ((IEcoUnknown*)(*ppv))->pVTbl->AddRef((IEcoUnknown*)*ppv);
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция увеличивает количество ссылок на интерфейс
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTestFactory_AddRef(/* in */ IEcoComponentFactoryPtr_t me) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;

    if (me == 0) {
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
 *   Функция уменьшает количество ссылок на интерфейс
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTestFactory_Release(/* in */ IEcoComponentFactoryPtr_t me) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;

    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        // deleteCEcoTestFactory(&pCMe->m_VtblICF);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализирует компонент с параметрами
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTestFactory_Init(/* in */ IEcoComponentFactoryPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ voidptr_t pv) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;
    int16_t result = ERR_ECO_POINTER;

    if (me == 0) {
        return result;
    }

    /* Инициализация компонента */
    result = pCMe->m_pInitInstance(pv, pIUnkSystem);

    return result;
}

/*
 *
 * <сводка>
 *   Функция Alloc
 * </сводка>
 *
 * <описание>
 *   Функция создает компонент
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTestFactory_Alloc(/* in */ IEcoComponentFactoryPtr_t me, /* in */ IEcoUnknownPtr_t pISystem, /* in */ IEcoUnknownPtr_t pIUnknownOuter, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;
    IEcoUnknown* pIUnk = 0;
    int16_t result = ERR_ECO_POINTER;

    if (me == 0) {
        return result;
    }

    /* Агрегирование при условии если IID это IID_IEcoUnknown */
    if ((pIUnknownOuter != 0) && !IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        /* Не поддерживает агрегирование */
        return ERR_ECO_NOAGGREGATION;
    }

    /* Создание компонента */
    result = pCMe->m_pInstance(pISystem, pIUnknownOuter, (void**)&pIUnk);
    if (result != 0 || pIUnk == 0) {
        return result;
    }

    /* Инициализация компонента */
    result = me->pVTbl->Init(me, pISystem, pIUnk);

    /* Получение указателя на интерфейс */
    result = pIUnk->pVTbl->QueryInterface(pIUnk, riid, ppv);

    /* Уменьшение ссылки запрошенной Фабрикой компонентов */
    pIUnk->pVTbl->Release(pIUnk);

    return result;
}

/*
 *
 * <сводка>
 *   Функция get_Name
 * </сводка>
 *
 * <описание>
 *   Функция возвращает наименование компонента
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoTestFactory_get_Name(/* in */ IEcoComponentFactoryPtr_t me) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;

    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Name;
}

/*
 *
 * <сводка>
 *   Функция get_Version
 * </сводка>
 *
 * <описание>
 *   Функция возвращает версию компонента
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoTestFactory_get_Version(/* in */ IEcoComponentFactoryPtr_t me) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;

    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Version;
}

/*
 *
 * <сводка>
 *   Функция get_Manufacturer
 * </сводка>
 *
 * <описание>
 *   Функция возвращает наименование производителя компонента
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoTestFactory_get_Manufacturer(/* in */ IEcoComponentFactoryPtr_t me) {
    CEcoTestFactory* pCMe = (CEcoTestFactory*)me;

    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_Manufacturer;
}

/* Create Virtual Table */
IEcoComponentFactoryVTbl g_x1234567890ABCDEF1234567890ABCDEFFactoryVTbl = {
    CEcoTestFactory_QueryInterface,
    CEcoTestFactory_AddRef,
    CEcoTestFactory_Release,
    CEcoTestFactory_Alloc,
    CEcoTestFactory_Init,
    CEcoTestFactory_get_Name,
    CEcoTestFactory_get_Version,
    CEcoTestFactory_get_Manufacturer
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
CEcoTestFactory g_x1234567890ABCDEF1234567890ABCDEFFactory = {
    &g_x1234567890ABCDEF1234567890ABCDEFFactoryVTbl,
    0,
    (CreateInstance)createCEcoTest,
    (InitInstance)initCEcoTest,
    "Eco.Test\0",
    "1.1.1.1\0",
    "Test\0"
};

#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr() {
    return (IEcoComponentFactory*)&g_x1234567890ABCDEF1234567890ABCDEFFactory;
};
#elif ECO_LIB
IEcoComponentFactory* GetIEcoComponentFactoryPtr_1234567890ABCDEF1234567890ABCDEF = (IEcoComponentFactory*)&g_x1234567890ABCDEF1234567890ABCDEFFactory;
#endif