#include "Eco_InterfaceBus_IEcoInterfaceBus.h"
#include "Eco_Java2ACOM.h"
#include "IEcoInterfaceBus1.h"

JNIEXPORT jint JNICALL Java_Eco_InterfaceBus_IEcoInterfaceBus_Init(JNIEnv* env, jobject thisObj) {
    IEcoInterfaceBus1* pIBus = (IEcoInterfaceBus1*)getPointerToInterface(env, thisObj);
    return pIBus->pVTbl->Init(pIBus);
}

JNIEXPORT jint JNICALL Java_Eco_InterfaceBus_IEcoInterfaceBus_InitWith(JNIEnv* env, jobject thisObj, jlong heapStartAddress, jlong size) {
    IEcoInterfaceBus1* pIBus = (IEcoInterfaceBus1*)getPointerToInterface(env, thisObj);
    return pIBus->pVTbl->InitWith(pIBus, (void*)heapStartAddress, size);
}

JNIEXPORT jint JNICALL Java_Eco_InterfaceBus_IEcoInterfaceBus_RegisterComponent(JNIEnv* env, jobject thisObj, jobject rcidObj, jobject iFactory) {
    IEcoInterfaceBus1* pIBus = (IEcoInterfaceBus1*)getPointerToInterface(env, thisObj);
    IEcoUnknown* pIFactory = (IEcoUnknown*)getPointerToInterface(env, iFactory);
    UGUID rcid = getUGUID(env, rcidObj);
    return pIBus->pVTbl->RegisterComponent(pIBus, &rcid, pIFactory);
}

JNIEXPORT jint JNICALL Java_Eco_InterfaceBus_IEcoInterfaceBus_UnRegisterComponent(JNIEnv* env, jobject thisObj, jobject rcidObj) {
    IEcoInterfaceBus1* pIBus = (IEcoInterfaceBus1*)getPointerToInterface(env, thisObj);
    UGUID rcid = getUGUID(env, rcidObj);
    return pIBus->pVTbl->UnRegisterComponent(pIBus, &rcid);
}

JNIEXPORT jint JNICALL Java_Eco_InterfaceBus_IEcoInterfaceBus_QueryComponent(JNIEnv* env, jobject thisObj, jobject rcidObj, jobject iUnkOuter, jobject riidObj, jobject iUnk) {
    IEcoInterfaceBus1* pIBus = (IEcoInterfaceBus1*)getPointerToInterface(env, thisObj);
    IEcoUnknown* pIUnkOuter = (IEcoUnknown*)getPointerToInterface(env, iUnkOuter);
    UGUID rcid = getUGUID(env, rcidObj);
    UGUID riid = getUGUID(env, riidObj);
    int16_t result = 0;
    void* pv = 0;

    result = pIBus->pVTbl->QueryComponent(pIBus, &rcid, pIUnkOuter, &riid, &pv);
    if (result != 0) {
        return result;
    }
    setPointerToInterface(env, iUnk, pv);
    return result;
}
