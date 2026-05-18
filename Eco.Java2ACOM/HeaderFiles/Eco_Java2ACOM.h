#ifndef _Included_Eco_Java2ACOM
#define _Included_Eco_Java2ACOM
#include <jni.h>
#include <ffi.h>
#include "IEcoBase1.h"
#include "IEcoEnumConnections.h"
#include "IEcoTypeLib1.h"

void SetTypeLibPointer(JNIEnv* env, jobject iUnk, IEcoTypeLib1* pITypeLib);
IEcoTypeLib1* GetTypeLibPointer(JNIEnv* env, jobject iUnk);
void SetPointerToInterface(JNIEnv* env, jobject iUnk, void* pv);
void* GetPointerToInterface(JNIEnv* env, jobject iUnk);
#endif
