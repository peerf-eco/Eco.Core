/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoUnitTestMain
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

/* Eco OS */
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoSystemInformation1.h"
#include "IEcoCommandArguments1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoList1.h"

#ifdef ECO_ANDROID
#include "EcoDemoDialog.h"
#endif

/* Глобальный указатель на системный интерфейс */
IEcoSystem1* g_pISys = 0;

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoSystemInformation1* pISysInfo = 0;
    IEcoCommandArguments1* pICmdArg = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    IEcoFileManager1* pIFileMgr = 0;
    IEcoFile1* pIFile = 0;
    IEcoList1* pIList = 0;
    char_t* pszBuffer = 0;
    uint32_t iSize = 0;
    uint32_t iIndex = 0;

    /* Проверка и создание системного интрефейса */
    if (g_pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&g_pISys);
        if (result != 0 || g_pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }


    /* Получение интерфейса для работы с интерфейсной шиной */
    result = g_pISys->pVTbl->QueryInterface(g_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Выделение блока памяти */
    pszBuffer = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);

    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, pszBuffer, 'a', 9);

    printf("buffer = %s\n", pszBuffer);

    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, pszBuffer);

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif


    result = g_pISys->pVTbl->QueryInterface(g_pISys, &IID_IEcoSystemInformation1, (void **)&pISysInfo);
    if (result != 0 || pISysInfo == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    result = pISysInfo->pVTbl->QueryInterface(pISysInfo, &IID_IEcoCommandArguments1, (void **)&pICmdArg);
    if (result != 0 || pICmdArg == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    pISysInfo->pVTbl->Release(pISysInfo);
    pICmdArg->pVTbl->Release(pICmdArg);

    /* Получение интерфейса для работы со списком */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pIList);
    /* Проверка */
    if (result != 0 || pIList == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

#ifdef ECO_ANDROID
    DialogProcessing();
#endif

Release:

    /* Освобождение интерфейса управления файлами */
    if (pIFileMgr != 0) {
        pIFileMgr->pVTbl->Release(pIFileMgr);
    }

    /* Освобождение интерфейса для работы со списком */
    if (pIList != 0) {
        pIList->pVTbl->Release(pIList);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение системного интерфейса */
    if (g_pISys != 0) {
        g_pISys->pVTbl->Release(g_pISys);
    }

    return result;
}
