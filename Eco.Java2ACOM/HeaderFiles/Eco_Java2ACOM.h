#ifndef _Included_Eco_Java2ACOM
#define _Included_Eco_Java2ACOM
#include <jni.h>
#include <ffi.h>
#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"
#include "IEcoList1.h"

IEcoTypeLib1* g_pITypeLib;
IEcoList1* g_pIDescCacheList;

typedef struct EcoDescCacheEntry {
    UGUID riid;
    IEcoInterfaceDirectory1* pIDirectory;
    IEcoInterfaceDescriptor1* pIDesc;
} EcoDescCacheEntry;

void SetPointerToInterface(JNIEnv* env, jobject iUnk, void* pv);
void* GetPointerToInterface(JNIEnv* env, jobject iUnk);
#endif
