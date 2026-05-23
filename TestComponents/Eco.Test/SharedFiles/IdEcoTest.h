/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IdEcoTest
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IdEcoTest
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __ID_ECO_TEST_H__
#define __ID_ECO_TEST_H__

#include "IEcoBase1.h"
#include "IEcoTest.h"

/* EcoTest CID = {12345678-90AB-CDEF-1234567890ABCDEF} */
#ifndef __CID_EcoTest
static const UGUID CID_EcoTest = {0x01, 0x10, {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF}};
#endif /* __CID_EcoTest */

/* Фабрика компонента для динамической и статической компановки */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory *ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory *GetIEcoComponentFactoryPtr_1234567890ABCDEF1234567890ABCDEF;
#endif

#endif /* __ID_ECO_TEST_H__ */
