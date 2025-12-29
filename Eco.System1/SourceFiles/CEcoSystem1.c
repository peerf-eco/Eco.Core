/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoSystem1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию фабрики для компонента
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoSystem1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoFileSystemManagement1.h"

#ifdef ECO_BCM283X
#include "IdEcoIPCCMailbox1.h"

extern char _end;
extern char _start;
extern char __heap_start__;
#endif

#ifdef ECO_ARM
extern uint8_t _end; /* Symbol defined in the linker script */
extern uint8_t _estack; /* Symbol defined in the linker script */
extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
#endif

//#ifdef ECO_STM32
//extern uint8_t _end; /* Symbol defined in the linker script */
//extern uint8_t _estack; /* Symbol defined in the linker script */
//extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
//#endif

/* Mac OS */
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>
#endif // __APPLE__

#ifdef __APPLE__

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#include <dlfcn.h>
/* Определение прототипов получение комонентов */
typedef IEcoComponentFactory* (*PROC_GETCOMPONENTFACTORY)(/* in */ void);

#elif TARGET_OS_MAC

#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <sys/sysctl.h>
#include <dlfcn.h>

/* Определение прототипов получение комонентов */
typedef IEcoComponentFactory* (*PROC_GETCOMPONENTFACTORY)(/* in */ void);
#endif

#endif

 /* Linux */
#ifdef LINUX
#define int8_t no_int8_t
#define wchar_t no_wchar_t
#define int64_t no_int64_t

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <err.h>
#include <unistd.h>

/* Определение прототипов получение комонентов */
typedef IEcoComponentFactory* (*PROC_GETCOMPONENTFACTORY)(/* in */ void);
#endif // LINUX

#ifdef __ANDROID__
#define int8_t no_int8_t
#define wchar_t no_wchar_t
#define int64_t no_int64_t
//#define uint64_t no_uint64_t

#include <stdlib.h>
#include <dlfcn.h>
#include <jni.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>

//#include <android/configuration.h>
//#include <android/looper.h>
//#include <android/native_activity.h>


#endif // __ANDROID__

/* Win32 */
#ifdef ECO_WINDOWS
#include "windows.h"
#include <stdlib.h>
#include <CRTDBG.H>

/* Определение прототипов получение комонентов */
typedef IEcoComponentFactory* (ECOCALLMETHOD *PROC_GETCOMPONENTFACTORY)(/* in */ void);
#endif


/* Выделяем память под один экземпляр */
CEcoSystem1_00000100 g_x00000000000000000000000000000100 = {0};


static void ECOCALLMETHOD CEcoSystem1_00000100_to_hex(unsigned char * in, unsigned int insz, char * out, unsigned int outsz) {
    unsigned char * pin = in;
    const char * hex = "0123456789ABCDEF";
    char * pout = out;
    for(; pin < in+insz; pout +=2, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        if (pout + 2 - out > outsz){
            break;
        }
    }
    pout[0] = 0;
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoSystem1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystem1_QueryInterface(/* in */ IEcoSystem1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoSystem1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystem1_AddRef(/* in */ IEcoSystem1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)me;

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoSystem1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystem1_Release(/* in */ IEcoSystem1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)me;

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция set_Data
 * </сводка>
 *
 * <описание>
 *   Функция set_Data для интерфейса IEcoSystem1
 * </описание>
 *
 */
static void ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystem1_set_Data(/* in */ IEcoSystem1Ptr_t me, /* in */ void *pv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)me;

    if (me == 0 ) {
        return; /* ERR_ECO_POINTER */
    }

    pCMe->m_data = pv;

}

/*
 *
 * <сводка>
 *   Функция get_Data
 * </сводка>
 *
 * <описание>
 *   Функция get_Data для интерфейса IEcoSystem1
 * </описание>
 *
 */
static void* ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystem1_get_Data(/* in */ IEcoSystem1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)me;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_data;
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystemInformation1_QueryInterface(/* in */ IEcoSystemInformation1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t));

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystemInformation1_AddRef(/* in */ IEcoSystemInformation1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(struct IEcoSystem1*));

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystemInformation1_Release(/* in */ IEcoSystemInformation1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(struct IEcoSystem1*));

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция get_Name
 * </сводка>
 *
 * <описание>
 *   Функция get_Name для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystemInformation1_get_Name(/* in */ IEcoSystemInformation1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t));
    IEcoMemoryAllocator1* pIMem = 0;
    int16_t result = -1;
    char_t bufferTemp[256] = {0};
    uint32_t nSize = 256;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if (pCMe->m_Name == 0) {
#ifdef __ANDROID__
    //__system_property_get("ro.product.manufacturer", manufacturer_string);
    __system_property_get("ro.product.model", bufferTemp);
#elif ECO_WINDOWS
        GetComputerNameA(bufferTemp, (LPDWORD)&nSize);
#elif __APPLE__
#if TARGET_OS_IPHONE
        gethostname(bufferTemp, nSize);
#elif TARGET_OS_MAC
        sysctlbyname("kern.hostname", bufferTemp, &nSize, NULL, 0);
#endif
#elif ECO_OS
#elif ECO_AVR8
#elif ECO_STM32
#else /* LINUX */
    nSize = 16; //INET_ADDRSTRLEN;
    gethostname(bufferTemp, nSize);
#endif
        if (nSize > 0) {
            result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
                pCMe->m_Name = (char_t*)pIMem->pVTbl->Alloc(pIMem, nSize + 1);
                pIMem->pVTbl->Copy(pIMem, pCMe->m_Name, bufferTemp, nSize);
                pCMe->m_Name[nSize] = 0;
                pIMem->pVTbl->Release(pIMem);
            }
        }
    }
    return pCMe->m_Name;

}

/*
 *
 * <сводка>
 *   Функция get_Id
 * </сводка>
 *
 * <описание>
 *   Функция get_Id для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoSystem1_00000100_IEcoSystemInformation1_get_Id(/* in */ IEcoSystemInformation1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t));
    IEcoMemoryAllocator1* pIMem = 0;
    int16_t result = -1;
#ifdef __ANDROID__
    char bufferTemp[256] = {0};
    int index = 0;
    int count = 0;
    int offset = 0;
    int nSize = 256;
#elif ECO_WINDOWS
    HW_PROFILE_INFOA HwProfInfo;
    int nSize = 0;
#elif __APPLE__
#if TARGET_OS_IPHONE
    CFUUIDRef uuidref;
    CFUUIDBytes uuidbytes;
    int nSize = 0;
#elif TARGET_OS_MAC
    uuid_t id;
    struct timespec timeout = {1, 0};
    int nSize = 0;
#endif
#elif OS_ECO
#else /* LINUX */
    char bufferTemp[256] = {0};
    int nSize = 256;
#endif

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

        if (pCMe->m_Id == 0) {
#ifdef __ANDROID__
            __system_property_get("ro.serialno", bufferTemp);
            count = strlen(bufferTemp);
            nSize = strlen("00000000-0000-0000-0000-000000000000");
            result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
                pCMe->m_Id = (char*)pIMem->pVTbl->Alloc(pIMem, nSize + 1);
                pIMem->pVTbl->Copy(pIMem, pCMe->m_Id, "00000000-0000-0000-0000-000000000000", nSize);
                for (index = 0; index < count; index++) {
                    if (index == 8 || index == 13) {
                        offset++;
                    }
                    pCMe->m_Id[index+offset] = bufferTemp[index];
                }
                pCMe->m_Id[nSize] = 0;
            }
#elif ECO_WINDOWS
       if (GetCurrentHwProfileA(&HwProfInfo)) {
            nSize = strlen(HwProfInfo.szHwProfileGuid);
            result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
                pCMe->m_Id = (char_t*)pIMem->pVTbl->Alloc(pIMem, nSize - 1);
                pIMem->pVTbl->Copy(pIMem, pCMe->m_Id, HwProfInfo.szHwProfileGuid + 1, nSize - 2);
                pCMe->m_Id[nSize - 2] = 0;
                pIMem->pVTbl->Release(pIMem);
            }
       }
#elif __APPLE__
        
#if TARGET_OS_IPHONE
        uuidref = CFUUIDCreate(NULL);
        uuidbytes = CFUUIDGetUUIDBytes(uuidref);
        nSize = 36 + 4;
        result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
        pCMe->m_Id = (char*)pIMem->pVTbl->Alloc(pIMem, nSize + 1);
        sprintf(pCMe->m_Id, "%2.2x%2.2x%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
                     uuidbytes.byte0, uuidbytes.byte1, uuidbytes.byte2, uuidbytes.byte3, uuidbytes.byte4, uuidbytes.byte5, uuidbytes.byte6, uuidbytes.byte7,
                    uuidbytes.byte8, uuidbytes.byte9, uuidbytes.byte10, uuidbytes.byte11, uuidbytes.byte12, uuidbytes.byte13, uuidbytes.byte14, uuidbytes.byte15);
        pCMe->m_Id[nSize] = 0;
                pIMem->pVTbl->Release(pIMem);
            }
#elif TARGET_OS_MAC
        if (gethostuuid(id, &timeout) == 0) {
            nSize = 36 + 4;
            result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
                pCMe->m_Id = (char_t*)pIMem->pVTbl->Alloc(pIMem, nSize - 1);
            uuid_unparse(id, pCMe->m_Id);
            pCMe->m_Id[nSize] = 0;
                pIMem->pVTbl->Release(pIMem);
            }
        }
#endif
#elif ECO_OS
#elif ECO_AVR8
#elif ECO_STM32
#else /* LINUX */
            nSize = strlen("00000000-0000-0000-0000-000000000000");
            result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
            if ( result == 0 && pIMem != 0) {
                pCMe->m_Id = (char*)pIMem->pVTbl->Alloc(pIMem, nSize + 1);
                pIMem->pVTbl->Copy(pIMem, pCMe->m_Id, "00000000-0000-0000-0000-000000000000", nSize);
                pCMe->m_Id[nSize] = 0;
                pIMem->pVTbl->Release(pIMem);
            }
#endif
    }

    return pCMe->m_Id;
}

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
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_QueryInterface(/* in */ IEcoCommandArguments1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_AddRef(/* in */ IEcoCommandArguments1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
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
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_Release(/* in */ IEcoCommandArguments1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция get_Path
 * </сводка>
 *
 * <описание>
 *   Функция get_Path для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_get_Path(/* in */ IEcoCommandArguments1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* ptrPos = 0;
    int16_t result = 0;
    int16_t size = 0;

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if (pCMe->m_Path == 0 && pCMe->m_argc > 0 && pCMe->m_argv != 0) {
        result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
        if ( result == 0 && pIMem != 0) {
            ptrPos = pCMe->m_argv[0];
            while ((char_t)ptrPos[0] != 0) {
                if ((char_t)ptrPos[0] == '\\') {
                    size = ptrPos - pCMe->m_argv[0];
                }
                ptrPos++;
            }
            pCMe->m_Path = (char_t*)pIMem->pVTbl->Alloc(pIMem, size+1);
            pIMem->pVTbl->Copy(pIMem, pCMe->m_Path, pCMe->m_argv[0], size);
            pCMe->m_Path[size] = 0;
            pIMem->pVTbl->Release(pIMem);
        }
    }

    return pCMe->m_Path;
}

/*
 *
 * <сводка>
 *   Функция get_Count
 * </сводка>
 *
 * <описание>
 *   Функция get_Count для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_get_Count(/* in */ IEcoCommandArguments1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 ) {
        return 0;  /* ERR_ECO_POINTER */
    }

    return pCMe->m_argc;
}

/*
 *
 * <сводка>
 *   Функция get_Args
 * </сводка>
 *
 * <описание>
 *   Функция get_Args для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
static char_t** ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_get_Args(/* in */ IEcoCommandArguments1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    return pCMe->m_argv;
}

/*
 *
 * <сводка>
 *   Функция get_Arg
 * </сводка>
 *
 * <описание>
 *   Функция get_Arg для интерфейса IEcoCommandArguments1
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoSystem1_00000100_IEcoCommandArguments1_get_Arg(/* in */ IEcoCommandArguments1Ptr_t me, /* in */ int16_t index) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*2 );

    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if (index >= pCMe->m_argc || pCMe->m_argv == 0) {
        return 0;
    }

    return pCMe->m_argv[index];
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_QueryInterface(/* in */ IEcoInterfaceBus1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_AddRef(/* in */ IEcoInterfaceBus1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_Release(/* in */ IEcoInterfaceBus1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
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
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_Init(/*in*/ IEcoInterfaceBus1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIBus->pVTbl->Init(pCMe->m_pIBus);
}

/*
 *
 * <сводка>
 *   Функция InitWith
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_InitWith(/*in*/ IEcoInterfaceBus1Ptr_t me, /*in*/ void* heapStartAddress, /*in*/ uint32_t size) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    return pCMe->m_pIBus->pVTbl->InitWith(pCMe->m_pIBus, heapStartAddress,  size);
}

/*
 *
 * <сводка>
 *   Функция RegisterComponent
 * </сводка>
 *
 * <описание>
 *   Функция RegisterComponent для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_RegisterComponent(/*in*/ IEcoInterfaceBus1Ptr_t me, /*in*/ const UGUID* rcid, /*in*/ struct IEcoUnknown* pIFactory) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }
    return pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, rcid, pIFactory);
}

/*
 *
 * <сводка>
 *   Функция UnRegisterComponent
 * </сводка>
 *
 * <описание>
 *   Функция UnRegisterComponent для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_UnRegisterComponent(/*in*/ IEcoInterfaceBus1Ptr_t me, /*in*/ const UGUID* rcid) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }
    return pCMe->m_pIBus->pVTbl->UnRegisterComponent(pCMe->m_pIBus, rcid);
}

/*
 *
 * <сводка>
 *   Функция QueryComponent
 * </сводка>
 *
 * <описание>
 *   Функция QueryComponent для интерфейса IEcoInterfaceBus1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1_QueryComponent(/*in*/ IEcoInterfaceBus1Ptr_t me, /*in*/ const UGUID* rcid, /*in*/ struct IEcoUnknown* pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*3 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }
    return pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, rcid, pIUnkOuter, riid, ppv);
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoError1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoError1_QueryInterface(/* in */ IEcoError1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*4);

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoError1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoError1_AddRef(/* in */ IEcoError1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*4);

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoError1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoError1_Release(/* in */ IEcoError1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*4);

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция get_Description
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoError1_get_Description(/* in */ IEcoError1Ptr_t me, /* in | out */ void *pvMessage, /* in | out */ uint16_t* iSize) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*4);

    /* Проверка указателя */
    if (me == 0 || pvMessage == 0 ) {
        return ERR_ECO_POINTER;
    }
    if ( pCMe->m_pIErr == 0) {
        pCMe->m_pIBus->pVTbl->QueryInterface(pCMe->m_pIBus, &IID_IEcoError1, &pCMe->m_pIErr);
    }
    if ( pCMe->m_pIErr != 0) {
        pCMe->m_pIErr->pVTbl->get_Description(pCMe->m_pIErr, 0,  pvMessage, iSize);
    }

    return 0;
}



#ifdef ECO_MEM_EXT

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoInterfaceBus1MemExt
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_QueryInterface(/* in */ IEcoInterfaceBus1MemExtPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoInterfaceBus1MemExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_AddRef(/* in */ IEcoInterfaceBus1MemExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1MemExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_Release(/* in */ IEcoInterfaceBus1MemExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция set_Manager
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_set_Manager(/* in */ IEcoInterfaceBus1MemExtPtr_t me, /*in*/ const UGUID* rcid) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );
    int16_t result = -1;

    /* Проверка указателя */
    if (me == 0 || rcid == 0 ) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIMemExt != 0) {
        return pCMe->m_pIMemExt->pVTbl->set_Manager(pCMe->m_pIMemExt, rcid);
    }
    else {
        result = pCMe->m_pIBus->pVTbl->QueryInterface(pCMe->m_pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pCMe->m_pIMemExt);
        if (result == 0 && pCMe->m_pIMemExt != 0) {
            return pCMe->m_pIMemExt->pVTbl->set_Manager(pCMe->m_pIMemExt, rcid);
        }
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция get_Manager
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static const UGUID* ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_get_Manager(/* in */ IEcoInterfaceBus1MemExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );

    /* Проверка указателя */
    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if ( pCMe->m_pIMemExt != 0) {
        return pCMe->m_pIMemExt->pVTbl->get_Manager(pCMe->m_pIMemExt);
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция set_ExpandPool
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_set_ExpandPool(/* in */ IEcoInterfaceBus1MemExtPtr_t me, /*in*/ bool_t bExpandPool) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*5 );

    /* Проверка указателя */
    if (me == 0) {
        return  ERR_ECO_POINTER;
    }

    if ( pCMe->m_pIMemExt != 0) {
        return pCMe->m_pIMemExt->pVTbl->set_ExpandPool(pCMe->m_pIMemExt, bExpandPool);
    }

    return  ERR_ECO_POINTER;
}


#ifdef ECO_FILE_EXT

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoInterfaceBus1FileExt
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_QueryInterface(/* in */ IEcoInterfaceBus1FileExtPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6);
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoInterfaceBus1FileExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_AddRef(/* in */ IEcoInterfaceBus1FileExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1FileExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_Release(/* in */ IEcoInterfaceBus1FileExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция set_Manager
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1FileExt
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_set_Manager(/* in */ IEcoInterfaceBus1FileExtPtr_t me, /*in*/ const UGUID* rcid) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 || rcid == 0 ) {
        return ERR_ECO_POINTER;
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->set_Manager(pCMe->m_pIFileExt, rcid);
    }

    return ERR_ECO_POINTER;
}

/*
 *
 * <сводка>
 *   Функция get_Manager
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
static const UGUID* ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_get_Manager(/* in */ IEcoInterfaceBus1FileExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->get_Manager(pCMe->m_pIFileExt);
    }

    return 0;

}

/*
 *
 * <сводка>
 *   Функция set_SearchPath
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_set_SearchPath(/* in */ IEcoInterfaceBus1FileExtPtr_t me, /*in*/ char_t* path) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->set_SearchPath(pCMe->m_pIFileExt, path);
    }

    return ERR_ECO_POINTER;
}

/*
 *
 * <сводка>
 *   Функция get_SearchPath
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
static char_t* ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_get_SearchPath(/* in */ IEcoInterfaceBus1FileExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return 0; /* ERR_ECO_POINTER */
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->get_SearchPath(pCMe->m_pIFileExt);
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция RegisterComponent
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_RegisterComponent(/* in */ IEcoInterfaceBus1FileExtPtr_t me, /*in*/ const UGUID* rcid, /*in*/ char_t* filename) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->RegisterComponent(pCMe->m_pIFileExt, rcid, filename);
    }

    return ERR_ECO_POINTER;
}

/*
 *
 * <сводка>
 *   Функция QueryComponent
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_QueryComponent(/* in */ IEcoInterfaceBus1FileExtPtr_t me, /*in*/ char_t* filename, /*in*/ const UGUID* rcid, /*in*/ IEcoUnknown* pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*6 );

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    if ( pCMe->m_pIFileExt != 0) {
        return pCMe->m_pIFileExt->pVTbl->QueryComponent(pCMe->m_pIFileExt, filename, rcid, pIUnkOuter, riid, ppv);
    }

    return ERR_ECO_POINTER;
}

#ifdef ECO_NET_EXT

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoInterfaceBus1NetExt
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_QueryInterface(/* in */ IEcoInterfaceBus1NetExtPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoInterfaceBus1NetExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_AddRef(/* in */ IEcoInterfaceBus1NetExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1NetExt
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_Release(/* in */ IEcoInterfaceBus1NetExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;  /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция set_Manager
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoInterfaceBus1NetExt
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_set_Manager(/* in */ IEcoInterfaceBus1NetExtPtr_t me, /*in*/ const UGUID* rcid) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );

    /* Проверка указателя */
    if (me == 0 || rcid == 0 ) {
        return ERR_ECO_POINTER;
    }

    if ( pCMe->m_pINetExt != 0) {
        return pCMe->m_pINetExt->pVTbl->set_Manager(pCMe->m_pINetExt, rcid);
    }

    return ERR_ECO_POINTER;
}

/*
 *
 * <сводка>
 *   Функция get_Manager
 * </сводка>
 *
 * <описание>
 *   Функция 
 * </описание>
 *
 */
static const UGUID* ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_get_Manager(/* in */ IEcoInterfaceBus1NetExtPtr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );

    /* Проверка указателя */
    if (me == 0 ) {
        return 0;  /* ERR_ECO_POINTER */
    }

    if ( pCMe->m_pINetExt != 0) {
        return pCMe->m_pINetExt->pVTbl->get_Manager(pCMe->m_pINetExt);
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция QueryComponent
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_QueryComponent(/* in */ IEcoInterfaceBus1NetExtPtr_t me, /*in*/ char_t* networkname, /*in*/ const UGUID* rcid, /*in*/ struct IEcoUnknown* pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*7 );

    /* Проверка указателя */
    if (me == 0 ) {
        return  ERR_ECO_POINTER;
    }

    if ( pCMe->m_pINetExt != 0) {
        return pCMe->m_pINetExt->pVTbl->QueryComponent(pCMe->m_pINetExt, networkname, rcid, pIUnkOuter, riid, ppv);
    }

    return ERR_ECO_POINTER;
}



















#ifdef ECO_ANDROID

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoAndroidNativeApp1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoAndroidNativeApp1_QueryInterface(/* in */ IEcoAndroidNativeApp1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*8 );

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &GID_IEcoSystem) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_pVTblISysInfo;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCommandArguments1) ) {
        *ppv = &pCMe->m_pVTblIArgs;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1) ) {
        *ppv = &pCMe->m_pVTblIBus;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoError1) ) {
        *ppv = &pCMe->m_pVTblIErr;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_MEM_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1MemExt) ) {
        *ppv = &pCMe->m_pVTblIMemExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_FILE_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1FileExt) ) {
        *ppv = &pCMe->m_pVTblIFileExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_NET_EXT
    else if ( IsEqualUGUID(riid, &IID_IEcoInterfaceBus1NetExt) ) {
        *ppv = &pCMe->m_pVTblINetExt;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#ifdef ECO_ANDROID
    else if ( IsEqualUGUID(riid, &IID_IEcoAndroidNativeApp1) ) {
        *ppv = &pCMe->m_pVTblIApp;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
    }
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblISys;
        pCMe->m_pVTblISys->AddRef((IEcoSystem1*)pCMe);
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
 *   Функция AddRef для интерфейса IEcoAndroidNativeApp1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoAndroidNativeApp1_AddRef(/* in */ IEcoAndroidNativeApp1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*8 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoAndroidNativeApp1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoAndroidNativeApp1_Release(/* in */ IEcoAndroidNativeApp1Ptr_t me) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*8 );

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;  /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    atomicdecrement_int32_t(&pCMe->m_cRef);

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystem1_00000100((IEcoSystem1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция get_Activity
 * </сводка>
 *
 * <описание>
 *   Функция get_Activity для интерфейса IEcoAndroidNativeApp1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoAndroidNativeApp1_get_Activity(/* in */ IEcoAndroidNativeApp1Ptr_t me, /* out */ ANativeActivity** activity, /* out */ void** savedState, /* out */ size_t* savedStateSize) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*8 );

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER; /* ERR_ECO_POINTER */
    }

    if ( activity != 0) {
        *activity = pCMe->m_pActivity;
    }

    if ( savedState != 0) {
        *savedState = pCMe->m_pSavedState;
        *savedStateSize = pCMe->m_iSavedStateSize;
    }

    return ERR_ECO_SUCCESES;

}

/*
 *
 * <сводка>
 *   Функция get_Window
 * </сводка>
 *
 * <описание>
 *   Функция get_Window для интерфейса IEcoAndroidNativeApp1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoSystem1_00000100_IEcoAndroidNativeApp1_get_Window(/* in */ IEcoAndroidNativeApp1Ptr_t me, /* out */ ANativeWindow** window) {
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)((uint64_t)me - sizeof(IEcoUnknownPtr_t)*8 );

    /* Проверка указателя */
    if (me == 0) {
        return ERR_ECO_POINTER; /* ERR_ECO_POINTER */
    }

    if ( window != 0) {
        *window = pCMe->m_pWindow;
    }

    return ERR_ECO_SUCCESES;

}

#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */

/* Create Virtual Table */
IEcoSystem1VTbl g_x00000000000000000000000000000100VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoSystem1_QueryInterface,
    CEcoSystem1_00000100_IEcoSystem1_AddRef,
    CEcoSystem1_00000100_IEcoSystem1_Release,
    CEcoSystem1_00000100_IEcoSystem1_set_Data,
    CEcoSystem1_00000100_IEcoSystem1_get_Data
};

IEcoSystemInformation1VTbl g_x000000000000000000000000000001FFVTbl_00000100 = {
    CEcoSystem1_00000100_IEcoSystemInformation1_QueryInterface,
    CEcoSystem1_00000100_IEcoSystemInformation1_AddRef,
    CEcoSystem1_00000100_IEcoSystemInformation1_Release,
    CEcoSystem1_00000100_IEcoSystemInformation1_get_Name,
    CEcoSystem1_00000100_IEcoSystemInformation1_get_Id
};

IEcoCommandArguments1VTbl g_x00000000000000000000000000000110VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoCommandArguments1_QueryInterface,
    CEcoSystem1_00000100_IEcoCommandArguments1_AddRef,
    CEcoSystem1_00000100_IEcoCommandArguments1_Release,
    CEcoSystem1_00000100_IEcoCommandArguments1_get_Count,
    CEcoSystem1_00000100_IEcoCommandArguments1_get_Args,
    CEcoSystem1_00000100_IEcoCommandArguments1_get_Path,
    CEcoSystem1_00000100_IEcoCommandArguments1_get_Arg
};

IEcoInterfaceBus1VTbl g_x00000000000000000000A00000000101VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoInterfaceBus1_QueryInterface,
    CEcoSystem1_00000100_IEcoInterfaceBus1_AddRef,
    CEcoSystem1_00000100_IEcoInterfaceBus1_Release,
    CEcoSystem1_00000100_IEcoInterfaceBus1_Init,
    CEcoSystem1_00000100_IEcoInterfaceBus1_InitWith,
    CEcoSystem1_00000100_IEcoInterfaceBus1_RegisterComponent,
    CEcoSystem1_00000100_IEcoInterfaceBus1_UnRegisterComponent,
    CEcoSystem1_00000100_IEcoInterfaceBus1_QueryComponent
};

IEcoError1VTbl g_x0000000000000000000000000000FFFEVTbl_00000100 = {
    CEcoSystem1_00000100_IEcoError1_QueryInterface,
    CEcoSystem1_00000100_IEcoError1_AddRef,
    CEcoSystem1_00000100_IEcoError1_Release,
    CEcoSystem1_00000100_IEcoError1_get_Description
};

#ifdef ECO_MEM_EXT
IEcoInterfaceBus1MemExtVTbl g_x00000000000000000000A00100000101VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_QueryInterface,
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_AddRef,
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_Release,
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_set_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_get_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1MemExt_set_ExpandPool
};
#ifdef ECO_FILE_EXT
IEcoInterfaceBus1FileExtVTbl g_x00000000000000000000A00200000101VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_QueryInterface,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_AddRef,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_Release,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_set_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_get_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_set_SearchPath,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_get_SearchPath,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_RegisterComponent,
    CEcoSystem1_00000100_IEcoInterfaceBus1FileExt_QueryComponent
};
#ifdef ECO_NET_EXT
IEcoInterfaceBus1NetExtVTbl g_x00000000000000000000A00300000101VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_QueryInterface,
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_AddRef,
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_Release,
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_set_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_get_Manager,
    CEcoSystem1_00000100_IEcoInterfaceBus1NetExt_QueryComponent
};
#ifdef ECO_ANDROID
IEcoAndroidNativeApp1VTbl g_x41D50AF92EED435599E0C40BC3AFBF43VTbl_00000100 = {
    CEcoSystem1_00000100_IEcoAndroidNativeApp1_QueryInterface,
    CEcoSystem1_00000100_IEcoAndroidNativeApp1_AddRef,
    CEcoSystem1_00000100_IEcoAndroidNativeApp1_Release,
    CEcoSystem1_00000100_IEcoAndroidNativeApp1_get_Activity,
    CEcoSystem1_00000100_IEcoAndroidNativeApp1_get_Window
};
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */

/*
 *
 * <сводка>
 *   Функция LoadComponent
 * </сводка>
 *
 * <описание>
 *   Функция LoadComponent
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoSystem1_00000100_LoadComponent(/*in*/ const UGUID* rcid, /*out*/ voidptr_t* ppv) {
    int16_t result = -1;
    char_t *eco_fw_rt = 0;
    const UGUID* rcGID = &GID_IEcoSystem;
#ifdef ECO_WINDOWS
    HMODULE hHandleModule;
    char_t buf[33] = {0};
    PROC_GETCOMPONENTFACTORY DynamicLibraryGetComponentFactory = 0;
    uint32_t size = 0;
    char_t szPathModuleName[1024] = {0};
    char_t szModiuleName[1024] = {0};
    char_t szEntryPointName[256] = {0};
    IEcoUnknown* pILoadFactory = 0;
#elif ECO_LINUX
    void* hHandleModule = 0;
    char_t* dir = 0;
    char_t buf[33] = {0};
    PROC_GETCOMPONENTFACTORY DynamicLibraryGetComponentFactory = 0;
    uint32_t size = 0;
    char_t szPathModuleName[1024] = {0};
    char_t szModiuleName[1024] = {0};
//    char_t szEntryPointName[256] = {0};
//    IEcoSystem1* pISys = 0;
    IEcoUnknown* pILoadFactory = 0;
#elif __APPLE__

//#ifdef TARGET_OS_MAC
    void* hHandleModule = 0;
    char_t* dir = 0;
    char_t buf[36] = {0};

    PROC_GETCOMPONENTFACTORY DynamicLibraryGetComponentFactory = 0;
    uint32_t size = 0;
    char_t szPathModuleName[1024] = {0};
    char_t szModiuleName[1024] = {0};
    char_t szEntryPointName[256] = {0};
    IEcoSystem1* pISys = 0;
    IEcoUnknown* pILoadFactory = 0;
//#endif
    
#endif

#ifdef ECO_WINDOWS
    /* Проверка */
    if (ppv != 0) {
        CEcoSystem1_00000100_to_hex((unsigned char*)rcid->Data, 16, buf, 16*2);
        size = strlen(buf);
        memcpy(szModiuleName, buf, size);
        memcpy(szModiuleName + size, ".dll", 4);
        hHandleModule = GetModuleHandleA(szModiuleName);
        if (hHandleModule == NULL) {
            hHandleModule = LoadLibraryA(szModiuleName);
        }
        if (hHandleModule == NULL) {
            _dupenv_s(&eco_fw_rt, &size, "ECO_FRAMEWORK_RT");
            memcpy(szPathModuleName, eco_fw_rt, size);
            if (szPathModuleName[size-1] != '\\' && size != 0) {
                szPathModuleName[size-1] = '\\';
            }
            CEcoSystem1_00000100_to_hex((unsigned char*)rcGID->Data, 16, buf, 16*2);
            memcpy(szPathModuleName + size, buf, strlen(szModiuleName));
            size += strlen(buf);
            szPathModuleName[size] = '\\';
            size++;

            memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
            hHandleModule = LoadLibraryA(szPathModuleName);
        }
        if (!hHandleModule) {
            return ERR_ECO_COMPONENT_NOTFOUND;
        }
#if ECO_X86_32
        DynamicLibraryGetComponentFactory = (PROC_GETCOMPONENTFACTORY)GetProcAddress(hHandleModule, "_GetIEcoComponentFactoryPtr@0");
#else
        DynamicLibraryGetComponentFactory = (PROC_GETCOMPONENTFACTORY)GetProcAddress(hHandleModule, "GetIEcoComponentFactoryPtr");
#endif
        if ( DynamicLibraryGetComponentFactory != 0 ) {
            pILoadFactory = (IEcoUnknown*)DynamicLibraryGetComponentFactory();
        }
        if (pILoadFactory != 0) {
            *ppv = pILoadFactory;
            result = 0;
        }

    }
#elif ECO_LINUX
    /* Проверка */
    if (ppv != 0) {
        sprintf(buf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", rcid->Data[0], rcid->Data[1], rcid->Data[2], rcid->Data[3], rcid->Data[4], rcid->Data[5], rcid->Data[6], rcid->Data[7], rcid->Data[8], rcid->Data[9], rcid->Data[10], rcid->Data[11], rcid->Data[12], rcid->Data[13], rcid->Data[14], rcid->Data[15]);
        size = strlen(buf);
        memcpy(szModiuleName, buf, size);
        memcpy(szModiuleName + size, ".so", 3);

        dir = getcwd(NULL, 0);
        if (dir != NULL) {
            size = strlen(dir);
            memcpy(szPathModuleName, dir, size);
            free(dir);
            if (szPathModuleName[size] != '/' && size != 0) {
                szPathModuleName[size] = '/';
                size++;
            }
            memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
            hHandleModule = dlopen(szPathModuleName, RTLD_LAZY|RTLD_GLOBAL);
        }
        if (!hHandleModule) {
            memset(szPathModuleName, 0, 1024);
            eco_fw_rt = getenv( "ECO_FRAMEWORK_RT" );
            if (eco_fw_rt != 0) {
                size = strlen(eco_fw_rt);
                memcpy(szPathModuleName, eco_fw_rt, size);
                if (szPathModuleName[size] != '/' && size != 0) {
                    szPathModuleName[size] = '/';
                    size++;
                }
                CEcoSystem1_00000100_to_hex((unsigned char*)rcGID->Data, 16, buf, 16*2);
                memcpy(szPathModuleName + size, buf, strlen(szModiuleName));
                size += strlen(buf);
                szPathModuleName[size] = '/';
                size++;
                memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
                hHandleModule = dlopen(szPathModuleName, RTLD_LAZY|RTLD_GLOBAL);
            }
        }
        if (!hHandleModule) {
            hHandleModule = dlopen(szModiuleName, RTLD_LAZY|RTLD_GLOBAL);
        }
        if (!hHandleModule) {
            //printf("%s\n", dlerror());
            return ERR_ECO_COMPONENT_NOTFOUND;
        }

        DynamicLibraryGetComponentFactory = (PROC_GETCOMPONENTFACTORY)dlsym(hHandleModule, "GetIEcoComponentFactoryPtr");
        if ( DynamicLibraryGetComponentFactory != 0 ) {
            pILoadFactory = (IEcoUnknown*)DynamicLibraryGetComponentFactory();
        }
        if (pILoadFactory != 0) {
            *ppv = pILoadFactory;
            result = 0;
        }
    }
#elif __APPLE__

#ifdef TARGET_OS_MAC
    /* Проверка */
    if (ppv != 0) {
        sprintf(buf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", rcid->Data[0], rcid->Data[1], rcid->Data[2], rcid->Data[3], rcid->Data[4], rcid->Data[5], rcid->Data[6], rcid->Data[7], rcid->Data[8], rcid->Data[9], rcid->Data[10], rcid->Data[11], rcid->Data[12], rcid->Data[13], rcid->Data[14], rcid->Data[15]);

        size = strlen(buf);
        memcpy(szModiuleName, buf, size);
        memcpy(szModiuleName + size, ".dylib", 6);

        dir = getcwd(NULL, 0);
        if (dir != NULL) {
            size = strlen(dir);
            memcpy(szPathModuleName, dir, size);
            free(dir);
            if (szPathModuleName[size] != '/' && size != 0) {
                szPathModuleName[size] = '/';
                size++;
            }
            memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
            hHandleModule = dlopen(szPathModuleName, RTLD_LAZY|RTLD_GLOBAL);
        }
        if (!hHandleModule) {
            memset(szPathModuleName, 0, 1024);
            eco_fw_rt = getenv( "ECO_FRAMEWORK_RT" );
            if (eco_fw_rt != 0) {
                size = strlen(eco_fw_rt);
                memcpy(szPathModuleName, eco_fw_rt, size);
                if (szPathModuleName[size] != '/' && size != 0) {
                    szPathModuleName[size] = '/';
                    size++;
                }
                CEcoSystem1_00000100_to_hex((unsigned char*)rcGID->Data, 16, buf, 16*2);
                memcpy(szPathModuleName + size, buf, strlen(szModiuleName));
                size += strlen(buf);
                szPathModuleName[size] = '/';
                size++;
                memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
                hHandleModule = dlopen(szPathModuleName, RTLD_LAZY|RTLD_GLOBAL);
            }
        }
        if (!hHandleModule) {
            memset(szPathModuleName, 0, 1024);
            eco_fw_rt = getenv( "ECO_FRAMEWORK_RT" );
            if (eco_fw_rt != 0) {
                size = strlen(eco_fw_rt);
                memcpy(szPathModuleName, eco_fw_rt, size);
                if (szPathModuleName[size] != '/' && size != 0) {
                    szPathModuleName[size] = '/';
                    size++;
                }
                memcpy(szPathModuleName + size, szModiuleName, strlen(szModiuleName));
                hHandleModule = dlopen(szPathModuleName, RTLD_LAZY|RTLD_GLOBAL);
            }
        }
        if (!hHandleModule) {
            hHandleModule = dlopen(szModiuleName, RTLD_LAZY|RTLD_GLOBAL);
        }
        if (!hHandleModule) {
            return ERR_ECO_COMPONENT_NOTFOUND;
        }

        DynamicLibraryGetComponentFactory = (PROC_GETCOMPONENTFACTORY)dlsym(hHandleModule, "GetIEcoComponentFactoryPtr");
        if ( DynamicLibraryGetComponentFactory != 0 ) {
            pILoadFactory = (IEcoUnknown*)DynamicLibraryGetComponentFactory();
        }
        if (pILoadFactory != 0) {
            *ppv = pILoadFactory;
            result = 0;
        }
    }
#endif
    
#endif

    return result;
}

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
int16_t ECOCALLMETHOD createCEcoSystem1_00000100(/* in */ int16_t argc, /* in */ char_t **argv, /* out */ IEcoSystem1** ppIEcoSys) {
    CEcoSystem1_00000100* pCMe = 0;
    int16_t result = -1;
    UGUID* rcidBus = (UGUID*)&CID_EcoInterfaceBus1;
    UGUID* rcidMem = (UGUID*)&CID_EcoMemoryManager1;
    UGUID* rcidFile = (UGUID*)&CID_EcoFileSystemManagement1;
    IEcoUnknown* pILoadFactory = 0;
    IEcoComponentFactory* pIFactory = 0;

#ifdef ECO_WINDOWS
    HMODULE hHandleModule;
    char_t buf[33] = {0};
    PROC_GETCOMPONENTFACTORY DynamicLibraryGetComponentFactory = 0;
    uint32_t size = 0;
    char_t szPathModuleName[1024] = {0};
    char_t szModiuleName[1024] = {0};
#endif
#ifdef ECO_AVR8
#endif
#ifdef ECO_ARM
const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
#endif
//#ifdef ECO_STM32
//const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
//#else
    //IEcoBIOSExtensions1VideoServices* pIVideoServices = 0;
//#endif
    //     IEcoIPCCMailbox1* pIMailbox = 0;
    /* Получение указателя на структуру системного объекта расположенноого в области инициализированных данных */
    pCMe = &g_x00000000000000000000000000000100;

    /* Проверка счетчика ссылок на компонент */
    if (pCMe->m_cRef == 0 && ppIEcoSys != 0) {
        /* Установка счетчика ссылок на компонент */
        pCMe->m_cRef = 1;

        /* Создание таблицы функций интерфейса IEcoSystem1 */
        pCMe->m_pVTblISys = &g_x00000000000000000000000000000100VTbl_00000100;
        /* Создание таблицы функций интерфейса IEcoSystemInformation1 */
        pCMe->m_pVTblISysInfo = &g_x000000000000000000000000000001FFVTbl_00000100;
        /* Создание таблицы функций интерфейса IEcoCommandArguments1 */
        pCMe->m_pVTblIArgs = &g_x00000000000000000000000000000110VTbl_00000100;
        /* Создание таблицы функций интерфейса IEcoInterfaceBus1VTbl */
        pCMe->m_pVTblIBus = &g_x00000000000000000000A00000000101VTbl_00000100;
        /* Создание таблицы функций интерфейса IEcoError1VTbl */
        pCMe->m_pVTblIErr = &g_x0000000000000000000000000000FFFEVTbl_00000100;
#ifdef ECO_MEM_EXT
        /* Создание таблицы функций интерфейса IEcoInterfaceBus1MemExt */
        pCMe->m_pVTblIMemExt = &g_x00000000000000000000A00100000101VTbl_00000100;
#ifdef ECO_FILE_EXT
        /* Создание таблицы функций интерфейса IEcoInterfaceBus1FileExt */
        pCMe->m_pVTblIFileExt = &g_x00000000000000000000A00200000101VTbl_00000100;
#ifdef ECO_NET_EXT
        /* Создание таблицы функций интерфейса IEcoInterfaceBus1NetExt */
        pCMe->m_pVTblINetExt = &g_x00000000000000000000A00300000101VTbl_00000100;
#ifdef ECO_ANDROID
        /* Создание таблицы функций интерфейса IEcoAndroidNativeApp1 */
        pCMe->m_pVTblIApp = &g_x41D50AF92EED435599E0C40BC3AFBF43VTbl_00000100;
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */

        /* Создание экземпляра интерфейсной шины */

        /* Статическое - использовалось на первых этапах */
#ifdef ECO_LIB
        result = GetIEcoComponentFactoryPtr_00000000000000000000000042757331->pVTbl->Alloc(GetIEcoComponentFactoryPtr_00000000000000000000000042757331, pCMe, 0, &IID_IEcoInterfaceBus1, (void **)&pCMe->m_pIBus);
#else
        /* Динамическое */
        result = CEcoSystem1_00000100_LoadComponent(rcidBus, &pILoadFactory);
        /* Проверка */
        if (result != 0 || pILoadFactory == 0) {
            /* Возврат в случае ошибки */
            return ERR_ECO_NOBUS;
        }

        result = pILoadFactory->pVTbl->QueryInterface(pILoadFactory, &IID_IEcoComponentFactory, (voidptr_t*)&pIFactory);

        /* Проверка */
        if (result != 0 || pIFactory == 0) {
            /* Возврат в случае ошибки */
            return ERR_ECO_NOBUS;
        }

        result = pIFactory->pVTbl->Alloc(pIFactory, 0, 0, &IID_IEcoInterfaceBus1, (void **)&pCMe->m_pIBus);
#endif
        /* Проверка */
        if (result != 0 || pCMe->m_pIBus == 0) {
            /* Возврат в случае ошибки */
            return result;
        }

#ifdef ECO_AVR8
#else

        /* Регистрация статического компонента для работы с менеджером памяти */

#ifdef ECO_LIB
        /* Статическое - использовалось на первых этапах */
        result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoMemoryManager1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_0000000000000000000000004D656D31);
#else
        /* Динамическое */
        pILoadFactory = 0;
        result = CEcoSystem1_00000100_LoadComponent(rcidMem, &pILoadFactory);
        /* Проверка */
        if (result != 0 || pILoadFactory == 0) {
            /* Возврат в случае ошибки */
            return ERR_ECO_MEMMGR_NOTREG;
        }
        result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoMemoryManager1, (IEcoUnknown*)pILoadFactory);
#endif
        /* Проверка */
        if (result != 0) {
            /* Возврат в случае ошибки */
            return result;
        }

        /* Запрос расширения интерфейсной шины */
        pCMe->m_pIMemExt = 0;
        result = pCMe->m_pIBus->pVTbl->QueryInterface(pCMe->m_pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pCMe->m_pIMemExt);
        if (result == 0 && pCMe->m_pIMemExt != 0) {
            /* Установка расширения менаджера памяти */
            pCMe->m_pIMemExt->pVTbl->set_Manager(pCMe->m_pIMemExt, &CID_EcoMemoryManager1);
            /* Установка разрешения расширения пула */
            pCMe->m_pIMemExt->pVTbl->set_ExpandPool(pCMe->m_pIMemExt, 1);
        }

        /* Получение интерфейса управления памятью */
        pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryManager1, (void**) &pCMe->m_pIMemMgr);
        if (result != 0 || pCMe->m_pIMemMgr == 0) {
            /* Возврат в случае ошибки */
            return result;
        }

#ifdef ECO_BCM283X

        pCMe->m_pIMemMgr->pVTbl->Init(pCMe->m_pIMemMgr, &__heap_start__, 0x080000);

    result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoIPCCMailbox1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_F10BC39A4F2143CF8A1E104650A2C302);
        /* Проверка */
        if (result != 0) {
            /* Возврат в случае ошибки */
         //   return result;
        }
         //   result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoIPCCMailbox1, 0, &IID_IEcoIPCCMailbox1, (void**) &pIMailbox);
#endif
#ifdef ECO_ARM

        pCMe->m_pIMemMgr->pVTbl->Init(pCMe->m_pIMemMgr, &_end, stack_limit);

//#endif
//        #elif ECO_STM32

        pCMe->m_pIMemMgr->pVTbl->Init(pCMe->m_pIMemMgr, &_end, stack_limit);

#else
        /* Выделение области памяти */
        pCMe->m_pIMemMgr->pVTbl->Init(pCMe->m_pIMemMgr, 0, 0x60000000);
#endif
        /* Регистрация статического компонента для работы с базовой системой ввода вывода */
        //result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoBIOSExtensions1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_00000000000000000000000042494F53);
        /* Проверка */
        //if (result != 0) {
            /* Возврат в случае ошибки */
        //    return result;
        //}
        //result = pCMe->m_pIBus->pVTbl->QueryComponent(pCMe->m_pIBus, &CID_EcoBIOSExtensions1, 0, &IID_IEcoBIOSExtensions1VideoServices, (void**) &pIVideoServices);
        /* Проверка */
        //if (result != 0 || pIVideoServices == 0) {
        //    /* Возврат в случае ошибки */
        //    return result;
        //}

       // pIVideoServices->pVTbl->WriteString(pIVideoServices, 0, 0, 0, 1, CHARACTER_ATTRIBUTE_FORE_COLOR_YELLOW, "\xdd\xea\xee\x20\xce\xd1\x21\x21\x21", 9);

#ifdef ECO_FILE_EXT
        /* Регистрация статического компонента для работы с менеджером файловой системы */
#ifdef ECO_LIB
        result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoFileSystemManagement1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_00000000000000000000000046534D31);
#else
        /* Динамическое */
        pILoadFactory = 0;
        result = CEcoSystem1_00000100_LoadComponent(rcidFile, &pILoadFactory);
        /* Проверка */
        if (result != 0 || pILoadFactory == 0) {
            /* Возврат в случае ошибки */
            return ERR_ECO_FILEMGR_NOTREG;
        }

        result = pCMe->m_pIBus->pVTbl->RegisterComponent(pCMe->m_pIBus, &CID_EcoFileSystemManagement1, (IEcoUnknown*)pILoadFactory);
#endif
        /* Проверка */
        if (result != 0) {
            /* Возврат в случае ошибки */
            return ERR_ECO_FILEMGR_NOTREG;
        }

        /* Запрос расширения интерфейсной шины */
        pCMe->m_pIFileExt = 0;
        result = pCMe->m_pIBus->pVTbl->QueryInterface(pCMe->m_pIBus, &IID_IEcoInterfaceBus1FileExt, (void**)&pCMe->m_pIFileExt);
        if (result != 0 && pCMe->m_pIFileExt != 0) {
            /* Установка расширения менаджера памяти */
            pCMe->m_pIFileExt->pVTbl->set_Manager(pCMe->m_pIFileExt, &CID_EcoFileSystemManagement1);
        }
        pCMe->m_pINetExt = 0;
#endif
#endif
        /* Сохранение указателей на аргументы */
        pCMe->m_argc = argc;
        pCMe->m_argv = argv;

        result = 0;
#ifdef ECO_LIB
        result = GetIEcoComponentFactoryPtr_00000000000000000000000042757331->pVTbl->Init(GetIEcoComponentFactoryPtr_00000000000000000000000042757331, (IEcoUnknown*)pCMe, pCMe->m_pIBus);
#else
        result = pIFactory->pVTbl->Init(pIFactory, (IEcoUnknown*)pCMe, pCMe->m_pIBus);
#endif
    }

    /* Возврат указателя на интерфейс */
    *ppIEcoSys = (IEcoSystem1*)pCMe;

    return result;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoSystem1_00000100(/* in */ IEcoSystem1* pIEcoSys) {
    CEcoSystem1_00000100* pCMe = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    int16_t result = 0;

    if (pIEcoSys != 0 ) {
        /* Преобразование */
        pCMe = (CEcoSystem1_00000100*)pIEcoSys;
        result = pCMe->m_pIMemMgr->pVTbl->QueryInterface(pCMe->m_pIMemMgr, &IID_IEcoMemoryAllocator1, (void**)&pIMem);
        if (result == 0 && pIMem != 0) {
            if (pCMe->m_Id != 0) {
                pIMem->pVTbl->Free(pIMem, pCMe->m_Id);
            }
            if (pCMe->m_Name != 0) {
                pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
            }
            if (pCMe->m_Path != 0) {
                pIMem->pVTbl->Free(pIMem, pCMe->m_Path);
            }
            pIMem->pVTbl->Release(pIMem);
        }

        pCMe->m_pIMemMgr->pVTbl->Release(pCMe->m_pIMemMgr);
        pCMe->m_pIBus->pVTbl->Release(pCMe->m_pIBus);
#ifdef ECO_MEM_EXT
        if (pCMe->m_pIMemExt != 0) {
            pCMe->m_pIMemExt->pVTbl->Release(pCMe->m_pIMemExt);
        }
#ifdef ECO_FILE_EXT
        if (pCMe->m_pIFileExt != 0) {
            pCMe->m_pIFileExt->pVTbl->Release(pCMe->m_pIFileExt);
        }
#ifdef ECO_NET_EXT
        if (pCMe->m_pINetExt != 0) {
            pCMe->m_pINetExt->pVTbl->Release(pCMe->m_pINetExt);
        }
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */
        pIEcoSys = 0;
    }
}
