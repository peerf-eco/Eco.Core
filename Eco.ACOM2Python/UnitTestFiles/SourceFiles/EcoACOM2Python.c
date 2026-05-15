/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoACOM2Python
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
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoACOM2Python.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include <stdio.h>


/* -----------------------------------------------------------------------------
 * Test setup
 * ----------------------------------------------------------------------------- */

#define ECO_EXAMPLE_PATH_SIMPLE       "AbsolutePathToThePythonFile"
#define ECO_EXAMPLE_PATH_INNER        "AbsolutePathToThePythonFile"
#define ECO_EXAMPLE_PATH_INCLUSION    "AbsolutePathToThePythonFile"
#define ECO_EXAMPLE_PATH_OUTER        "AbsolutePathToThePythonFile"

/* Selects which Python-side calculator to exercise.
 *   1 = simple    (CID_EcoCalculator)          — non-aggregatable IX + IY
 *   2 = inner     (CID_EcoCalculatorInner)     — aggregatable, IX only
 *   3 = inclusion (CID_EcoCalculatorInclusion) — IX delegated to 'simple', IY local
 *   4 = outer     (CID_EcoCalculatorOuter)     — aggregates inner, IY local
 */
#define ECO_TEST_VARIANT 1


/* -----------------------------------------------------------------------------
 * Pretty printing
 * ----------------------------------------------------------------------------- */

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"

#define TAG_OK    COLOR_GREEN "[OK]"   COLOR_RESET
#define TAG_FAIL  COLOR_RED   "[FAIL]" COLOR_RESET

/* Print a UGUID as XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX. */
static void PrintGUID(const UGUID* guid) {
    const uint8_t* data = guid->Data;
    printf(COLOR_CYAN "{%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X}" COLOR_RESET,
        data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
        data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
}

static void ReportOk(const char* oper) {
    printf("  " TAG_OK " %s\n", oper);
}

static void ReportFail(const char* oper, int16_t code) {
    printf("  " TAG_FAIL " %s " COLOR_YELLOW "(code = 0x%04X)" COLOR_RESET "\n", oper, (uint16_t)code);
}

static void ReportRegister(const char* name, const UGUID* cid) {
    printf("  " TAG_OK " RegisterComponent " COLOR_BOLD "%s" COLOR_RESET " as ", name);
    PrintGUID(cid);
    printf("\n");
}

static void ReportQuery(const char* cid_name, const UGUID* cid, const char* iid_name, const UGUID* iid) {
    printf("  " TAG_OK " QueryComponent " COLOR_BOLD "%s" COLOR_RESET " ", cid_name);
    PrintGUID(cid);
    printf(",\n                      " COLOR_BOLD "%s" COLOR_RESET " ", iid_name);
    PrintGUID(iid);
    printf("\n");
}


/* -----------------------------------------------------------------------------
 * Variant-dependent labels
 * ----------------------------------------------------------------------------- */

#if   ECO_TEST_VARIANT == 1
    #define ECO_VARIANT_NAME "simple"
    #define ECO_VARIANT_CID  CID_EcoCalculator
    #define ECO_VARIANT_CID_NAME "CID_EcoCalculator"
#elif ECO_TEST_VARIANT == 2
    #define ECO_VARIANT_NAME "inner"
    #define ECO_VARIANT_CID  CID_EcoCalculatorInner
    #define ECO_VARIANT_CID_NAME "CID_EcoCalculatorInner"
#elif ECO_TEST_VARIANT == 3
    #define ECO_VARIANT_NAME "inclusion"
    #define ECO_VARIANT_CID  CID_EcoCalculatorInclusion
    #define ECO_VARIANT_CID_NAME "CID_EcoCalculatorInclusion"
#elif ECO_TEST_VARIANT == 4
    #define ECO_VARIANT_NAME "outer"
    #define ECO_VARIANT_CID  CID_EcoCalculatorOuter
    #define ECO_VARIANT_CID_NAME "CID_EcoCalculatorOuter"
#endif


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

    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoACOM2Python* pIEcoACOM2Python = 0;
    IEcoCalculatorX* pIX = 0;
    IEcoCalculatorY* pIY = 0;

    UGUID CID_EcoCalculator          = { 0x01, 0x10, {0x48, 0x28, 0xF6, 0x55, 0x2E, 0x45, 0x40, 0xE7, 0x81, 0x21, 0xEB, 0xD2, 0x20, 0xDC, 0x36, 0x0E} };
    UGUID CID_EcoCalculatorInner     = { 0x01, 0x10, {0xAE, 0x20, 0x2E, 0x54, 0x3C, 0xE5, 0x45, 0x50, 0x89, 0x96, 0x03, 0xBD, 0x70, 0xC6, 0x25, 0x65} };
    UGUID CID_EcoCalculatorInclusion = { 0x01, 0x10, {0x34, 0x8E, 0x44, 0x67, 0x7E, 0x82, 0x47, 0x5C, 0xB4, 0xA3, 0x71, 0x9E, 0xD8, 0x39, 0x7E, 0x61} };
    UGUID CID_EcoCalculatorOuter     = { 0x01, 0x10, {0x87, 0x2F, 0xEF, 0xD1, 0xE3, 0x31, 0x48, 0x87, 0xAD, 0x44, 0xD1, 0xE7, 0xC2, 0x32, 0xC2, 0xF0} };

    printf("\n" COLOR_BOLD COLOR_MAGENTA "  == Eco.ACOM2Python · variant: '%s' ==" COLOR_RESET "\n\n", ECO_VARIANT_NAME);

    /* System interface */
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 || pISys == 0) {
        ReportFail("QueryInterface [GID_IEcoSystem]", result);
        goto Release;
    }
    ReportOk("QueryInterface [GID_IEcoSystem]");

    /* Interface bus — through the system interface */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) {
        ReportFail("QueryInterface [IID_IEcoInterfaceBus1]", result);
        goto Release;
    }
    ReportOk("QueryInterface [IID_IEcoInterfaceBus1]");

#ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoACOM2Python, (IEcoUnknown*)GetIEcoComponentFactoryPtr_219EDB626EF14B42BE16F93A566F1CC3);
    if (result != 0) {
	    ReportFail("RegisterComponent [CID_EcoACOM2Python]", result); 
        goto Release;
    }
    ReportRegister("EcoACOM2Python", &CID_EcoACOM2Python);
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0) {
	    ReportFail("RegisterComponent [CID_EcoList1]", result); 
        goto Release;
    }
    ReportRegister("EcoList1", &CID_EcoList1);
#endif

    /* Python bridge — through the interface bus */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoACOM2Python, (void**)&pIEcoACOM2Python);
    if (result != 0 || pIEcoACOM2Python == 0) {
        ReportFail("QueryInterface [IID_IEcoACOM2Python]", result);
        goto Release;
    }
    ReportOk("QueryInterface [IID_IEcoACOM2Python]");

    /* Register every Python calculator the chosen variant depends on */
#if ECO_TEST_VARIANT == 1 || ECO_TEST_VARIANT == 3
    result = pIEcoACOM2Python->pVTbl->RegisterComponent(pIEcoACOM2Python, ECO_EXAMPLE_PATH_SIMPLE, &CID_EcoCalculator);
    if (result != 0) { 
        ReportFail("RegisterComponent [simple]", result); 
        goto Release; 
    }
    ReportRegister("simple", &CID_EcoCalculator);
#endif
#if ECO_TEST_VARIANT == 2 || ECO_TEST_VARIANT == 4
    result = pIEcoACOM2Python->pVTbl->RegisterComponent(pIEcoACOM2Python, ECO_EXAMPLE_PATH_INNER, &CID_EcoCalculatorInner);
    if (result != 0) { 
        ReportFail("RegisterComponent [inner]", result); 
        goto Release; 
    }
    ReportRegister("inner", &CID_EcoCalculatorInner);
#endif
#if ECO_TEST_VARIANT == 3
    result = pIEcoACOM2Python->pVTbl->RegisterComponent(pIEcoACOM2Python, ECO_EXAMPLE_PATH_INCLUSION, &CID_EcoCalculatorInclusion);
    if (result != 0) { 
        ReportFail("RegisterComponent [inclusion]", result); 
        goto Release; 
    }
    ReportRegister("inclusion", &CID_EcoCalculatorInclusion);
#endif
#if ECO_TEST_VARIANT == 4
    result = pIEcoACOM2Python->pVTbl->RegisterComponent(pIEcoACOM2Python, ECO_EXAMPLE_PATH_OUTER, &CID_EcoCalculatorOuter);
    if (result != 0) { 
        ReportFail("RegisterComponent [outer]", result); 
        goto Release; 
    }
    ReportRegister("outer", &CID_EcoCalculatorOuter);
#endif

    /* Query the selected variant's X interface */
    result = pIEcoACOM2Python->pVTbl->QueryComponent(pIEcoACOM2Python, &ECO_VARIANT_CID, 0, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result != 0 || pIX == 0) {
        ReportFail("QueryComponent [" ECO_VARIANT_CID_NAME ", IID_IEcoCalculatorX]", result);
        goto Release;
    }
    ReportQuery(ECO_VARIANT_CID_NAME, &ECO_VARIANT_CID, "IID_IEcoCalculatorX", &IID_IEcoCalculatorX);

    printf("\n" COLOR_BOLD "  IEcoCalculatorX" COLOR_RESET "\n");
    printf("  >    10 + 10 = " COLOR_YELLOW "%d" COLOR_RESET "\n", pIX->pVTbl->Addition(pIX, 10, 10));
    printf("  >    30 - 45 = " COLOR_YELLOW "%d" COLOR_RESET "\n\n", pIX->pVTbl->Subtraction(pIX, 30, 45));

#if ECO_TEST_VARIANT != 2
    /* Variants 1 / 3 / 4 also expose Y interface */
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result != 0 || pIY == 0) {
        ReportFail("QueryInterface [IID_IEcoCalculatorY]", result);
        goto Release;
    }
    ReportOk("QueryInterface [IID_IEcoCalculatorY]");

    printf("\n" COLOR_BOLD "  IEcoCalculatorY" COLOR_RESET "\n");
    printf("  >     5 *  8 = " COLOR_YELLOW "%d" COLOR_RESET "\n", pIY->pVTbl->Multiplication(pIY, 5, 8));
    printf("  >    42 /  7 = " COLOR_YELLOW "%d" COLOR_RESET "\n", pIY->pVTbl->Division(pIY, 42, 7));
#endif

Release:

    if (pIX != 0) pIX->pVTbl->Release(pIX);
    if (pIY != 0) pIY->pVTbl->Release(pIY);
    if (pIEcoACOM2Python != 0) pIEcoACOM2Python->pVTbl->Release(pIEcoACOM2Python);
    if (pIBus != 0) pIBus->pVTbl->Release(pIBus);
    if (pISys != 0) pISys->pVTbl->Release(pISys);

    return result;
}
