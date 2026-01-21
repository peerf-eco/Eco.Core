#include "Eco_Calculator_IEcoCalculatorY.h"
#include "Eco_Java2ACOM.h"
#include "IEcoCalculatorY.h"

JNIEXPORT jint JNICALL Java_Eco_Calculator_IEcoCalculatorY_Multiplication(JNIEnv* env, jobject thisObj, jint a, jint b) {
    IEcoCalculatorY* pICalcY = (IEcoCalculatorY*)getPointerToInterface(env, thisObj);
    return pICalcY->pVTbl->Multiplication(pICalcY, a, b);
}

JNIEXPORT jint JNICALL Java_Eco_Calculator_IEcoCalculatorY_Division(JNIEnv* env, jobject thisObj, jint a, jint b) {
    IEcoCalculatorY* pICalcY = (IEcoCalculatorY*)getPointerToInterface(env, thisObj);
    return pICalcY->pVTbl->Division(pICalcY, a, b);
}
