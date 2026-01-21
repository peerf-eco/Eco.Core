#include "Eco_Core_IEcoUnknown.h"
#include "Eco_Java2ACOM.h"

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_QueryInterface(JNIEnv* env, jobject thisObj, jobject riidObj, jobject iUnk) {
    IEcoUnknown* pIUnk = 0;
    int16_t result = 0;
    UGUID riid = {0};
    void* pv = 0;

    pIUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    riid = getUGUID(env, riidObj);
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &riid, &pv);
    if (result != 0) {
        return result;
    }
    setPointerToInterface(env, iUnk, pv);
    return result;
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_AddRef(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* pIEcoUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    return pIEcoUnk->pVTbl->AddRef(pIEcoUnk);
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_Release(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* pIEcoUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    return pIEcoUnk->pVTbl->Release(pIEcoUnk);
}
