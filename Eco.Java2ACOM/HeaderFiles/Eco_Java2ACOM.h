#include <jni.h>

#ifndef _Included_Eco_Java2ACOM
#define _Included_Eco_Java2ACOM
#include "IEcoBase1.h"

void setPointerToInterface(JNIEnv* env, jobject iUnk, void* pv);
void* getPointerToInterface(JNIEnv* env, jobject iUnk);
UGUID getUGUID(JNIEnv* env, jobject uguidObj);
void getCharArray(JNIEnv* env, jstring str, char_t** result);
#endif
