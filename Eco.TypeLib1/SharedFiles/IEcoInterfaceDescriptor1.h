/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoInterfaceDescriptor1
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoInterfaceDescriptor1
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

#ifndef __I_ECO_INTERFACE_DESCRIPTOR_1_H__
#define __I_ECO_INTERFACE_DESCRIPTOR_1_H__

#include "IEcoBase1.h"
#include "IEcoMethodDescriptor1.h"
#include "IEcoConstDescriptor1.h"


/* Флаги интерфейса (InterfaceDescriptor) - упакованы в 1 байт */

/* Интерфейс доступен для скриптовых языков (Python, JS) */
#define ECO_INTERFACE_FLAG_SCRIPTABLE    0x01

/* Интерфейс является "функциональным" (содержит только один основной метод) */
#define ECO_INTERFACE_FLAG_FUNCTION      0x02

/* IEcoInterfaceDescriptor1 IID = {5D9AE041-C0AB-482C-935F-A35F0F07B0F1} */
#ifndef __IID_IEcoInterfaceDescriptor1
static const UGUID IID_IEcoInterfaceDescriptor1 = {0x01, 0x10, {0x5D, 0x9A, 0xE0, 0x41, 0xC0, 0xAB, 0x48, 0x2C, 0x93, 0x5F, 0xA3, 0x5F, 0x0F, 0x07, 0xB0, 0xF1}};
#endif /* __IID_IEcoInterfaceDescriptor1 */

typedef struct IEcoInterfaceDescriptor1* IEcoInterfaceDescriptor1Ptr_t;

typedef struct IEcoInterfaceDescriptor1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);

    /* IEcoInterfaceDescriptor1 */
 
    /* Установка свойств дескриптора */
    /* Установка индекса родительского интерфейса */
    int16_t (ECOCALLMETHOD *set_ParentIndex)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t parentIndex);
    uint16_t (ECOCALLMETHOD *get_ParentIndex)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);

    /* Установка флагов: is_scriptable, is_function */
    int16_t (ECOCALLMETHOD *set_Flags)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint8_t flags);
    uint8_t (ECOCALLMETHOD *get_Flags)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);

    /* IEcoInterfaceDescriptor1: Наполнение методами */
    
    /* Добавление готового дескриптора метода */
    int16_t (ECOCALLMETHOD *AddMethod)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ IEcoMethodDescriptor1* pIMethod);
    
    uint16_t (ECOCALLMETHOD *get_MethodCount)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);
    int16_t (ECOCALLMETHOD *get_MethodAtIndex)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t index, /* out */ IEcoMethodDescriptor1** ppIMethod);

    /* IEcoInterfaceDescriptor1: Наполнение константами */
    
    /* Добавление готового дескриптора константы */
    int16_t (ECOCALLMETHOD *AddConstant)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ IEcoConstDescriptor1* pIConst);
    
    uint16_t (ECOCALLMETHOD *get_ConstantCount)(/* in */ IEcoInterfaceDescriptor1Ptr_t me);
    int16_t (ECOCALLMETHOD *get_ConstantAtIndex)(/* in */ IEcoInterfaceDescriptor1Ptr_t me, /* in */ uint16_t index, /* out */ IEcoConstDescriptor1** ppIConst);

} IEcoInterfaceDescriptor1VTbl, *IEcoInterfaceDescriptor1VTblPtr_t;

interface IEcoInterfaceDescriptor1 {
    struct IEcoInterfaceDescriptor1VTbl *pVTbl;
} IEcoInterfaceDescriptor1;


#endif /* __I_ECO_INTERFACE_DESCRIPTOR_1_H__ */

