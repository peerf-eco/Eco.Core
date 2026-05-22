#include "Eco_Java2ACOM.h"

void SetPointerToInterface(JNIEnv* env, jobject iUnk, void* pv) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "J");
    return (*env)->SetLongField(env, iUnk, field, (jlong)pv);
}

void* GetPointerToInterface(JNIEnv* env, jobject iUnk) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "J");
    return (void*) (*env)->GetLongField(env, iUnk, field);
}
