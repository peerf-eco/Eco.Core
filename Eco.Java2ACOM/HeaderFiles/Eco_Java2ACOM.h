#include <jni.h>
#include <ffi.h>

#ifndef _Included_Eco_Java2ACOM
#define _Included_Eco_Java2ACOM
#include "IEcoBase1.h"

void setPointerToInterface(JNIEnv* env, jobject iUnk, void* pv);
void* getPointerToInterface(JNIEnv* env, jobject iUnk);
UGUID getUGUID(JNIEnv* env, jobject uguidObj);
void getCharArray(JNIEnv* env, jstring str, char_t** result);
void getObjectClassName(JNIEnv* env, jobject obj, char_t** result);
void getClassName(JNIEnv* env, jclass classObj, char_t** result);
#endif
