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
 *   Данный исходный код описывает реализацию интерфейсов CEcoTest
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
#include "CEcoTest.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoTest
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoTest_QueryInterface(/* in */ IEcoTestPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoTest)) {
        *ppv = &pCMe->m_pVTblIX;
        pCMe->m_pVTblIX->AddRef((IEcoTest*)pCMe);
    }
    else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIX;
        pCMe->m_pVTblIX->AddRef((IEcoTest*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoTest
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTest_AddRef(/* in */ IEcoTestPtr_t me) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
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
 *   Функция Release для интерфейса IEcoTest
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoTest_Release(/* in */ IEcoTestPtr_t me) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCEcoTest((IEcoTest*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 * <сводка>
 *   Функция Addition
 * </сводка>
 *
 * <описание>
 *   Сложение двух целых чисел
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_Addition(/* in */ IEcoTestPtr_t me, /* in */ int32_t left, /* in */ int32_t right) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return left + right;
}

/*
 * <сводка>
 *   Функция Negation
 * </сводка>
 *
 * <описание>
 *   Отрицание целого числа
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_Negation(/* in */ IEcoTestPtr_t me, /* in */ int32_t number) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return -number;
}

/*
 * <сводка>
 *   Функция ScalarMul
 * </сводка>
 *
 * <описание>
 *   Умножение двух вещественных чисел (double)
 * </описание>
 */
static double_t ECOCALLMETHOD CEcoTest_Multiplication(/* in */ IEcoTestPtr_t me, /* in */ double_t left, /* in */ double_t right) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return left * right;
}

/*
 * <сводка>
 *   Функция SumArray
 * </сводка>
 *
 * <описание>
 *   Подсчёт суммы элементов массива int32_t
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_SumArray(/* in */ IEcoTestPtr_t me, /* in */ int32_t* arr, /* in */ uint32_t size) {
    CEcoTest* pCMe = (CEcoTest*)me;
    int32_t sum = 0;
    uint32_t idx = 0;

    /* Проверка указателя */
    if (me == 0 || arr == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }

    for (idx = 0; idx < size; ++idx) {
        sum += arr[idx];
    }

    return sum;
}

/*
 * <сводка>
 *   Функция FillArray
 * </сводка>
 *
 * <описание>
 *   Заполнение массива int32_t указанным значением
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_FillArray(/* in */ IEcoTestPtr_t me, /* out */ int32_t* arr, /* in */ uint32_t size, /* in */ int32_t value) {
    CEcoTest* pCMe = (CEcoTest*)me;
    uint32_t idx;

    /* Проверка указателя */
    if (me == 0 || arr == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }

    for (idx = 0; idx < size; ++idx) {
        arr[idx] = value;
    }
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция SortArray
 * </сводка>
 *
 * <описание>
 *   Сортировка массива int32_t методом пузырька
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_SortArray(/* in */ IEcoTestPtr_t me, /* in, out */ int32_t* arr, /* in */ uint32_t size) {
    CEcoTest* pCMe = (CEcoTest*)me;
    uint32_t i, j;
    int32_t temp;

    /* Проверка указателя */
    if (me == 0 || arr == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }

    for (i = 0; i < size; ++i) {
        for (j = 0; j + 1 < size - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция GetName
 * </сводка>
 *
 * <описание>
 *   Возвращает статическую строку с именем компонента
 * </описание>
 */
static char_t* ECOCALLMETHOD CEcoTest_GetName(/* in */ IEcoTestPtr_t me) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return 0; /* ERR_ECO_POINTER */
    }

    return "EcoTest";
}

/*
 * <сводка>
 *   Функция MakePoint
 * </сводка>
 *
 * <описание>
 *   Создание структуры EcoPoint из двух координат
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_MakePoint(/* in */ IEcoTestPtr_t me, /* in */ int32_t x, /* in */ int32_t y, /* out */ EcoPoint* outPoint) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || outPoint == 0) {
        return ERR_ECO_POINTER;
    }

    outPoint->x = x;
    outPoint->y = y;

    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция CalculateDistance
 * </сводка>
 *
 * <описание>
 *   Квадрат евклидова расстояния между двумя точками
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_CalculateDistance(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint* point, /* in */ EcoPoint* other) {
    CEcoTest* pCMe = (CEcoTest*)me;
    int32_t dx;
    int32_t dy;

    /* Проверка указателя */
    if (me == 0 || point == 0 || other == 0) {
        return ERR_ECO_POINTER;
    }

    dx = point->x - other->x;
    dy = point->y - other->y;
    return dx * dx + dy * dy;
}

/*
 * <сводка>
 *   Функция Translate
 * </сводка>
 *
 * <описание>
 *   Сдвиг точки на (dx, dy) на месте
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_Translate(/* in */ IEcoTestPtr_t me, /* in, out */ EcoPoint* point, /* in */ int32_t dx, /* in */ int32_t dy) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || point == 0) {
        return ERR_ECO_POINTER;
    }

    point->x += dx;
    point->y += dy;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция PackInt
 * </сводка>
 *
 * <описание>
 *   Упаковка целого значения в EcoValue
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_PackInt(/* in */ IEcoTestPtr_t me, /* in */ int32_t value, /* out */ EcoValue* outValue) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || outValue == 0) {
        return ERR_ECO_POINTER;
    }

    outValue->asInt = value;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция PackDouble
 * </сводка>
 *
 * <описание>
 *   Упаковка вещественного значения в EcoValue
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_PackDouble(/* in */ IEcoTestPtr_t me, /* in */ double_t value, /* out */ EcoValue* outValue) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || outValue == 0) {
        return ERR_ECO_POINTER;
    }

    outValue->asDouble = value;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция UnpackInt
 * </сводка>
 *
 * <описание>
 *   Чтение целого значения из EcoValue (union по значению)
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_UnpackInt(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return value.asInt;
}

/*
 * <сводка>
 *   Функция UnpackDouble
 * </сводка>
 *
 * <описание>
 *   Чтение вещественного значения из EcoValue (union по значению)
 * </описание>
 */
static double_t ECOCALLMETHOD CEcoTest_UnpackDouble(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return value.asDouble;
}

/*
 * <сводка>
 *   Функция ReinterpretAsInt
 * </сводка>
 *
 * <описание>
 *   XOR битовых половин union — проверяет, что все байты дошли корректно
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_ReinterpretAsInt(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    return value.asBytes[0] ^ value.asBytes[1];
}

/*
 * <сводка>
 *   Функция MakeRect
 * </сводка>
 *
 * <описание>
 *   Построение прямоугольника из двух точек (передаваемых по значению)
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_MakeRect(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint topLeft, /* in */ EcoPoint bottomRight, /* out */ EcoRect* outRect) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || outRect == 0) {
        return ERR_ECO_POINTER;
    }

    outRect->topLeft = topLeft;
    outRect->bottomRight = bottomRight;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция RectArea
 * </сводка>
 *
 * <описание>
 *   Вычисление площади прямоугольника (|dx| * |dy|)
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_RectArea(/* in */ IEcoTestPtr_t me, /* in */ EcoRect* rect) {
    CEcoTest* pCMe = (CEcoTest*)me;
    int32_t dx;
    int32_t dy;

     /* Проверка указателя */
    if (me == 0 || rect == 0) {
        return ERR_ECO_POINTER;
    }

    dx = rect->bottomRight.x - rect->topLeft.x;
    dy = rect->bottomRight.y - rect->topLeft.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx * dy;
}

/*
 * <сводка>
 *   Функция SumPoints
 * </сводка>
 *
 * <описание>
 *   Покомпонентное сложение массива точек
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_SumPoints(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint* points, /* in */ uint32_t size, /* out */ EcoPoint* outSum) {
    CEcoTest* pCMe = (CEcoTest*)me;
    uint32_t idx;

     /* Проверка указателя */
    if (me == 0 || points == 0 || outSum == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }

    outSum->x = 0;
    outSum->y = 0;
    for (idx = 0; idx < size; ++idx) {
        outSum->x += points[idx].x;
        outSum->y += points[idx].y;
    }
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция MakeVariant
 * </сводка>
 *
 * <описание>
 *   Создание tagged-варианта (модель с union по значению)
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_MakeVariant(/* in */ IEcoTestPtr_t me, /* in */ int32_t tag, /* in */ EcoValue value, /* out */ EcoVariant* outVariant) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || outVariant == 0) {
        return ERR_ECO_POINTER;
    }

    outVariant->tag = tag;
    outVariant->value = value;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция GetVariantTag
 * </сводка>
 *
 * <описание>
 *   Чтение тега варианта
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_GetVariantTag(/* in */ IEcoTestPtr_t me, /* in */ EcoVariant* variant) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || variant == 0) {
        return ERR_ECO_POINTER;
    }

    return variant->tag;
}

/*
 * <сводка>
 *   Функция GetVariantValue
 * </сводка>
 *
 * <описание>
 *   Чтение union-значения варианта (через out-параметр)
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_GetVariantValue(/* in */ IEcoTestPtr_t me, /* in */ EcoVariant* variant, /* out */ EcoValue* outValue) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателя */
    if (me == 0 || variant == 0 || outValue == 0) {
        return ERR_ECO_POINTER;
    }

    *outValue = variant->value;
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция SortArrayWith
 * </сводка>
 *
 * <описание>
 *   Сортировка массива int32_t пузырьком с пользовательским компаратором
 * </описание>
 */
static int16_t ECOCALLMETHOD CEcoTest_SortArrayWith(/* in */ IEcoTestPtr_t me, /* in, out */ int32_t* arr, /* in */ uint32_t size, /* in */ EcoCompareFunc cmp) {
    CEcoTest* pCMe = (CEcoTest*)me;    
    uint32_t i, j;
    int32_t temp;

    /* Проверка указателя */
    if (me == 0 || arr == 0 || cmp == 0 || size == 0) {
        return ERR_ECO_POINTER;
    }

    for (i = 0; i < size; ++i) {
        for (j = 0; j + 1 < size - i; ++j) {
            if (cmp(arr[j], arr[j + 1]) > 0) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return ERR_ECO_SUCCESES;
}

/*
 * <сводка>
 *   Функция StringLength
 * </сводка>
 *
 * <описание>
 *   Возвращает длину C-строки (как strlen), переданной как параметр
 * </описание>
 */
static int32_t ECOCALLMETHOD CEcoTest_StringLength(/* in */ IEcoTestPtr_t me, /* in */ const char_t* str) {
    CEcoTest* pCMe = (CEcoTest*)me;
    int32_t len = 0;

    /* Проверка указателя */
    if (me == 0 || str == 0) {
        return ERR_ECO_POINTER;
    }

    while (str[len] != 0) {
        ++len;
    }
    return len;
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
int16_t ECOCALLMETHOD initCEcoTest(/* in */ IEcoTestPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTest* pCMe = (CEcoTest*)me;

    /* Проверка указателей */
    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    return ERR_ECO_SUCCESES;
}

/* Create Virtual Table */
IEcoTestVTbl g_xABCDEF1234567890ABCDEF1234567890VTblA = {
    CEcoTest_QueryInterface,
    CEcoTest_AddRef,
    CEcoTest_Release,
    CEcoTest_Addition,
    CEcoTest_Negation,
    CEcoTest_Multiplication,
    CEcoTest_SumArray,
    CEcoTest_FillArray,
    CEcoTest_SortArray,
    CEcoTest_GetName,
    CEcoTest_MakePoint,
    CEcoTest_CalculateDistance,
    CEcoTest_Translate,
    CEcoTest_PackInt,
    CEcoTest_PackDouble,
    CEcoTest_UnpackInt,
    CEcoTest_UnpackDouble,
    CEcoTest_ReinterpretAsInt,
    CEcoTest_MakeRect,
    CEcoTest_RectArea,
    CEcoTest_SumPoints,
    CEcoTest_MakeVariant,
    CEcoTest_GetVariantTag,
    CEcoTest_GetVariantValue,
    CEcoTest_SortArrayWith,
    CEcoTest_StringLength
};

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
int16_t ECOCALLMETHOD createCEcoTest(/* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter, /* out */ IEcoTestPtr_t* ppITest) {
    CEcoTest* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppITest == 0 || pIUnkSystem == 0) {
        return result; /* ERR_ECO_POINTER */
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (voidptr_t*)&pISys);
    /* Проверка */
    if (result != 0 || pISys == 0) {
        return ERR_ECO_NOSYSTEM;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (voidptr_t*)&pIBus);
    /* Проверка */
    if (result != 0 || pIBus == 0) {
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_NOBUS;
    }

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (voidptr_t*)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (voidptr_t*) &pIMem);
    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение в случае ошибки */
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoTest*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoTest));
    if (pCMe == 0) {
        /* Освобождение в случае ошибки */
        pIBus->pVTbl->Release(pIBus);
        pISys->pVTbl->Release(pISys);
        return ERR_ECO_OUTOFMEMORY;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoTest */
    pCMe->m_pVTblIX = &g_xABCDEF1234567890ABCDEF1234567890VTblA;

    /* Возврат указателя на интерфейс */
    *ppITest = (IEcoTest*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

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
void ECOCALLMETHOD deleteCEcoTest(/* in */ IEcoTestPtr_t pITest) {
    CEcoTest* pCMe = (CEcoTest*)pITest;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pITest != 0) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if (pCMe->m_pISys != 0) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
