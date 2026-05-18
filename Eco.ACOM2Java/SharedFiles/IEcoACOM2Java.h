/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoACOM2Java
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoACOM2Java
 * </description>
 *
 * <reference>
 *
 * </reference>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __I_ECO_ACOM_2_JAVA_H__
#define __I_ECO_ACOM_2_JAVA_H__

#include "IEcoBase1.h"

/* IEcoACOM2Java IID = {ED2D1283-E263-48DC-B9A8-89E10C8C3657} */
#ifndef __IID_IEcoACOM2Java
static const UGUID IID_IEcoACOM2Java = {0x01, 0x10, {0xED, 0x2D, 0x12, 0x83, 0xE2, 0x63, 0x48, 0xDC, 0xB9, 0xA8, 0x89, 0xE1, 0x0C, 0x8C, 0x36, 0x57}};
#endif /* __IID_IEcoACOM2Java */

typedef struct IEcoACOM2Java* IEcoACOM2JavaPtr_t;

typedef struct IEcoACOM2JavaVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoACOM2JavaPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoACOM2JavaPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoACOM2JavaPtr_t me);

    /* IEcoACOM2Java */
    int16_t (ECOCALLMETHOD *CreateJavaVM) (/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ char_t* classpath, /*in*/ uint64_t minHeapSize, /*in*/ uint64_t maxHeapSize);
    int16_t (ECOCALLMETHOD *RegisterComponent) (/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ char_t* classpath, /*in*/ char_t* classname, /*in*/ const UGUID* rcid);
    int16_t (ECOCALLMETHOD *UnRegisterComponent) (/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ const UGUID* rcid);
    int16_t (ECOCALLMETHOD *QueryComponent) (/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ const UGUID* rcid, /*in*/ IEcoUnknownPtr_t pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ voidptr_t* ppv);

} IEcoACOM2JavaVTbl, *IEcoACOM2JavaVTblPtr_t;

interface IEcoACOM2Java {
    struct IEcoACOM2JavaVTbl *pVTbl;
} IEcoACOM2Java;


#endif /* __I_ECO_ACOM_2_JAVA_H__ */

