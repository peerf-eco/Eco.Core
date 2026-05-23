
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoTest1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoTest1.
 *
 *   Интерфейс предназначен для проверки моста Eco.Java2ACOM: для каждого
 *   типа данных, поддерживаемого библиотекой типов Eco.TypeLib1, в нем
 *   присутствует метод, принимающий один параметр в каждом из режимов
 *   (in / inout / out) и возвращающий значение этого же типа. Это
 *   позволяет покрыть все комбинации передачи и возврата значений.
 *
 *   Преобразования, выполняемые методами, детерминированы, поэтому
 *   результаты вызовов легко проверяются в Java-тесте.
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECOTEST1_H__
#define __I_ECOTEST1_H__

#include "IEcoBase1.h"

/* IEcoTest1 IID = {A788E7F1-D058-4870-A151-1A9A75EFC821} */
#ifndef __IID_IEcoTest1
static const UGUID IID_IEcoTest1 = {0x01, 0x10, {0xA7, 0x88, 0xE7, 0xF1, 0xD0, 0x58, 0x48, 0x70, 0xA1, 0x51, 0x1A, 0x9A, 0x75, 0xEF, 0xC8, 0x21}};
#endif /* __IID_IEcoTest1 */

typedef struct IEcoTest1* IEcoTest1Ptr_t;

typedef struct IEcoTest1VTbl {

    /* IEcoUnknown */
    int16_t  (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoTest1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoTest1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoTest1Ptr_t me);

    /* IEcoTest1
     *
     * Для каждого скалярного типа метод выполняет следующие действия:
     *   - возвращает (in * 2);
     *   - присваивает (*inOut += in);
     *   - присваивает *out известным эталонным значением.
     */
    int8_t   (ECOCALLMETHOD *TestInt8)   (/* in */ IEcoTest1Ptr_t me, /* in */ int8_t   in, /* inout */ int8_t*   inOut, /* out */ int8_t*   out);
    int16_t  (ECOCALLMETHOD *TestInt16)  (/* in */ IEcoTest1Ptr_t me, /* in */ int16_t  in, /* inout */ int16_t*  inOut, /* out */ int16_t*  out);
    int32_t  (ECOCALLMETHOD *TestInt32)  (/* in */ IEcoTest1Ptr_t me, /* in */ int32_t  in, /* inout */ int32_t*  inOut, /* out */ int32_t*  out);
    int64_t  (ECOCALLMETHOD *TestInt64)  (/* in */ IEcoTest1Ptr_t me, /* in */ int64_t  in, /* inout */ int64_t*  inOut, /* out */ int64_t*  out);

    uint8_t  (ECOCALLMETHOD *TestUInt8)  (/* in */ IEcoTest1Ptr_t me, /* in */ uint8_t  in, /* inout */ uint8_t*  inOut, /* out */ uint8_t*  out);
    uint16_t (ECOCALLMETHOD *TestUInt16) (/* in */ IEcoTest1Ptr_t me, /* in */ uint16_t in, /* inout */ uint16_t* inOut, /* out */ uint16_t* out);
    uint32_t (ECOCALLMETHOD *TestUInt32) (/* in */ IEcoTest1Ptr_t me, /* in */ uint32_t in, /* inout */ uint32_t* inOut, /* out */ uint32_t* out);
    uint64_t (ECOCALLMETHOD *TestUInt64) (/* in */ IEcoTest1Ptr_t me, /* in */ uint64_t in, /* inout */ uint64_t* inOut, /* out */ uint64_t* out);

    float    (ECOCALLMETHOD *TestFloat)  (/* in */ IEcoTest1Ptr_t me, /* in */ float    in, /* inout */ float*    inOut, /* out */ float*    out);
    double   (ECOCALLMETHOD *TestDouble) (/* in */ IEcoTest1Ptr_t me, /* in */ double   in, /* inout */ double*   inOut, /* out */ double*   out);

    /* result = !in;  *inOut = (*inOut) ^ in;  *out = TRUE. */
    bool_t   (ECOCALLMETHOD *TestBoolean)(/* in */ IEcoTest1Ptr_t me, /* in */ bool_t   in, /* inout */ bool_t*   inOut, /* out */ bool_t*   out);

    /* result = in + 1;  *inOut = *inOut + in;  *out = эталонный символ. */
    char_t   (ECOCALLMETHOD *TestChar)   (/* in */ IEcoTest1Ptr_t me, /* in */ char_t   in, /* inout */ char_t*   inOut, /* out */ char_t*   out);
    wchar_t  (ECOCALLMETHOD *TestWChar)  (/* in */ IEcoTest1Ptr_t me, /* in */ wchar_t  in, /* inout */ wchar_t*  inOut, /* out */ wchar_t*  out);

    /*
     * Строки:
     *   result   = новая строка "RESULT:<in>";
     *   *inOut   = новая строка "<старое значение>:<in>";
     *   *out     = новая строка "OUT".
     */
    char_t*  (ECOCALLMETHOD *TestAString)(/* in */ IEcoTest1Ptr_t me, /* in */ char_t*  in, /* inout */ char_t**  inOut, /* out */ char_t**  out);

    /*
     * Строки:
     *   result   = новая строка "РЕЗУЛЬТАТ:<in>";
     *   *inOut   = новая строка "<старое значение>:<in>";
     *   *out     = новая строка "ВЫХОД".
     */
    wchar_t* (ECOCALLMETHOD *TestWString)(/* in */ IEcoTest1Ptr_t me, /* in */ wchar_t* in, /* inout */ wchar_t** inOut, /* out */ wchar_t** out);

    /*
     * UGUID:
     *   result  = копия in (с инвертированным первым байтом);
     *   *inOut  = копия in;
     *   *out    = известный эталонный UGUID.
     */
    UGUID*   (ECOCALLMETHOD *TestUGUID)  (/* in */ IEcoTest1Ptr_t me, /* in */ const UGUID* in, /* inout */ UGUID** inOut, /* out */ UGUID** out);

    /*
     * Интерфейс:
     *   - result : возвращается in (без AddRef);
     *   - in     : выполняется AddRef/Release для проверки целостности счетчика;
     *   - inOut  : старый указатель освобождается, новый = in (AddRef);
     *   - out    : *out = in (AddRef).
     */
    IEcoUnknown* (ECOCALLMETHOD *TestInterface)(/* in */ IEcoTest1Ptr_t me, /* in */ IEcoUnknown* in, /* inout */ IEcoUnknown** inOut, /* out */ IEcoUnknown** out);

    /*
     * VoidPtr:
     *   result = in;  *inOut = in;  *out = in.
     */
    voidptr_t (ECOCALLMETHOD *TestVoidPtr)(/* in */ IEcoTest1Ptr_t me, /* in */ voidptr_t in, /* inout */ voidptr_t* inOut, /* out */ voidptr_t* out);

    /*
     * VOID-результат (ECO_TYPE_VOID валиден только в позиции результата).
     * Полученное значение сохраняется во внутреннем поле компонента и
     * может быть прочитано через GetLastVoidInput.
     */
    void     (ECOCALLMETHOD *TestVoid)       (/* in */ IEcoTest1Ptr_t me, /* in */ int32_t in);
    int32_t  (ECOCALLMETHOD *GetLastVoidInput)(/* in */ IEcoTest1Ptr_t me);

} IEcoTest1VTbl, *IEcoTest1VTblPtr_t;

interface IEcoTest1 {
    struct IEcoTest1VTbl *pVTbl;
} IEcoTest1;


#endif /* __I_ECOTEST1_H__ */

