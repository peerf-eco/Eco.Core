#include "Eco_System_CEcoSystem.h"
#include "Eco_Java2ACOM.h"
#include "CEcoSystem1.h"
#include "IdEcoTypeLib1.h"
#include "IdEcoList1.h"

JNIEXPORT jshort JNICALL Java_Eco_System_CEcoSystem_createCEcoSystem(JNIEnv* env, jclass thisClass, jobjectArray args, jobject iSys) {
    int16_t argc = (*env)->GetArrayLength(env, args);
    char_t** argv = (char_t**)malloc(argc * sizeof(char_t*));
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoTypeLib1* pITypeLib = 0;
    int16_t result = 0;
    int16_t i = 0;

    for (i = 0; i < argc; i++) {
        jstring arg = (jstring)(*env)->GetObjectArrayElement(env, args, i);
        argv[i] = (*env)->GetStringUTFChars(env, arg, 0);
    }
    result = createCEcoSystem1_00000100(argc, argv, &pISys);
    if (result != 0) {
        return result;
    }
    SetPointerToInterface(env, iSys, pISys);

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, &pIBus);
    if (result != 0) {
        return result;
    }
    #ifdef ECO_LIB
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoTypeLib1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_8039E233E9A34D43BAF7833001434A0B);
    if (result != 0 ) {
        return result;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        return result;
    }
    #endif
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, &pITypeLib);
    if (result != 0) {
        return result;
    }
    pIBus->pVTbl->Release(pIBus);
    SetTypeLibPointer(env, iSys, pITypeLib);

    return result;
}

JNIEXPORT void JNICALL Java_Eco_System_CEcoSystem_deleteCEcoSystem(JNIEnv* env, jclass thisClass, jobject iSys) {
    IEcoTypeLib1* pITypeLib = GetTypeLibPointer(env, iSys);
    pITypeLib->pVTbl->Release(pITypeLib);
    deleteCEcoSystem1_00000100((IEcoSystem1*)GetPointerToInterface(env, iSys));
}
