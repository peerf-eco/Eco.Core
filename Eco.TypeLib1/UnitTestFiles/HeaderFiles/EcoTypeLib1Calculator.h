/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   EcoTypeLib1Calculator
 * </summary>
 *
 * <description>
 *   Hand-written `.etl` writers for the calculator interfaces
 *   (`IEcoCalculatorX` and `IEcoCalculatorY`).
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __ECO_TYPE_LIB_1_CALCULATOR_H__
#define __ECO_TYPE_LIB_1_CALCULATOR_H__

#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"

/*
 * <summary>
 *   IID_IEcoCalculatorX = {93221116-2248-4742-AE06-82819447843D}
 * </summary>
 */
extern const UGUID IID_IEcoCalculatorX;

/*
 * <summary>
 *   IID_IEcoCalculatorY = {BD6414C2-9096-423E-A90C-04D77AFD1CAD}
 * </summary>
 */
extern const UGUID IID_IEcoCalculatorY;

/*
 * <summary>
 *   SaveTypeLib_IEcoCalculatorX
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoCalculatorX` (Addition,
 *   Subtraction) and persist it as the corresponding `.etl` file.
 * </description>
 */
void SaveTypeLib_IEcoCalculatorX(IEcoTypeLib1* pTypeLib);

/*
 * <summary>
 *   SaveTypeLib_IEcoCalculatorY
 * </summary>
 *
 * <description>
 *   Build the directory describing `IEcoCalculatorY` (Multiplication,
 *   Division) and persist it as the corresponding `.etl` file.
 * </description>
 */
void SaveTypeLib_IEcoCalculatorY(IEcoTypeLib1* pTypeLib);

#endif /* __ECO_TYPE_LIB_1_CALCULATOR_H__ */
