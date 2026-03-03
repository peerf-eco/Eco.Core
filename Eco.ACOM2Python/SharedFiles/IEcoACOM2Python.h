/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IEcoACOM2Python
 * </summary>
 *
 * <description>
 *   This header describes the interface IEcoACOM2Python
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

#ifndef __I_ECO_ACOM_2_PYTHON_H__
#define __I_ECO_ACOM_2_PYTHON_H__

#include "IEcoBase1.h"

/* IEcoACOM2Python IID = {107F4B5C-1FEF-47CC-A7E6-3167ABB9E0A7} */
#ifndef __IID_IEcoACOM2Python
static const UGUID IID_IEcoACOM2Python = {0x01, 0x10, {0x10, 0x7F, 0x4B, 0x5C, 0x1F, 0xEF, 0x47, 0xCC, 0xA7, 0xE6, 0x31, 0x67, 0xAB, 0xB9, 0xE0, 0xA7}};
#endif /* __IID_IEcoACOM2Python */

typedef struct IEcoACOM2Python* IEcoACOM2PythonPtr_t;

typedef struct IEcoACOM2PythonVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoACOM2PythonPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoACOM2PythonPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoACOM2PythonPtr_t me);

    /* IEcoACOM2Python */
    int16_t (ECOCALLMETHOD *RegisterComponent) (/*in*/ IEcoACOM2PythonPtr_t me, /*in*/ char_t* pathname, /*in*/ const UGUID* rcid, /*in*/ IEcoUnknownPtr_t pIFactory);
    int16_t (ECOCALLMETHOD *UnRegisterComponent) (/*in*/ IEcoACOM2PythonPtr_t me, /*in*/ const UGUID* rcid);
    int16_t (ECOCALLMETHOD *QueryComponent) (/*in*/ IEcoACOM2PythonPtr_t me, /*in*/ const UGUID* rcid, /*in*/ IEcoUnknownPtr_t pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ voidptr_t* ppv);

} IEcoACOM2PythonVTbl, *IEcoACOM2PythonVTblPtr_t;

interface IEcoACOM2Python {
    struct IEcoACOM2PythonVTbl *pVTbl;
} IEcoACOM2Python;


#endif /* __I_ECO_ACOM_2_PYTHON_H__ */

