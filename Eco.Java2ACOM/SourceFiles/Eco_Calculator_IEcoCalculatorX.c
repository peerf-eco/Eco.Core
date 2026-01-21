#include "Eco_Calculator_IEcoCalculatorX.h"
#include "Eco_Java2ACOM.h"
#include "IEcoCalculatorX.h"

JNIEXPORT jint JNICALL Java_Eco_Calculator_IEcoCalculatorX_Addition(JNIEnv* env, jobject thisObj, jint a, jint b) {
    IEcoCalculatorX* pICalcX = (IEcoCalculatorX*)getPointerToInterface(env, thisObj);
    return pICalcX->pVTbl->Addition(pICalcX, a, b);
}

JNIEXPORT jint JNICALL Java_Eco_Calculator_IEcoCalculatorX_Subtraction(JNIEnv* env, jobject thisObj, jint a, jint b) {
    IEcoCalculatorX* pICalcX = (IEcoCalculatorX*)getPointerToInterface(env, thisObj);
    return pICalcX->pVTbl->Subtraction(pICalcX, a, b);
}
