
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoTestC1_816BDCCA
 * </сводка>
 *
 * <описание>
 *   Реализация компонента CEcoTestC1. Все методы интерфейса IEcoTestC1
 *   рассчитаны на проверку моста Eco.Java2ACOM: каждый поддерживаемый
 *   тип передается во всех трех режимах (in/inout/out) и возвращается
 *   из функции (где это допустимо). Преобразования детерминированы и
 *   воспроизводимы, что позволяет легко сверять результаты в Java-тесте.
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include <string.h>

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoTestC1.h"

/* Эталонные значения, которые методы кладут в out-параметры. */
#define ECO_TESTC1_OUT_INT8     ((int8_t)   -42)
#define ECO_TESTC1_OUT_INT16    ((int16_t)  -1234)
#define ECO_TESTC1_OUT_INT32    ((int32_t)  -123456)
#define ECO_TESTC1_OUT_INT64    ((int64_t)  -1234567890123LL)
#define ECO_TESTC1_OUT_UINT8    ((uint8_t)   200)
#define ECO_TESTC1_OUT_UINT16   ((uint16_t)  50000)
#define ECO_TESTC1_OUT_UINT32   ((uint32_t)  3000000000U)
#define ECO_TESTC1_OUT_UINT64   ((uint64_t)  9876543210123ULL)
#define ECO_TESTC1_OUT_FLOAT    (1.5f)
#define ECO_TESTC1_OUT_DOUBLE   (3.1415926535897932)
#define ECO_TESTC1_OUT_BOOL     ((bool_t)1)
#define ECO_TESTC1_OUT_CHAR     ('Z')
#define ECO_TESTC1_OUT_WCHAR    ((wchar_t)0x044F) /* кириллическая 'я' */

/* Эталонный UGUID для out-параметра TestUGUID. */
static const UGUID ECO_TESTC1_OUT_UGUID = {0x01, 0x10, {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00
}};

/* ===== Вспомогательные функции для работы со строками через IEcoMemoryAllocator1 ===== */

static uint32_t TestC1_AStrLen(const char_t* src) {
    uint32_t len = 0;
    if (src == 0) return 0;
    while (src[len] != 0) len++;
    return len;
}

static uint32_t TestC1_WStrLen(const wchar_t* src) {
    uint32_t len = 0;
    if (src == 0) return 0;
    while (src[len] != 0) len++;
    return len;
}

static char_t* TestC1_DupAString(IEcoMemoryAllocator1* pIMem, const char_t* src) {
    uint32_t len = TestC1_AStrLen(src);
    char_t* dst = (char_t*)pIMem->pVTbl->Alloc(pIMem, len + 1);
    if (dst != 0) {
        if (len != 0) {
            memcpy(dst, src, len);
        }
        dst[len] = 0;
    }
    return dst;
}

static wchar_t* TestC1_DupWString(IEcoMemoryAllocator1* pIMem, const wchar_t* src) {
    uint32_t len = TestC1_WStrLen(src);
    wchar_t* dst = (wchar_t*)pIMem->pVTbl->Alloc(pIMem, (len + 1) * (uint32_t)sizeof(wchar_t));
    if (dst != 0) {
        if (len != 0) {
            memcpy(dst, src, len * sizeof(wchar_t));
        }
        dst[len] = 0;
    }
    return dst;
}

/* Формирует ASCII-строку вида "<prefix>:<src>". */
static char_t* TestC1_PrefixedAString(IEcoMemoryAllocator1* pIMem, const char_t* prefix, const char_t* src) {
    uint32_t prefixLen = TestC1_AStrLen(prefix);
    uint32_t srcLen = TestC1_AStrLen(src);
    uint32_t total = prefixLen + 1 /*':'*/ + srcLen;
    char_t* dst = (char_t*)pIMem->pVTbl->Alloc(pIMem, total + 1);
    if (dst != 0) {
        memcpy(dst, prefix, prefixLen);
        dst[prefixLen] = ':';
        if (srcLen != 0) {
            memcpy(dst + prefixLen + 1, src, srcLen);
        }
        dst[total] = 0;
    }
    return dst;
}

/* Формирует широкую строку вида "<prefix>:<src>". prefix задается ASCII-литералом. */
static wchar_t* TestC1_PrefixedWString(IEcoMemoryAllocator1* pIMem, const char_t* prefix, const wchar_t* src) {
    uint32_t prefixLen = TestC1_AStrLen(prefix);
    uint32_t srcLen = TestC1_WStrLen(src);
    uint32_t total = prefixLen + 1 /*':'*/ + srcLen;
    wchar_t* dst = (wchar_t*)pIMem->pVTbl->Alloc(pIMem, (total + 1) * (uint32_t)sizeof(wchar_t));
    uint32_t i = 0;
    if (dst == 0) {
        return 0;
    }
    for (i = 0; i < prefixLen; i++) {
        dst[i] = (wchar_t)(unsigned char)prefix[i];
    }
    dst[prefixLen] = (wchar_t)':';
    if (srcLen != 0) {
        memcpy(dst + prefixLen + 1, src, srcLen * sizeof(wchar_t));
    }
    dst[total] = 0;
    return dst;
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 */
static int16_t ECOCALLMETHOD CEcoTestC1_816BDCCA_QueryInterface(/* in */ IEcoTestC1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoTestC1) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoTestC1;
        pCMe->m_pVTblIEcoTestC1->AddRef((IEcoTestC1*)pCMe);
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
 */
static uint32_t ECOCALLMETHOD CEcoTestC1_816BDCCA_AddRef(/* in */ IEcoTestC1Ptr_t me) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;

    if (me == 0 ) {
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
 */
static uint32_t ECOCALLMETHOD CEcoTestC1_816BDCCA_Release(/* in */ IEcoTestC1Ptr_t me) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;

    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/* ===== Числовые типы ===== */

#define ECO_TESTC1_NUMERIC_BODY(Type, OutConst)         \
    if (me == 0) return 0;                              \
    if (inOut != 0) *inOut = (Type)(*inOut + in);       \
    if (out   != 0) *out   = (Type)(OutConst);          \
    return (Type)(in * 2)

static int8_t   ECOCALLMETHOD CEcoTestC1_816BDCCA_TestInt8(IEcoTestC1Ptr_t me, int8_t in, int8_t* inOut, int8_t* out) {
    ECO_TESTC1_NUMERIC_BODY(int8_t, ECO_TESTC1_OUT_INT8);
}

static int16_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestInt16(IEcoTestC1Ptr_t me, int16_t in, int16_t* inOut, int16_t* out) {
    ECO_TESTC1_NUMERIC_BODY(int16_t, ECO_TESTC1_OUT_INT16);
}

static int32_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestInt32(IEcoTestC1Ptr_t me, int32_t in, int32_t* inOut, int32_t* out) {
    ECO_TESTC1_NUMERIC_BODY(int32_t, ECO_TESTC1_OUT_INT32);
}

static int64_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestInt64(IEcoTestC1Ptr_t me, int64_t in, int64_t* inOut, int64_t* out) {
    ECO_TESTC1_NUMERIC_BODY(int64_t, ECO_TESTC1_OUT_INT64);
}

static uint8_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestUInt8(IEcoTestC1Ptr_t me, uint8_t in, uint8_t* inOut, uint8_t* out) {
    ECO_TESTC1_NUMERIC_BODY(uint8_t, ECO_TESTC1_OUT_UINT8);
}

static uint16_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestUInt16(IEcoTestC1Ptr_t me, uint16_t in, uint16_t* inOut, uint16_t* out) {
    ECO_TESTC1_NUMERIC_BODY(uint16_t, ECO_TESTC1_OUT_UINT16);
}

static uint32_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestUInt32(IEcoTestC1Ptr_t me, uint32_t in, uint32_t* inOut, uint32_t* out) {
    ECO_TESTC1_NUMERIC_BODY(uint32_t, ECO_TESTC1_OUT_UINT32);
}

static uint64_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestUInt64(IEcoTestC1Ptr_t me, uint64_t in, uint64_t* inOut, uint64_t* out) {
    ECO_TESTC1_NUMERIC_BODY(uint64_t, ECO_TESTC1_OUT_UINT64);
}

static float   ECOCALLMETHOD CEcoTestC1_816BDCCA_TestFloat(IEcoTestC1Ptr_t me, float in, float* inOut, float* out) {
    if (me == 0) return 0.0f;
    if (inOut != 0) *inOut = *inOut + in;
    if (out   != 0) *out   = ECO_TESTC1_OUT_FLOAT;
    return in * 2.0f;
}

static double  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestDouble(IEcoTestC1Ptr_t me, double in, double* inOut, double* out) {
    if (me == 0) return 0.0;
    if (inOut != 0) *inOut = *inOut + in;
    if (out   != 0) *out   = ECO_TESTC1_OUT_DOUBLE;
    return in * 2.0;
}

/* ===== Логические/символьные типы ===== */

static bool_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestBoolean(IEcoTestC1Ptr_t me, bool_t in, bool_t* inOut, bool_t* out) {
    if (me == 0) return 0;
    if (inOut != 0) *inOut = (bool_t)((*inOut) ^ in);
    if (out   != 0) *out   = ECO_TESTC1_OUT_BOOL;
    return (bool_t)(!in);
}

static char_t  ECOCALLMETHOD CEcoTestC1_816BDCCA_TestChar(IEcoTestC1Ptr_t me, char_t in, char_t* inOut, char_t* out) {
    if (me == 0) return 0;
    if (inOut != 0) *inOut = (char_t)(*inOut + in);
    if (out   != 0) *out   = ECO_TESTC1_OUT_CHAR;
    return (char_t)(in + 1);
}

static wchar_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestWChar(IEcoTestC1Ptr_t me, wchar_t in, wchar_t* inOut, wchar_t* out) {
    if (me == 0) return 0;
    if (inOut != 0) *inOut = (wchar_t)(*inOut + in);
    if (out   != 0) *out   = ECO_TESTC1_OUT_WCHAR;
    return (wchar_t)(in + 1);
}

/* ===== Строки ===== */

static char_t* ECOCALLMETHOD CEcoTestC1_816BDCCA_TestAString(IEcoTestC1Ptr_t me, char_t* in, char_t** inOut, char_t** out) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    IEcoMemoryAllocator1* pIMem = 0;

    if (me == 0 || pCMe->m_pIMem == 0) {
        return 0;
    }
    pIMem = pCMe->m_pIMem;

    if (inOut != 0) {
        char_t* oldVal = *inOut;
        char_t* combined = TestC1_PrefixedAString(pIMem, (oldVal != 0) ? oldVal : "", in);
        if (oldVal != 0) {
            pIMem->pVTbl->Free(pIMem, oldVal);
        }
        *inOut = combined;
    }

    if (out != 0) {
        *out = TestC1_DupAString(pIMem, "OUT");
    }

    return TestC1_PrefixedAString(pIMem, "RESULT", in);
}

static wchar_t* ECOCALLMETHOD CEcoTestC1_816BDCCA_TestWString(IEcoTestC1Ptr_t me, wchar_t* in, wchar_t** inOut, wchar_t** out) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    IEcoMemoryAllocator1* pIMem = 0;
    static const wchar_t emptyWStr[] = { 0 };
    static const wchar_t outWStr[]   = { L'O', L'U', L'T', 0 };

    if (me == 0 || pCMe->m_pIMem == 0) {
        return 0;
    }
    pIMem = pCMe->m_pIMem;

    if (inOut != 0) {
        wchar_t* oldVal = *inOut;
        wchar_t* combined = TestC1_PrefixedWString(pIMem, "INOUT", (oldVal != 0) ? oldVal : emptyWStr);
        /*
         * Игнорируем входное in для INOUT, т.к. префикс уже несет смысловую
         * нагрузку. Тестируется лишь корректность чтения старого значения и
         * записи нового.
         */
        (void)in;
        if (oldVal != 0) {
            pIMem->pVTbl->Free(pIMem, oldVal);
        }
        *inOut = combined;
    }

    if (out != 0) {
        *out = TestC1_DupWString(pIMem, outWStr);
    }

    return TestC1_PrefixedWString(pIMem, "RESULT", (in != 0) ? in : emptyWStr);
}

/* ===== UGUID ===== */

static UGUID* ECOCALLMETHOD CEcoTestC1_816BDCCA_TestUGUID(IEcoTestC1Ptr_t me, const UGUID* in, UGUID** inOut, UGUID** out) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    IEcoMemoryAllocator1* pIMem = 0;
    UGUID* result = 0;

    if (me == 0 || pCMe->m_pIMem == 0 || in == 0) {
        return 0;
    }
    pIMem = pCMe->m_pIMem;

    /* result: копия in с инвертированным первым байтом данных (детерминированное преобразование). */
    result = (UGUID*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)sizeof(UGUID));
    if (result != 0) {
        *result = *in;
        if (result->Length > 0) {
            result->Data[0] = (byte_t)(~result->Data[0]);
        }
    }

    /* inOut: освобождаем старое значение, кладем копию in. */
    if (inOut != 0) {
        UGUID* copy = (UGUID*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)sizeof(UGUID));
        if (copy != 0) {
            *copy = *in;
        }
        if (*inOut != 0) {
            pIMem->pVTbl->Free(pIMem, *inOut);
        }
        *inOut = copy;
    }

    /* out: эталонный UGUID. */
    if (out != 0) {
        UGUID* outCopy = (UGUID*)pIMem->pVTbl->Alloc(pIMem, (uint32_t)sizeof(UGUID));
        if (outCopy != 0) {
            *outCopy = ECO_TESTC1_OUT_UGUID;
        }
        *out = outCopy;
    }

    return result;
}

/* ===== Интерфейс ===== */

static int16_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestInterface(IEcoTestC1Ptr_t me, IEcoUnknown* in, IEcoUnknown** inOut, IEcoUnknown** out) {
    if (me == 0 || in == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверяем целостность счетчика ссылок in. */
    in->pVTbl->AddRef(in);
    in->pVTbl->Release(in);

    if (inOut != 0) {
        if (*inOut != 0) {
            (*inOut)->pVTbl->Release(*inOut);
        }
        in->pVTbl->AddRef(in);
        *inOut = in;
    }

    if (out != 0) {
        in->pVTbl->AddRef(in);
        *out = in;
    }

    return ERR_ECO_SUCCESES;
}

/* ===== VoidPtr ===== */

static voidptr_t ECOCALLMETHOD CEcoTestC1_816BDCCA_TestVoidPtr(IEcoTestC1Ptr_t me, voidptr_t in, voidptr_t* inOut, voidptr_t* out) {
    if (me == 0) {
        return 0;
    }
    if (inOut != 0) {
        *inOut = in;
    }
    if (out != 0) {
        *out = in;
    }
    return in;
}

/* ===== VOID результат ===== */

static void ECOCALLMETHOD CEcoTestC1_816BDCCA_TestVoid(IEcoTestC1Ptr_t me, int32_t in) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    if (me == 0) {
        return;
    }
    pCMe->m_lastVoidInput = in;
}

static int32_t ECOCALLMETHOD CEcoTestC1_816BDCCA_GetLastVoidInput(IEcoTestC1Ptr_t me) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    if (me == 0) {
        return 0;
    }
    return pCMe->m_lastVoidInput;
}

/* ===== Init / Create / Delete ===== */

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 */
static int16_t ECOCALLMETHOD initCEcoTestC1_816BDCCA(/*in*/ CEcoTestC1_816BDCCAPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoTestC1_816BDCCA* pCMe = (CEcoTestC1_816BDCCA*)me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    if (me == 0 ) {
        return result;
    }

    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        return result;
    }

    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 */
static int16_t ECOCALLMETHOD createCEcoTestC1_816BDCCA(/* in */ CEcoTestC1_816BDCCAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    if (pCMe == 0) {
        return ERR_ECO_POINTER;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 */
static void ECOCALLMETHOD deleteCEcoTestC1_816BDCCA(/* in */ CEcoTestC1_816BDCCAPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        if (pIMem != 0) {
            pIMem->pVTbl->Free(pIMem, pCMe);
            pIMem->pVTbl->Release(pIMem);
        }
    }
}


/* Виртуальная таблица IEcoTestC1 */
IEcoTestC1VTbl g_xA788E7F1D0584870A1511A9A75EFC821VTbl_816BDCCA = {
    CEcoTestC1_816BDCCA_QueryInterface,
    CEcoTestC1_816BDCCA_AddRef,
    CEcoTestC1_816BDCCA_Release,

    CEcoTestC1_816BDCCA_TestInt8,
    CEcoTestC1_816BDCCA_TestInt16,
    CEcoTestC1_816BDCCA_TestInt32,
    CEcoTestC1_816BDCCA_TestInt64,

    CEcoTestC1_816BDCCA_TestUInt8,
    CEcoTestC1_816BDCCA_TestUInt16,
    CEcoTestC1_816BDCCA_TestUInt32,
    CEcoTestC1_816BDCCA_TestUInt64,

    CEcoTestC1_816BDCCA_TestFloat,
    CEcoTestC1_816BDCCA_TestDouble,

    CEcoTestC1_816BDCCA_TestBoolean,
    CEcoTestC1_816BDCCA_TestChar,
    CEcoTestC1_816BDCCA_TestWChar,

    CEcoTestC1_816BDCCA_TestAString,
    CEcoTestC1_816BDCCA_TestWString,

    CEcoTestC1_816BDCCA_TestUGUID,
    CEcoTestC1_816BDCCA_TestInterface,
    CEcoTestC1_816BDCCA_TestVoidPtr,

    CEcoTestC1_816BDCCA_TestVoid,
    CEcoTestC1_816BDCCA_GetLastVoidInput
};


/* Экземпляр объекта */
CEcoTestC1_816BDCCA g_xCEcoTestC1_816BDCCA = {
    &g_xA788E7F1D0584870A1511A9A75EFC821VTbl_816BDCCA,
    initCEcoTestC1_816BDCCA,
    createCEcoTestC1_816BDCCA,
    deleteCEcoTestC1_816BDCCA,
    1, /* m_cRef       */
    0, /* m_pIMem      */
    0, /* m_pISys      */
    0  /* m_lastVoidInput */
};
