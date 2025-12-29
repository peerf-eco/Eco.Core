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
 *   Данный исходный код описывает реализацию интерфейсов CEcoCommandArguments1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "CEcoCommandArguments1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
int CEcoCommandArguments1_QueryInterface(/* in */ struct IEcoCommandArguments1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoCommandArguments1* pCMe = 0;

    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoCommandArguments1*)me;

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        //*ppv = &pCMe->m_VtblIArgs;
        //pCMe->m_VtblIArgs.AddRef(&pCMe->m_VtblIArgs);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        //*ppv = &pCMe->m_VtblIArgs;
        //pCMe->m_VtblIArgs.AddRef(&pCMe->m_VtblIArgs);
    }
    else {
        *ppv = 0;
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
unsigned long CEcoCommandArguments1_AddRef(/* in */ struct IEcoCommandArguments1* me) {
    CEcoCommandArguments1* pCMe = 0;

    if (me == 0 ) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoCommandArguments1*)me;

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
unsigned long CEcoCommandArguments1_Release(/* in */ struct IEcoCommandArguments1* me) {
    CEcoCommandArguments1* pCMe = 0;

    if (me == 0 ) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoCommandArguments1*)me;

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        //deleteCEcoCommandArguments1(&pCMe->m_VtblIArgs);
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
 *   Функция
 * </описание>
 *
 */
int CEcoCommandArguments1_Init(/* in */ struct IEcoCommandArguments1* me, /* in */ int argc, /* in */ char** argv) {
    CEcoCommandArguments1* pCMe = 0;
    int index = 0;
    int size = 0;
    char* arg = 0;

    /* Преобразование */
    pCMe = (CEcoCommandArguments1*)me;

    //if (pCMe != 0) {
    //    for (index = 0; index < argc; index++) {
    //        size = pCMe->m_pIString->RetrieveSize(pCMe->m_pIString, argv[index]);
    //        arg = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, size + 1);
    //        pCMe->m_pIMem->Copy(pCMe->m_pIMem, arg, argv[index], size);
    //        arg[size] = 0;
    //        pCMe->m_pIList->Add(pCMe->m_pIList, arg);
    //    }
    //}

    return 0;
}

/*
 *
 * <сводка>
 *   Функция get_Path
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
char* CEcoCommandArguments1_get_Path(/* in */ struct IEcoCommandArguments1* me) {
    CEcoCommandArguments1* pCMe = 0;
    char* path = 0;

    /* Преобразование */
    pCMe = (CEcoCommandArguments1*)me;

    if (pCMe != 0) {
    }

    return path;
}

/*
 *
 * <сводка>
 *   Функция get_Args
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
//IEcoList1* CEcoCommandArguments1_get_Args(/* in */ struct IEcoCommandArguments1* me) {
//    CEcoCommandArguments1* pCMe = 0;
//
//    /* Преобразование */
//    pCMe = (CEcoCommandArguments1*)me;
//
//    return pCMe->m_pIList;
//}

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
int createCEcoCommandArguments1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoCommandArguments1** ppICommandArguments) {
    int result = -1;
    IEcoSystem1* pISystem = 0;
    //IEcoMemoryManager1* pIMemMgr = 0;
    //IEcoMemoryAllocator1* pIMem = 0;
    CEcoCommandArguments1* pCMe = 0;

    /* Проверка указателей */
    if (ppICommandArguments == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISystem);

    /* Проверка */
    if (result != 0 && pISystem == 0) {
        return result;
    }

    /* Получение интерфейса управления памятью */
    //result = pISystem->QueryInterface(pISystem, &IID_IEcoMemoryManager1, (void **)&pIMemMgr);
    ///* Проверка */
    //if (result != 0 && pIMemMgr == 0) {
    //    /* Освобождение системного интерфейса в случае ошибки */
    //    pISystem->Release(pISystem);
    //    return result;
    //}

    ///* Получение интерфейса распределителя памяти */
    //pIMem = pIMemMgr->get_Allocator(pIMemMgr);
    ///* Освобождение интерфейса управления памятью */
    //pIMemMgr->Release(pIMemMgr);

    ///* Выделение памяти для данных экземпляра */
    //pCMe = (CEcoCommandArguments1*)pIMem->Alloc(pIMem, sizeof(CEcoCommandArguments1));

    ///* Сохранение указателя на интерфейс для работы с памятью */
    //pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoCommandArguments1 */
    pCMe->m_pVTblIArgs->QueryInterface = CEcoCommandArguments1_QueryInterface;
    pCMe->m_pVTblIArgs->AddRef = CEcoCommandArguments1_AddRef;
    pCMe->m_pVTblIArgs->Release = CEcoCommandArguments1_Release;
    //pCMe->m_pVTblIArgs->Init = CEcoCommandArguments1_Init;
    //pCMe->m_pVTblIArgs->get_Path = CEcoCommandArguments1_get_Path;
   // pCMe->m_VtblIArgs.get_Args = CEcoCommandArguments1_get_Args;

    /* Получение интерфейса для работы со строкой */
    ///pCMe->m_pIString = 0;
    //result = pISystem->QueryInterface(pISystem, &IID_IEcoString1, (void **)&pCMe->m_pIString);
    //if (result != 0 || pCMe->m_pIString == 0) {
    //    deleteCEcoCommandArguments1((IEcoCommandArguments1*)pCMe);
    //    return result;
    //}

    ///* Получение интерфейса для работы со списком */
    //pCMe->m_pIList = 0;
    //result = pISystem->QueryInterface(pISystem, &IID_IEcoList1, (void **)&pCMe->m_pIList);
    //if (result != 0 || pCMe->m_pIList == 0) {
    //    deleteCEcoCommandArguments1((IEcoCommandArguments1*)pCMe);
    //    return result;
    //}

    /* Возврат указателя на интерфейс */
    *ppICommandArguments = pCMe;

    /* Освобождение системного интерфейса */
    pISystem->pVTbl->Release(pISystem);

    return 0;
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
void deleteCEcoCommandArguments1(/* in */ IEcoCommandArguments1* pICommandArguments) {
    CEcoCommandArguments1* pCMe = 0;
    //IEcoMemoryAllocator1* pIMem = 0;
    //unsigned long index = 0;
    //unsigned long count = 0;
    //char* pszArg = 0;

    //if (pICommandArguments != 0 ) {
    //    /* Преобразование */
    //    pCMe = (CEcoCommandArguments1*)pICommandArguments;
    //    pIMem = pCMe->m_pIMem;
    //    /* Освобождение */
    //    if (pCMe->m_pIList != 0) {
    //        count = pCMe->m_pIList->Count(pCMe->m_pIList);
    //        for (index = 0; index < count; index++) {
    //            pszArg = (char *)pCMe->m_pIList->Item(pCMe->m_pIList, index);
    //            if (pszArg != 0) {
    //                pCMe->m_pIString->Free(pCMe->m_pIString, pszArg);
    //            }
    //        }
    //        pCMe->m_pIList->Clear(pCMe->m_pIList);
    //        pCMe->m_pIList->Release(pCMe->m_pIList);
    //        pCMe->m_pIList = 0;
    //    }
    //    if (pCMe->m_pIString != 0) {
    //        pCMe->m_pIString->Release(pCMe->m_pIString);
    //        pCMe->m_pIString = 0;
    //    }
    //    pIMem->Free(pIMem, pCMe);
    //    pIMem->Release(pIMem);
    //}
}

