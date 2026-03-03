/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoACOM2Python
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoACOM2Python
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

#ifndef __ID_ECOACOM2PYTHON_H__
#define __ID_ECOACOM2PYTHON_H__

#include "IEcoBase1.h"
#include "IEcoACOM2Python.h"

/* EcoACOM2Python CID = {219EDB62-6EF1-4B42-BE16-F93A566F1CC3} */
#ifndef __CID_EcoACOM2Python
static const UGUID CID_EcoACOM2Python = {0x01, 0x10, {0x21, 0x9E, 0xDB, 0x62, 0x6E, 0xF1, 0x4B, 0x42, 0xBE, 0x16, 0xF9, 0x3A, 0x56, 0x6F, 0x1C, 0xC3}};
#endif /* __CID_EcoACOM2Python */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_219EDB626EF14B42BE16F93A566F1CC3;
#endif

#endif /* __ID_ECOACOM2PYTHON_H__ */

