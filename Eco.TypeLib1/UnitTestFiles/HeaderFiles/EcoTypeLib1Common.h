/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoTypeLib1Common
 * </summary>
 *
 * <description>
 *   Helpers shared by every interface-specific `.etl`.
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __ECO_TYPE_LIB_1_COMMON_H__
#define __ECO_TYPE_LIB_1_COMMON_H__

#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"

/*
 * <summary>
 *   ECO_TYPE_NAME table
 * </summary>
 *
 * <description>
 *   Printable names for every `EcoTypeLib1TypeTag` value, indexed by the
 *   tag itself.
 * </description>
 */
extern const char_t* ECO_TYPE_NAME[];

/*
 * <summary>
 *   UGUIDPtrToTypeLibFileName
 * </summary>
 *
 * <description>
 *   Build a `<HEX>.etl` file path from the bytes of a UGUID.
 * </description>
 */
char_t* UGUIDPtrToTypeLibFilePath(const UGUID* uguid);

/*
 * <summary>
 *   LoadAndInspect
 * </summary>
 *
 * <description>
 *   Load the `.etl` for `riid` through `pTypeLib`, find the matching
 *   directory entry and pretty-print its methods and parameters to
 *   stdout.
 * </description>
 */
void LoadAndInspect(IEcoTypeLib1* pTypeLib, const UGUID* riid);

#endif /* __ECO_TYPE_LIB_1_COMMON_H__ */
