#include "Eco_Java2ACOM.h"

void SetPointerToInterface(JNIEnv* env, jobject iUnk, void* pv) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "I");
    if (field == NULL) return;
    (*env)->SetIntField(env, iUnk, field, (jint)pv);
}

void* GetPointerToInterface(JNIEnv* env, jobject iUnk) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "I");
    if (field == NULL) return NULL;
    return (void*) (*env)->GetIntField(env, iUnk, field);
}
