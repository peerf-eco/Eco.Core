/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoTypeLib1_01434A0BFactory
 * </summary>
 *
 * <description>
 *   This header describes the implementation of the factory for the component
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOTYPELIB1_FACTORY_H__
#define __C_ECOTYPELIB1_FACTORY_H__

#include "IEcoSystem1.h"

typedef struct CEcoTypeLib1_01434A0BFactory {

    /* IEcoComponentFactory interface function table */
    IEcoComponentFactoryVTbl* m_pVTblICF;

    /* Reference counter */
    uint32_t m_cRef;

    /* Component data for the factory */
    char_t m_Name[64];
    char_t m_Version[16];
    char_t m_Manufacturer[64];

} CEcoTypeLib1_01434A0BFactory;

#endif /* __C_ECOTYPELIB1_FACTORY_H__ */
