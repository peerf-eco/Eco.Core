/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   IdEcoACOM2Java
 * </summary>
 *
 * <description>
 *   This header describes the interface IdEcoACOM2Java
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

#ifndef __ID_ECOACOM2JAVA_H__
#define __ID_ECOACOM2JAVA_H__

#include "IEcoBase1.h"
#include "IEcoACOM2Java.h"

/* EcoACOM2Java CID = {C8035C97-41CB-40F9-B104-A12C3F41E2AA} */
#ifndef __CID_EcoACOM2Java
static const UGUID CID_EcoACOM2Java = {0x01, 0x10, {0xC8, 0x03, 0x5C, 0x97, 0x41, 0xCB, 0x40, 0xF9, 0xB1, 0x04, 0xA1, 0x2C, 0x3F, 0x41, 0xE2, 0xAA}};
#endif /* __CID_EcoACOM2Java */

/* Component factory for dynamic and static layout */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_C8035C9741CB40F9B104A12C3F41E2AA;
#endif

#endif /* __ID_ECOACOM2JAVA_H__ */

