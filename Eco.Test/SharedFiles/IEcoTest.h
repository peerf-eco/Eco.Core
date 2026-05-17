/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoTest
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoTest.
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECO_TEST_H__
#define __I_ECO_TEST_H__

#include "IEcoBase1.h"

/* IEcoTest IID = {ABCDEF12-3456-7890-ABCD-EF1234567890} */
#ifndef __IID_IEcoTest
static const UGUID IID_IEcoTest = {0x01, 0x10, {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90}};
#endif /* __IID_IEcoTest */

typedef struct IEcoTest* IEcoTestPtr_t;

/* Simple 2D point structure for interop testing */
typedef struct EcoPoint {
    int32_t x;
    int32_t y;
} EcoPoint;

/* Rectangle composed of two nested EcoPoints */
typedef struct EcoRect {
    EcoPoint topLeft;
    EcoPoint bottomRight;
} EcoRect;

/* Tagged-value union — same storage reinterpreted as different types */
typedef union EcoValue {
    int32_t asInt;
    double_t asDouble;
    int32_t asBytes[2];
} EcoValue;

/* Tag identifying which member of EcoValue is valid */
typedef enum EcoValueTag {
    ECO_VALUE_INT = 0,
    ECO_VALUE_DOUBLE = 1
} EcoValueTag;

/* Model containing a union by value — the "tagged variant" pattern */
typedef struct EcoVariant {
    int32_t tag;
    EcoValue value;
} EcoVariant;

/* Comparator callback: returns <0, 0, >0 like strcmp */
typedef int32_t (ECOCALLMETHOD *EcoCompareFunc)(/* in */ int32_t a, /* in */ int32_t b);

typedef struct IEcoTestVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoTestPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoTestPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoTestPtr_t me);

    /* IEcoTest - basic arithmetic */
    int32_t (ECOCALLMETHOD *Addition)(/* in */ IEcoTestPtr_t me, /* in */ int32_t left, /* in */ int32_t right);
    int32_t (ECOCALLMETHOD *Negation)(/* in */ IEcoTestPtr_t me, /* in */ int32_t number);

    /* IEcoTest - floating-point arithmetic */
    double_t (ECOCALLMETHOD *Multiplication)(/* in */ IEcoTestPtr_t me, /* in */ double_t left, /* in */ double_t right);

    /* IEcoTest - arrays & pointers */
    int32_t (ECOCALLMETHOD *SumArray)(/* in */ IEcoTestPtr_t me, /* in */ int32_t* arr, /* in */ uint32_t size);
    int16_t (ECOCALLMETHOD *FillArray)(/* in */ IEcoTestPtr_t me, /* out */ int32_t* arr, /* in */ uint32_t size, /* in */ int32_t value);

    /* IEcoTest - sorting */
    int16_t (ECOCALLMETHOD *SortArray)(/* in */ IEcoTestPtr_t me, /* in, out */ int32_t* arr, /* in */ uint32_t size);

    /* IEcoTest - strings */
    char_t* (ECOCALLMETHOD *GetName)(/* in */ IEcoTestPtr_t me);

    /* IEcoTest - structures */
    int16_t (ECOCALLMETHOD *MakePoint)(/* in */ IEcoTestPtr_t me, /* in */ int32_t x, /* in */ int32_t y, /* out */ EcoPoint* outPoint);
    int32_t (ECOCALLMETHOD *CalculateDistance)(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint* point, /* in */ EcoPoint* other);
    int16_t (ECOCALLMETHOD *Translate)(/* in */ IEcoTestPtr_t me, /* in, out */ EcoPoint* point, /* in */ int32_t dx, /* in */ int32_t dy);

    /* IEcoTest - unions by value and by pointer */
    int16_t (ECOCALLMETHOD *PackInt)(/* in */ IEcoTestPtr_t me, /* in */ int32_t value, /* out */ EcoValue* outValue);
    int16_t (ECOCALLMETHOD *PackDouble)(/* in */ IEcoTestPtr_t me, /* in */ double_t value, /* out */ EcoValue* outValue);
    int32_t (ECOCALLMETHOD *UnpackInt)(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value);
    double_t (ECOCALLMETHOD *UnpackDouble)(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value);
    int32_t (ECOCALLMETHOD *ReinterpretAsInt)(/* in */ IEcoTestPtr_t me, /* in */ EcoValue value);

    /* IEcoTest - nested structures and arrays of structures */
    int16_t (ECOCALLMETHOD *MakeRect)(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint topLeft, /* in */ EcoPoint bottomRight, /* out */ EcoRect* outRect);
    int32_t (ECOCALLMETHOD *RectArea)(/* in */ IEcoTestPtr_t me, /* in */ EcoRect* rect);
    int16_t (ECOCALLMETHOD *SumPoints)(/* in */ IEcoTestPtr_t me, /* in */ EcoPoint* points, /* in */ uint32_t size, /* out */ EcoPoint* outSum);

    /* IEcoTest - tagged variants (model containing a union) */
    int16_t (ECOCALLMETHOD *MakeVariant)(/* in */ IEcoTestPtr_t me, /* in */ int32_t tag, /* in */ EcoValue value, /* out */ EcoVariant* outVariant);
    int32_t (ECOCALLMETHOD *GetVariantTag)(/* in */ IEcoTestPtr_t me, /* in */ EcoVariant* variant);
    int16_t (ECOCALLMETHOD *GetVariantValue)(/* in */ IEcoTestPtr_t me, /* in */ EcoVariant* variant, /* out */ EcoValue* outValue);

    /* IEcoTest - user-supplied comparator callback */
    int16_t (ECOCALLMETHOD *SortArrayWith)(/* in */ IEcoTestPtr_t me, /* in, out */ int32_t* arr, /* in */ uint32_t size, /* in */ EcoCompareFunc cmp);

    /* IEcoTest - string parameter */
    int32_t (ECOCALLMETHOD *StringLength)(/* in */ IEcoTestPtr_t me, /* in */ const char_t* str);

} IEcoTestVTbl, *IEcoTestVTblPtr;

interface IEcoTest {
    struct IEcoTestVTbl *pVTbl;
} IEcoTest;

#endif /* __I_ECO_TEST_H__ */
