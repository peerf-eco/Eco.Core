/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoTypeLib1Builder
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoTypeLib1Builder
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECO_TYPE_LIB_1_BUILDER_H__
#define __I_ECO_TYPE_LIB_1_BUILDER_H__

#include "IEcoBase1.h"
#include "IEcoParamDescriptor1.h"
#include "IEcoMethodDescriptor1.h"
#include "IEcoConstDescriptor1.h"
#include "IEcoInterfaceDescriptor1.h"
#include "IEcoInterfaceDirectoryEntry1.h"
#include "IEcoInterfaceDirectory1.h"

/* Типы данных для TypeDescriptor */
typedef enum EcoTypeLib1TypeTag {
    ECO_TYPE_UNDEFINED = 0,
    ECO_TYPE_INT8,
    ECO_TYPE_UINT8,
    ECO_TYPE_INT16,
    ECO_TYPE_UINT16,
    ECO_TYPE_INT32,
    ECO_TYPE_UINT32,
    ECO_TYPE_FLOAT,
    ECO_TYPE_DOUBLE,
    ECO_TYPE_ASTRING,    /* ASCII String */
    ECO_TYPE_WSTRING,    /* Wide String */
    ECO_TYPE_INTERFACE,  /* Указатель на интерфейс */
    ECO_TYPE_VOIDPTR,    /* void* */
    ECO_TYPE_VOID        /* void (только для Result) */
} EcoTypeLib1TypeTag;

/* Флаги параметров (ParamDescriptor) */
#define ECO_PARAM_IN        0x01  /* Аргумент передается в метод */
#define ECO_PARAM_OUT       0x02  /* Аргумент возвращается из метода */
#define ECO_PARAM_RETVAL    0x04  /* Аргумент является результатом (для скриптов) */
#define ECO_PARAM_SHARED    0x08  /* Разделяемая память (shared) */
#define ECO_PARAM_DIPPER    0x10  /* Специфичный флаг XPCOM для выделения памяти */

/* Флаги методов (MethodDescriptor) */
/* 
 * Метод является "геттером" свойства (например, get_Count). 
 * Bridge в Python может превратить такой метод в @property. 
 */
#define ECO_METHOD_GETTER       0x01

/* 
 * Метод является "сеттером" свойства (например, set_Count). 
 * Позволяет Bridge понимать, что метод ожидает одно значение для записи. 
 */
#define ECO_METHOD_SETTER       0x02

/* 
 * Флаг указывает, что метод не следует правилам именования COM. 
 * Используется для специфических системных вызовов, которые не возвращают result. 
 */
#define ECO_METHOD_NOTCOM       0x04

/* 
 * Метод является конструктором объекта. 
 * Важно для Java2COM, чтобы понимать, как инициализировать прокси-объект. 
 */
#define ECO_METHOD_CONSTRUCTOR  0x08

/* 
 * Метод скрыт для систем автоматизации (скриптов). 
 * Bridge может проигнорировать этот метод при генерации обертки для Python. 
 */
#define ECO_METHOD_HIDDEN       0x10


/* IEcoTypeLib1Builder IID = {8B8B4F5C-12BB-4E44-8C69-4F35D95BABA1} */
#ifndef __IID_IEcoTypeLib1Builder
static const UGUID IID_IEcoTypeLib1Builder = {0x01, 0x10, {0x8B, 0x8B, 0x4F, 0x5C, 0x12, 0xBB, 0x4E, 0x44, 0x8C, 0x69, 0x4F, 0x35, 0xD9, 0x5B, 0xAB, 0xA1}};
#endif /* __IID_IEcoTypeLib1Builder */

typedef struct IEcoTypeLib1Builder* IEcoTypeLib1BuilderPtr_t;

typedef struct IEcoTypeLib1BuilderVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoTypeLib1BuilderPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoTypeLib1BuilderPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoTypeLib1BuilderPtr_t me);

    /* IEcoTypeLib1Builder */

    /* Создание параметра: name, typeTag, flags (ECO_PARAM_IN | ECO_PARAM_OUT ...) */
    int16_t (ECOCALLMETHOD *CreateParameter)(
        /* in */ IEcoTypeLib1BuilderPtr_t me, 
        /* in */ const char* name, 
        /* in */ uint16_t typeTag, 
        /* in */ uint8_t flags,
        /* out */ struct IEcoParamDescriptor1** ppIParam
    );

    /* Создание метода: name, flags (ECO_METHOD_GETTER ...) */
    int16_t (ECOCALLMETHOD *CreateMethod)(
        /* in */ IEcoTypeLib1BuilderPtr_t me, 
        /* in */ const char* name,
        /* in */ uint8_t flags,
        /* out */ struct IEcoMethodDescriptor1** ppIMethod
    );

    /* Создание константы */
    int16_t (ECOCALLMETHOD *CreateConstant)(
        /* in */ IEcoTypeLib1BuilderPtr_t me, 
        /* in */ const char* name,
        /* in */ uint16_t typeTag,
        /* in */ voidptr_t value,
        /* out */ struct IEcoConstDescriptor1** ppIConst
    );

    /* Создание дескриптора интерфейса */
    int16_t (ECOCALLMETHOD *CreateInterfaceDescriptor)(
        /* in */ IEcoTypeLib1BuilderPtr_t me,
        /* in */ uint16_t parentIndex,
        /* out */ struct IEcoInterfaceDescriptor1** ppIDesc
    );

     /* Создание записи в директории (связывание IID и дескриптора) */
    int16_t (ECOCALLMETHOD *CreateInterfaceDirectoryEntry)(
        /* in */ IEcoTypeLib1BuilderPtr_t me,
        /* in */ const char* name,
        /* in */ const char* ns,
        /* in */ const UGUID* iid,
        /* in */ struct IEcoInterfaceDescriptor1* pIDesc,
        /* out */ struct IEcoInterfaceDirectoryEntry1** ppIEntry
    );

    /* Создание итогового объекта директории */
    int16_t (ECOCALLMETHOD *CreateInterfaceDirectory)(
        /* in */ IEcoTypeLib1BuilderPtr_t me,
        /* out */ struct IEcoInterfaceDirectory1** ppIDirectory
    );   

} IEcoTypeLib1BuilderVTbl, *IEcoTypeLib1BuilderVTblPtr_t;

interface IEcoTypeLib1Builder {
    struct IEcoTypeLib1BuilderVTbl *pVTbl;
} IEcoTypeLib1Builder;


#endif /* __I_ECO_TYPE_LIB_1_BUILDER_H__ */

