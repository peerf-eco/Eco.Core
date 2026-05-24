/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoACOM2Java
 * </summary>
 *
 * <description>
 *   This source file is the entry point
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */


/* Eco OS */
#include <string.h>
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoACOM2Java.h"
#include "IdEcoList1.h"
#include "IdEcoTypeLib1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoTest1.h"

/* Эталонные значения, которые TestVoid и т.д. должны вернуть/положить в out-параметры.
 * Должны совпадать с константами в Java-компоненте CEcoTest1.
 */
#define ECO_TEST1_OUT_INT8     ((int8_t)   -42)
#define ECO_TEST1_OUT_INT16    ((int16_t)  -1234)
#define ECO_TEST1_OUT_INT32    ((int32_t)  -123456)
#define ECO_TEST1_OUT_INT64    ((int64_t)  -1234567890123LL)
#define ECO_TEST1_OUT_UINT8    ((uint8_t)   200)
#define ECO_TEST1_OUT_UINT16   ((uint16_t)  50000)
#define ECO_TEST1_OUT_UINT32   ((uint32_t)  3000000000U)
#define ECO_TEST1_OUT_UINT64   ((uint64_t)  9876543210123ULL)

int16_t TestCalculator(IEcoACOM2Java* pIEcoACOM2Java) {
    UGUID CID_EcoCalculatorJ = {0x01, 0x10, {0x0A, 0x7B, 0x93, 0x7E, 0xD5, 0x0A, 0xC6, 0xC9, 0x4D, 0xFD, 0xA1, 0x28, 0x6B, 0xFD, 0xA9, 0xDC}};
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;
    int16_t result = 0;
    int32_t resultAddition = 0;
    int16_t resultSubtraction = 0;
    int32_t resultMultiplication = 0;
    int16_t resultDivision = 0;

    printf("\n==== Test Calculator ====\n");

    printf("CEcoCalculatorJFactory RegisterComponent");
    result = pIEcoACOM2Java->pVTbl->RegisterComponent(pIEcoACOM2Java, &CID_EcoCalculatorJ, "C:\\Programming\\Eco.Core\\Eco.Java2ACOM\\BuildFiles\\production\\UnitTestFiles", "Eco/Calculator/CEcoCalculatorJFactory");
    if (result != 0) {
        printf(": Fail\n");
        return result;
    }
    printf(": OK\n");

    printf("IEcoCalculatorX QueryComponent");
    result = pIEcoACOM2Java->pVTbl->QueryComponent(pIEcoACOM2Java, &CID_EcoCalculatorJ, 0, &IID_IEcoCalculatorX, (void**) &pIX);
    if (result != 0) {
        printf(": Fail\n");
        if (pIX != 0) {
            pIX->pVTbl->Release(pIX);
        }
        return result;
    }
    printf(": OK\n");

    printf("IEcoCalculatorY QueryInterface");
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**) &pIY);
    if (result != 0) {
        printf(": Fail\n");
        if (pIX != 0) {
            pIX->pVTbl->Release(pIX);
        }
        if (pIY != 0) {
            pIY->pVTbl->Release(pIY);
        }
        return result;
    }
    printf(": OK\n");

    resultAddition = pIX->pVTbl->Addition(pIX, 9, 10);
    printf("Addition: 9 + 10 = %d", resultAddition);
    if (resultAddition == 19) {
        printf(": OK\n");
    } else {
        printf(": Fail\n");
        result = ERR_ECO_FAIL;
    }

    resultSubtraction = pIX->pVTbl->Subtraction(pIX, 67, 13);
    printf("Subtraction: 67 - 13 = %d", resultSubtraction);
    if (resultSubtraction == 54) {
        printf(": OK\n");
    } else {
        printf(": Fail\n");
        result = ERR_ECO_FAIL;
    }

    resultMultiplication = pIY->pVTbl->Multiplication(pIY, 6, 8);
    printf("Multiplication: 6 * 8 = %d", resultMultiplication);
    if (resultMultiplication == 48) {
        printf(": OK\n");
    } else {
        printf(": Fail\n");
        result = ERR_ECO_FAIL;
    }

    resultDivision = pIY->pVTbl->Division(pIY, 42, 7);
    printf("Division: 42 / 7 = %d", resultDivision);
    if (resultDivision == 6) {
        printf(": OK\n");
    } else {
        printf(": Fail\n");
        result = ERR_ECO_FAIL;
    }

    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }
    return result;
}

/* Длина широкой строки без зависимости от <wchar.h>. */
static uint32_t TestComponent_WStrLen(const wchar_t* s) {
    uint32_t n = 0;
    if (s == 0) return 0;
    while (s[n] != 0) n++;
    return n;
}

/* Сравнение широких строк. */
static int TestComponent_WStrEq(const wchar_t* a, const wchar_t* b) {
    if (a == 0 || b == 0) return a == b;
    while (*a != 0 && *a == *b) { a++; b++; }
    return *a == *b;
}

/* Сравнение UGUID по содержимому. */
static int TestComponent_UGUIDEq(const UGUID* a, const UGUID* b) {
    uint8_t i;
    if (a == 0 || b == 0) return a == b;
    if (a->Preamble != b->Preamble || a->Length != b->Length) return 0;
    for (i = 0; i < a->Length; i++) {
        if (a->Data[i] != b->Data[i]) return 0;
    }
    return 1;
}

int16_t TestComponent(IEcoACOM2Java* pIEcoACOM2Java, IEcoMemoryAllocator1* pIMem) {
    UGUID CID_EcoTest1Java = {0x01, 0x10, {0xAD, 0x6F, 0x39, 0x98, 0xBC, 0xFC, 0x47, 0x95, 0x9A, 0xF3, 0xDB, 0x4A, 0x38, 0x54, 0x98, 0x42}};
    IEcoTest1* pITest = 0;
    int16_t result = 0;

    printf("\n==== Test Component ====\n");

    printf("CEcoTest1Factory RegisterComponent");
    result = pIEcoACOM2Java->pVTbl->RegisterComponent(pIEcoACOM2Java, &CID_EcoTest1Java, "C:\\Programming\\Eco.Core\\TestComponents\\Java\\Eco.Test1\\BuildFiles\\artifacts\\Eco_Test1_jar\\Eco.Test1.jar", "Eco/Test/CEcoTest1Factory");
    if (result != 0) {
        printf(": Fail\n");
        return result;
    }
    printf(": OK\n");

    printf("IEcoTest1 QueryComponent");
    result = pIEcoACOM2Java->pVTbl->QueryComponent(pIEcoACOM2Java, &CID_EcoTest1Java, 0, &IID_IEcoTest1, (void**) &pITest);
    if (result != 0) {
        printf(": Fail\n\n");
        if (pITest != 0) {
            pITest->pVTbl->Release(pITest);
        }
        return result;
    }
    printf(": OK\n\n");

    /* ==== TestInt8 ==== */
    {
        int8_t in    = 12;
        int8_t inOut = -23;
        int8_t out   = 0;
        int8_t res   = pITest->pVTbl->TestInt8(pITest, in, &inOut, &out);
        printf("12 * 2 = %d\n", (int)res);
        printf("12 - 23 = %d\n", (int)inOut);
        printf("-42 = %d\n", (int)out);
        if (res == 24 && inOut == -11 && out == ECO_TEST1_OUT_INT8) {
            printf("TestInt8: OK\n\n");
        } else {
            printf("TestInt8: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestInt16 ==== */
    {
        int16_t in    = 1234;
        int16_t inOut = -2345;
        int16_t out   = 0;
        int16_t res   = pITest->pVTbl->TestInt16(pITest, in, &inOut, &out);
        printf("1234 * 2 = %d\n", (int)res);
        printf("1234 - 2345 = %d\n", (int)inOut);
        printf("-1234 = %d\n", (int)out);
        if (res == 2468 && inOut == -1111 && out == ECO_TEST1_OUT_INT16) {
            printf("TestInt16: OK\n\n");
        } else {
            printf("TestInt16: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestInt32 ==== */
    {
        int32_t in    = 123456;
        int32_t inOut = -234567;
        int32_t out   = 0;
        int32_t res   = pITest->pVTbl->TestInt32(pITest, in, &inOut, &out);
        printf("123456 * 2 = %d\n", res);
        printf("123456 - 234567 = %d\n", inOut);
        printf("-123456 = %d\n", out);
        if (res == 246912 && inOut == -111111 && out == ECO_TEST1_OUT_INT32) {
            printf("TestInt32: OK\n\n");
        } else {
            printf("TestInt32: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestInt64 ==== */
    {
        int64_t in    = 12345678901LL;
        int64_t inOut = -23456789012LL;
        int64_t out   = 0;
        int64_t res   = pITest->pVTbl->TestInt64(pITest, in, &inOut, &out);
        printf("12345678901 * 2 = %lld\n", res);
        printf("12345678901 - 23456789012 = %lld\n", inOut);
        printf("-1234567890123 = %lld\n", out);
        if (res == 24691357802LL && inOut == -11111110111LL && out == ECO_TEST1_OUT_INT64) {
            printf("TestInt64: OK\n\n");
        } else {
            printf("TestInt64: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestUInt8 ==== */
    {
        uint8_t in    = 21;
        uint8_t inOut = 32;
        uint8_t out   = 0;
        uint8_t res   = pITest->pVTbl->TestUInt8(pITest, in, &inOut, &out);
        printf("21 * 2 = %u\n", (unsigned)res);
        printf("21 + 32 = %u\n", (unsigned)inOut);
        printf("200 = %u\n", (unsigned)out);
        if (res == 42 && inOut == 53 && out == ECO_TEST1_OUT_UINT8) {
            printf("TestUInt8: OK\n\n");
        } else {
            printf("TestUInt8: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestUInt16 ==== */
    {
        uint16_t in    = 4321;
        uint16_t inOut = 5432;
        uint16_t out   = 0;
        uint16_t res   = pITest->pVTbl->TestUInt16(pITest, in, &inOut, &out);
        printf("4321 * 2 = %u\n", (unsigned)res);
        printf("4321 + 5432 = %u\n", (unsigned)inOut);
        printf("50000 = %u\n", (unsigned)out);
        if (res == 8642 && inOut == 9753 && out == ECO_TEST1_OUT_UINT16) {
            printf("TestUInt16: OK\n\n");
        } else {
            printf("TestUInt16: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestUInt32 ==== */
    {
        uint32_t in    = 654321U;
        uint32_t inOut = 765432U;
        uint32_t out   = 0U;
        uint32_t res   = pITest->pVTbl->TestUInt32(pITest, in, &inOut, &out);
        printf("654321 * 2 = %u\n", res);
        printf("654321 + 765432 = %u\n", inOut);
        printf("3000000000 = %u\n", out);
        if (res == 1308642U && inOut == 1419753U && out == ECO_TEST1_OUT_UINT32) {
            printf("TestUInt32: OK\n\n");
        } else {
            printf("TestUInt32: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestUInt64 ==== */
    {
        uint64_t in    = 10987654321ULL;
        uint64_t inOut = 21098765432ULL;
        uint64_t out   = 0ULL;
        uint64_t res   = pITest->pVTbl->TestUInt64(pITest, in, &inOut, &out);
        printf("10987654321 * 2 = %llu\n", res);
        printf("10987654321 + 21098765432 = %llu\n", inOut);
        printf("9876543210123 = %llu\n", out);
        if (res == 21975308642ULL && inOut == 32086419753ULL && out == ECO_TEST1_OUT_UINT64) {
            printf("TestUInt64: OK\n\n");
        } else {
            printf("TestUInt64: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestFloat ==== */
    {
        float in    = 1.25F;
        float inOut = 2.5F;
        float out   = 0.0F;
        float res   = pITest->pVTbl->TestFloat(pITest, in, &inOut, &out);
        printf("1.25 * 2 = %f\n", res);
        printf("1.25 + 2.5 = %f\n", inOut);
        printf("1.5 = %f\n", out);
        if (res == 2.5F && inOut == 3.75F && out == 1.5F) {
            printf("TestFloat: OK\n\n");
        } else {
            printf("TestFloat: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestDouble ==== */
    {
        double in    = 1.125;
        double inOut = 2.75;
        double out   = 0.0;
        double res   = pITest->pVTbl->TestDouble(pITest, in, &inOut, &out);
        printf("1.125 * 2 = %f\n", res);
        printf("1.125 + 2.75 = %f\n", inOut);
        printf("3.141592653589793 = %f\n", out);
        if (res == 2.25 && inOut == 3.875 && out == 3.1415926535897932) {
            printf("TestDouble: OK\n\n");
        } else {
            printf("TestDouble: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestBoolean ==== */
    {
        bool_t in    = 1;
        bool_t inOut = 0;
        bool_t out   = 0;
        bool_t res   = pITest->pVTbl->TestBoolean(pITest, in, &inOut, &out);
        printf("!true = %d\n", (int)res);
        printf("false ^ true = %d\n", (int)inOut);
        printf("true = %d\n", (int)out);
        if (!res && inOut && out) {
            printf("TestBoolean: OK\n\n");
        } else {
            printf("TestBoolean: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestChar ==== */
    {
        char_t in    = 'A';
        char_t inOut = (char_t)4;
        char_t out   = 0;
        char_t res   = pITest->pVTbl->TestChar(pITest, in, &inOut, &out);
        printf("A + 1 = %c\n", res);
        printf("A + 4 = %c\n", inOut);
        printf("Z = %c\n", out);
        if (res == 'B' && inOut == 'E' && out == 'Z') {
            printf("TestChar: OK\n\n");
        } else {
            printf("TestChar: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestWChar ==== */
    {
        wchar_t in    = 0x0430; /* 'а' */
        wchar_t inOut = 4;
        wchar_t out   = 0;
        wchar_t res   = pITest->pVTbl->TestWChar(pITest, in, &inOut, &out);
        printf("а + 1 = U+%04X\n", (unsigned)res);
        printf("а + 4 = U+%04X\n", (unsigned)inOut);
        printf("я = U+%04X\n", (unsigned)out);
        if (res == 0x0431 /*'б'*/ && inOut == 0x0434 /*'д'*/ && out == 0x044F /*'я'*/) {
            printf("TestWChar: OK\n\n");
        } else {
            printf("TestWChar: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestAString ==== */
    {
        char_t* in    = "IN";
        char_t  inOutBuf[] = "INOUT";
        char_t* inOut = inOutBuf;  /* INOUT-память на стеке: исходное значение */
        char_t* out   = 0;
        char_t* res   = pITest->pVTbl->TestAString(pITest, in, &inOut, &out);
        printf("%s\n", res != 0 ? res : "(null)");
        printf("%s\n", inOut != 0 ? inOut : "(null)");
        printf("%s\n", out != 0 ? out : "(null)");
        if (res != 0 && inOut != 0 && out != 0 &&
            strcmp(res, "RESULT:IN") == 0 &&
            strcmp(inOut, "INOUT:IN") == 0 &&
            strcmp(out, "OUT") == 0) {
            printf("TestAString: OK\n\n");
        } else {
            printf("TestAString: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
        /* Освобождаем строки, выделенные мостом через IEcoMemoryAllocator1. */
        if (inOut != 0 && inOut != inOutBuf) pIMem->pVTbl->Free(pIMem, inOut);
        if (out != 0) pIMem->pVTbl->Free(pIMem, out);
        /* res получается через GetStringUTFChars в мосте, ответственность за освобождение остается на мосте. */
    }

    /* ==== TestWString ==== */
    {
        static const wchar_t inStr[]    = { 1042, 1061, 1054, 1044, 0 };                          /* ВХОД */
        static const wchar_t inOutInit[] = { 1042, 1061, 1054, 1044, L'-', 1042, 1067, 1061, 1054, 1044, 0 }; /* ВХОД-ВЫХОД */
        static const wchar_t expectedRes[] = { 1056, 1045, 1047, 1059, 1051, 1068, 1058, 1040, 1058, L':', 1042, 1061, 1054, 1044, 0 }; /* РЕЗУЛЬТАТ:ВХОД */
        static const wchar_t expectedIO[]  = { 1042, 1061, 1054, 1044, L'-', 1042, 1067, 1061, 1054, 1044, L':', 1042, 1061, 1054, 1044, 0 }; /* ВХОД-ВЫХОД:ВХОД */
        static const wchar_t expectedOut[] = { 1042, 1067, 1061, 1054, 1044, 0 }; /* ВЫХОД */

        wchar_t inOutBuf[sizeof(inOutInit) / sizeof(wchar_t)];
        wchar_t* in    = (wchar_t*)inStr;
        wchar_t* inOut = inOutBuf;
        wchar_t* out   = 0;
        wchar_t* res   = 0;
        uint32_t i;
        for (i = 0; i < sizeof(inOutInit) / sizeof(wchar_t); i++) {
            inOutBuf[i] = inOutInit[i];
        }

        res = pITest->pVTbl->TestWString(pITest, in, &inOut, &out);
        printf("res len = %u\n",   res   != 0 ? TestComponent_WStrLen(res)   : 0u);
        printf("inOut len = %u\n", inOut != 0 ? TestComponent_WStrLen(inOut) : 0u);
        printf("out len = %u\n",   out   != 0 ? TestComponent_WStrLen(out)   : 0u);
        if (res != 0 && inOut != 0 && out != 0 &&
            TestComponent_WStrEq(res,   expectedRes) &&
            TestComponent_WStrEq(inOut, expectedIO)  &&
            TestComponent_WStrEq(out,   expectedOut)) {
            printf("TestWString: OK\n\n");
        } else {
            printf("TestWString: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
        if (inOut != 0 && inOut != inOutBuf) pIMem->pVTbl->Free(pIMem, inOut);
        if (out != 0) pIMem->pVTbl->Free(pIMem, out);
    }

    /* ==== TestUGUID ==== */
    {
        UGUID inUg     = {0x01, 0x10, {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}};
        UGUID inOutBuf = {0x01, 0x10, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
        UGUID expectRes = {0x01, 0x10, {0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}};
        UGUID expectOut = {0x01, 0x10, {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00}};
        UGUID* inOut = &inOutBuf;
        UGUID* out   = 0;
        UGUID* res   = pITest->pVTbl->TestUGUID(pITest, &inUg, &inOut, &out);
        printf("res first byte   = 0x%02X (expected 0xFF)\n", res   != 0 ? (unsigned)res->Data[0]   : 0u);
        printf("inOut first byte = 0x%02X (expected 0x00)\n", inOut != 0 ? (unsigned)inOut->Data[0] : 0u);
        printf("out first byte   = 0x%02X (expected 0x11)\n", out   != 0 ? (unsigned)out->Data[0]   : 0u);
        if (res != 0 && inOut != 0 && out != 0 &&
            TestComponent_UGUIDEq(res,   &expectRes) &&
            TestComponent_UGUIDEq(inOut, &inUg)      &&
            TestComponent_UGUIDEq(out,   &expectOut)) {
            printf("TestUGUID: OK\n\n");
        } else {
            printf("TestUGUID: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
        /* res и out выделены мостом через pIMem (см. JavaObjectToParam). */
        if (res != 0) pIMem->pVTbl->Free(pIMem, res);
        if (out != 0) pIMem->pVTbl->Free(pIMem, out);
        /* inOut указывает на тот же буфер, что и наш inOutBuf, если мост перезаписывает его на месте;
         * если же мост подменил указатель — освобождаем подменённый. */
        if (inOut != 0 && inOut != &inOutBuf) pIMem->pVTbl->Free(pIMem, inOut);
    }

    /* ==== TestInterface ==== */
    {
        IEcoUnknown* in     = (IEcoUnknown*)pITest;
        IEcoUnknown* inOut  = 0;
        IEcoUnknown* out    = 0;
        IEcoUnknown* res    = 0;
        uint32_t count1, count2, count3, count4;

        res = pITest->pVTbl->TestInterface(pITest, in, &inOut, &out);
        count1 = (res   != 0) ? res  ->pVTbl->Release(res)   : 0u;
        printf("res.Release: %u\n", count1);
        count2 = (inOut != 0) ? inOut->pVTbl->Release(inOut) : 0u;
        printf("in_out.Release: %u\n", count2);
        count3 = (out   != 0) ? out  ->pVTbl->AddRef(out)    : 0u;
        printf("out.AddRef: %u\n", count3);
        count4 = pITest->pVTbl->Release((IEcoUnknown*)pITest);
        printf("orig.Release: %u\n", count4);
        if (res != 0 && inOut != 0 && out != 0 && count1 == 2 && count2 == 1 && count3 == 2 && count4 == 1) {
            printf("TestInterface: OK\n\n");
        } else {
            printf("TestInterface: Fail (got NULL pointer from bridge)\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestVoidPtr ==== */
    {
        /* В качестве in мост ожидает указатель, который Java сторона получит как Object.
         * Используем int*, чтобы по адресу можно было сравнить идентичность. */
        int marker = 12345;
        void* in    = &marker;
        void* inOut = 0;
        void* out   = 0;
        void* res   = pITest->pVTbl->TestVoidPtr(pITest, in, &inOut, &out);
        printf("res   = %p (expected %p)\n", res,   in);
        printf("inOut = %p (expected %p)\n", inOut, in);
        printf("out   = %p (expected %p)\n", out,   in);
        if (res == in && inOut == in && out == in) {
            printf("TestVoidPtr: OK\n\n");
        } else {
            printf("TestVoidPtr: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* ==== TestVoid + GetLastVoidInput ==== */
    {
        int32_t output = 0;
        pITest->pVTbl->TestVoid(pITest, 42);
        output = pITest->pVTbl->GetLastVoidInput(pITest);
        printf("output = %d\n", output);
        if (output == 42) {
            printf("TestVoid + GetLastVoidInput: OK\n\n");
        } else {
            printf("TestVoid + GetLastVoidInput: Fail\n\n");
            result = ERR_ECO_FAIL;
        }
    }

    /* pITest уже освобожден внутри теста TestInterface (orig.Release), повторно
     * освобождать не нужно. */
    return result;
}

/*
 *
 * <summary>
 *   EcoMain Function
 * </summary>
 *
 * <description>
 *   EcoMain function - entry point
 * </description>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Pointer to the system interface */
    IEcoSystem1* pISys = 0;
    /* Pointer to the interface for working with the system interface bus */
    IEcoInterfaceBus1* pIBus = 0;
    /* Pointer to the memory management interface */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Pointer to the tested interface */
    IEcoACOM2Java* pIEcoACOM2Java = 0;

    /* System interface check and creation */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Free the system interface in case of an error */
            goto Release;
        }
    }

    /* Getting the interface for working with the interface bus */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Free in case of an error */
        goto Release;
    }
#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoACOM2Java, (IEcoUnknown*)GetIEcoComponentFactoryPtr_C8035C9741CB40F9B104A12C3F41E2AA);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        /* Free in case of an error */
        goto Release;
    }
#endif
    /* Getting the memory management interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Check */
    if (result != 0 || pIMem == 0) {
        /* Free the system interface in case of an error */
        goto Release;
    }

    /* Getting the tested interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoACOM2Java, 0, &IID_IEcoACOM2Java, (void**) &pIEcoACOM2Java);
    if (result != 0 || pIEcoACOM2Java == 0) {
        /* Free interfaces in case of an error */
        goto Release;
    }

    result = pIEcoACOM2Java->pVTbl->CreateJavaVM(pIEcoACOM2Java, "C:\\Programming\\Eco.Core\\Eco.Java2ACOM\\BuildFiles\\artifacts\\Eco_Java2ACOM_jar\\Eco.Java2ACOM.jar", 8 << 20, 16 << 20);
    if (result != 0) {
        goto Release;
    }

    result = TestCalculator(pIEcoACOM2Java);
    if (result != 0) {
        goto Release;
    }

    result = TestComponent(pIEcoACOM2Java, pIMem);

Release:

    /* Free the interface for working with the interface bus */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Free the memory management interface */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Free the tested interfaces */
    if (pIEcoACOM2Java != 0) {
        pIEcoACOM2Java->pVTbl->Release(pIEcoACOM2Java);
    }

    /* Free the system interface */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}
