#include "Eco_System_CEcoSystem.h"
#include "Eco_Java2ACOM.h"
#include "CEcoSystem1.h"
#include "IdEcoTypeLib1.h"
#include "IdEcoList1.h"

JNIEXPORT jshort JNICALL Java_Eco_System_CEcoSystem_createCEcoSystem(JNIEnv* env, jclass thisClass, jobjectArray args, jobject iSys) {
    int16_t argc = (*env)->GetArrayLength(env, args);
    char_t** argv = (char_t**)malloc(argc * sizeof(char_t*));
    jstring* jArgv = (jstring*)malloc(argc * sizeof(jstring));
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    UGUID* rcid = &CID_EcoMemoryManager1;
    int16_t result = 0;
    int16_t i = 0;

    for (i = 0; i < argc; i++) {
        jArgv[i] = (jstring)(*env)->GetObjectArrayElement(env, args, i);
        argv[i] = (*env)->GetStringUTFChars(env, jArgv[i], 0);
    }
    result = createCEcoSystem1_00000100(argc, argv, &pISys);
    for (i = 0; i < argc; i++) {
        (*env)->ReleaseStringUTFChars(env, jArgv[i], argv[i]);
    }
    free(argv);
    free(jArgv);
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
        pIBus->pVTbl->Release(pIBus);
        return result;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoList1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_53884AFC93C448ECAA929C8D3A562281);
    if (result != 0 ) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }
    #endif

    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &g_pIMem);
    if (result != 0 || g_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, &g_pITypeLib);
    if (result != 0) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, &g_pIDescCacheList);
    pIBus->pVTbl->Release(pIBus);
    return result;
}

JNIEXPORT void JNICALL Java_Eco_System_CEcoSystem_deleteCEcoSystem(JNIEnv* env, jclass thisClass, jobject iSys) {
    uint32_t count = g_pIDescCacheList->pVTbl->Count(g_pIDescCacheList);
    uint32_t index = 0;

    for (index = 0; index < count; index++) {
        EcoDescCacheEntry* pDescCacheEntry = g_pIDescCacheList->pVTbl->Item(g_pIDescCacheList, index);
        pDescCacheEntry->pIDirectory->pVTbl->Release(pDescCacheEntry->pIDirectory);
        g_pIMem->pVTbl->Free(g_pIMem, pDescCacheEntry);
    }

    g_pIDescCacheList->pVTbl->Clear(g_pIDescCacheList);
    g_pIDescCacheList->pVTbl->Release(g_pIDescCacheList);
    g_pITypeLib->pVTbl->Release(g_pITypeLib);
    g_pIMem->pVTbl->Release(g_pIMem);
    deleteCEcoSystem1_00000100((IEcoSystem1*)GetPointerToInterface(env, iSys));
}
