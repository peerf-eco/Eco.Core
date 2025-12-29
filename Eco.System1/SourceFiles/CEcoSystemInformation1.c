/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoSystemInformation1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoSystemInformation1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "CEcoSystemInformation1.h"

#ifdef __ANDROID__

#include <sys/system_properties.h>
#include <android/log.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <linux/if.h>
#include <dlfcn.h>
#define HAVE_GETADDRINFO 1

#if (__ANDROID_API__ >= 21)

typedef int (*PFN_SYSTEM_PROP_GET)(const char *, char *);
int __system_property_get(const char* name, char* value)
{
    static PFN_SYSTEM_PROP_GET __real_system_property_get = NULL;
    if (!__real_system_property_get) {
        void *handle = dlopen("libc.so", RTLD_NOLOAD);
        if (!handle) {
            __android_log_print(ANDROID_LOG_ERROR, "system", "Cannot dlopen libc.so: %s.\n", dlerror());
        } else {
            __real_system_property_get = (PFN_SYSTEM_PROP_GET)dlsym(handle, "__system_property_get");
        }
        if (!__real_system_property_get) {
            __android_log_print(ANDROID_LOG_ERROR, "system", "Cannot resolve __system_property_get(): %s.\n", dlerror());
        }
    }
    return (*__real_system_property_get)(name, value);
} 
#endif // __ANDROID_API__ >= 21

#elif __APPLE__

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <pthread.h>
//#include <CFNetwork/CFSocketStream.h>
#include <CoreFoundation/CoreFoundation.h>

#elif TARGET_OS_MAC

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <pthread.h>
#include <unistd.h>
#include <uuid/uuid.h>

#endif

#elif ECO_WINDOWS

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#elif OS_ECO
#else // LINUX

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <ifaddrs.h>
#include <pthread.h>
//#include <linux/if.h>
#include <dlfcn.h>
#endif

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
int CEcoSystemInformation1_QueryInterface(/* in */ struct IEcoSystemInformation1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoSystemInformation1* pCMe = 0;

    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoSystemInformation1*)me;

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoSystemInformation1) ) {
        *ppv = &pCMe->m_VtblIInfo;
        pCMe->m_VtblIInfo.AddRef(&pCMe->m_VtblIInfo);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_VtblIInfo;
        pCMe->m_VtblIInfo.AddRef(&pCMe->m_VtblIInfo);
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
 *   Функция AddRef для интерфейса IEcoSystemInformation1
 * </описание>
 *
 */
unsigned long CEcoSystemInformation1_AddRef(/* in */ struct IEcoSystemInformation1* me) {
    CEcoSystemInformation1* pCMe = 0;

    if (me == 0 ) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoSystemInformation1*)me;

    return ++pCMe->m_cRef;
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
unsigned long CEcoSystemInformation1_Release(/* in */ struct IEcoSystemInformation1* me) {
    CEcoSystemInformation1* pCMe = 0;

    if (me == 0 ) {
        return -1;
    }

    /* Преобразование */
    pCMe = (CEcoSystemInformation1*)me;

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoSystemInformation1(&pCMe->m_VtblIInfo);
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
 *   Функция
 * </описание>
 *
 */
char* CEcoSystemInformation1_get_Name(/* in */ struct IEcoSystemInformation1* me) {
    CEcoSystemInformation1* pCMe = 0;
    char bufferTemp[256] = {0};
    unsigned long nSize = 256;

    if (me == 0 ) {
        return 0;
    }

    /* Преобразование */
    pCMe = (CEcoSystemInformation1*)me;
    
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
#elif OS_ECO
#else /* LINUX */
    nSize = INET_ADDRSTRLEN;
    gethostname(bufferTemp, nSize);
#endif
        if (nSize > 0) {
            //pCMe->m_Name = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize + 1);
            //pCMe->m_pIMem->Copy(pCMe->m_pIMem, pCMe->m_Name, bufferTemp, nSize);
            pCMe->m_Name[nSize] = 0;
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
 *   Функция
 * </описание>
 *
 */
char* CEcoSystemInformation1_get_Id(/* in */ struct IEcoSystemInformation1* me) {
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
    CEcoSystemInformation1* pCMe = 0;

    if (me == 0 ) {
        return 0;
    }

    /* Преобразование */
    pCMe = (CEcoSystemInformation1*)me;

    if (pCMe->m_Id == 0) {
#ifdef __ANDROID__
            __system_property_get("ro.serialno", bufferTemp);
            count = strlen(bufferTemp);
            nSize = strlen("00000000-0000-0000-0000-000000000000");
            pCMe->m_Id = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize + 1);
            pCMe->m_pIMem->Copy(pCMe->m_pIMem, pCMe->m_Id, "00000000-0000-0000-0000-000000000000", nSize);
            for (index = 0; index < count; index++) {
                if (index == 8 || index == 13) {
                    offset++;
                }
                pCMe->m_Id[index+offset] = bufferTemp[index];
            }
            pCMe->m_Id[nSize] = 0;
#elif ECO_WINDOWS
       if (GetCurrentHwProfileA(&HwProfInfo)) {
            nSize = strlen(HwProfInfo.szHwProfileGuid);
            //pCMe->m_Id = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize - 1);
            //pCMe->m_pIMem->Copy(pCMe->m_pIMem, pCMe->m_Id, HwProfInfo.szHwProfileGuid + 1, nSize - 2);
            pCMe->m_Id[nSize - 2] = 0;
       }
#elif __APPLE__
        
#if TARGET_OS_IPHONE
        uuidref = CFUUIDCreate(NULL);
        uuidbytes = CFUUIDGetUUIDBytes(uuidref);
        nSize = 36 + 4;
        pCMe->m_Id = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize + 1);
        sprintf(pCMe->m_Id, "%2.2x%2.2x%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
                     uuidbytes.byte0, uuidbytes.byte1, uuidbytes.byte2, uuidbytes.byte3, uuidbytes.byte4, uuidbytes.byte5, uuidbytes.byte6, uuidbytes.byte7,
                    uuidbytes.byte8, uuidbytes.byte9, uuidbytes.byte10, uuidbytes.byte11, uuidbytes.byte12, uuidbytes.byte13, uuidbytes.byte14, uuidbytes.byte15);
        pCMe->m_Id[nSize] = 0;
#elif TARGET_OS_MAC
        if (gethostuuid(id, &timeout) == 0) {
            nSize = 36 + 4;
            pCMe->m_Id = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize + 1);
            uuid_unparse(id, pCMe->m_Id);
            pCMe->m_Id[nSize] = 0;
        }
#endif
#elif OS_ECO
#else /* LINUX */
            nSize = strlen("00000000-0000-0000-0000-000000000000");
            pCMe->m_Id = (char*)pCMe->m_pIMem->Alloc(pCMe->m_pIMem, nSize + 1);
            pCMe->m_pIMem->Copy(pCMe->m_pIMem, pCMe->m_Id, "00000000-0000-0000-0000-000000000000", nSize);
            pCMe->m_Id[nSize] = 0;
#endif
    }

    return pCMe->m_Id;
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
int16_t createCEcoSystemInformation1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoSystemInformation1** ppIInfo) {
    int result = -1;
    IEcoSystem1* pISystem = 0;
    //IEcoMemoryManager1* pIMemMgr = 0;
    //IEcoMemoryAllocator1* pIMem = 0;
    CEcoSystemInformation1* pCMe = 0;

    /* Проверка указателей */
    if (ppIInfo == 0 || pIUnkSystem == 0) {
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
    //pCMe = (CEcoSystemInformation1*)pIMem->Alloc(pIMem, sizeof(CEcoSystemInformation1));

    ///* Сохранение указателя на интерфейс для работы с памятью */
    //pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoSystemInformation1 */
    pCMe->m_VtblIInfo.QueryInterface = CEcoSystemInformation1_QueryInterface;
    pCMe->m_VtblIInfo.AddRef = CEcoSystemInformation1_AddRef;
    pCMe->m_VtblIInfo.Release = CEcoSystemInformation1_Release;
    pCMe->m_VtblIInfo.get_Name = CEcoSystemInformation1_get_Name;
    pCMe->m_VtblIInfo.get_Id = CEcoSystemInformation1_get_Id;

    /* Инициализация данных */
    pCMe->m_Name = 0;
    pCMe->m_Id = 0;

    /* Возврат указателя на интерфейс */
    *ppIInfo = &pCMe->m_VtblIInfo;

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
void deleteCEcoSystemInformation1(/* in */ IEcoSystemInformation1* pIInfo) {
    CEcoSystemInformation1* pCMe = 0;
    //IEcoMemoryAllocator1* pIMem = 0;

    if (pIInfo != 0 ) {
        /* Преобразование */
        pCMe = (CEcoSystemInformation1*)pIInfo;
        //pIMem = pCMe->m_pIMem;
        /* Освобождение */
        //if (pCMe->m_Name != 0) {
        //    pIMem->Free(pIMem, pCMe->m_Name);
        //    pCMe->m_Name = 0;
        //}
        //if (pCMe->m_Id != 0) {
        //    pIMem->Free(pIMem, pCMe->m_Id);
        //    pCMe->m_Id = 0;
        //}
        //pIMem->Free(pIMem, pCMe);
        //pIMem->Release(pIMem);
    }
}
