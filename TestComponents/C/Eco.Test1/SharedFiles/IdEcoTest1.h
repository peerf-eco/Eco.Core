
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IdEcoTest1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IdEcoTest1
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

#ifndef __ID_ECOTEST1_H__
#define __ID_ECOTEST1_H__

#include "IEcoBase1.h"
#include "IEcoTest1.h"

/* EcoTest1 CID = {602F6169-A6FB-4B14-9AB6-88ED816BDCCA} */
#ifndef __CID_EcoTest1
static const UGUID CID_EcoTest1 = {0x01, 0x10, {0x60, 0x2F, 0x61, 0x69, 0xA6, 0xFB, 0x4B, 0x14, 0x9A, 0xB6, 0x88, 0xED, 0x81, 0x6B, 0xDC, 0xCA}};
#endif /* __CID_EcoTest1 */

/* Фабрика компонента для динамической и статической компановки */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_602F6169A6FB4B149AB688ED816BDCCA;
#endif

#endif /* __ID_ECOTEST1_H__ */

