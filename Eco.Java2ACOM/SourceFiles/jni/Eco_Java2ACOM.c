#include "Eco_Java2ACOM.h"

void SetTypeLibPointer(JNIEnv* env, jobject iUnk, IEcoTypeLib1* pITypeLib) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetStaticFieldID(env, clazz, "pITypeLib", "J");
    (*env)->SetStaticLongField(env, iUnk, field, (jlong)pITypeLib);
}

IEcoTypeLib1* GetTypeLibPointer(JNIEnv* env, jobject iUnk) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetStaticFieldID(env, clazz, "pITypeLib", "J");
    return (IEcoTypeLib1*) (*env)->GetStaticLongField(env, iUnk, field);
}

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
