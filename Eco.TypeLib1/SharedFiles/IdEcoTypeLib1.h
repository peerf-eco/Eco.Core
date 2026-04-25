/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoTypeLib1
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoTypeLib1
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

#ifndef __ID_ECO_TYPE_LIB_1_H__
#define __ID_ECO_TYPE_LIB_1_H__

#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"

/* EcoTypeLib1 CID = {8039E233-E9A3-4D43-BAF7-833001434A0B} */
#ifndef __CID_EcoTypeLib1
static const UGUID CID_EcoTypeLib1 = {0x01, 0x10, {0x80, 0x39, 0xE2, 0x33, 0xE9, 0xA3, 0x4D, 0x43, 0xBA, 0xF7, 0x83, 0x30, 0x01, 0x43, 0x4A, 0x0B}};
#endif /* __CID_EcoTypeLib1 */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B;
#endif

#endif /* __ID_ECO_TYPE_LIB_1_H__ */

