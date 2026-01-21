#include "Eco_Java2ACOM.h"

void setPointerToInterface(JNIEnv* env, jobject iUnk, void* pv) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "J");
    (*env)->SetLongField(env, iUnk, field, (jlong)pv);
}

void* getPointerToInterface(JNIEnv* env, jobject iUnk) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pInstance", "J");
    return (void*)(*env)->GetLongField(env, iUnk, field);
}

UGUID getUGUID(JNIEnv* env, jobject uguidObj) {
    jclass clazz = (*env)->GetObjectClass(env, uguidObj);
    jfieldID field = 0;
    jobject obj = 0;
    jbyte* bytes;
    uint16_t i = 0;
    UGUID result = {0};
    field = (*env)->GetFieldID(env, clazz, "Preamble", "B");
    result.Preamble = (*env)->GetByteField(env, uguidObj, field);
    field = (*env)->GetFieldID(env, clazz, "Length", "B");
    result.Length = (*env)->GetByteField(env, uguidObj, field);
    field = (*env)->GetFieldID(env, clazz, "Data", "[B");
    obj = (*env)->GetObjectField(env, uguidObj, field);
    bytes = (*env)->GetByteArrayElements(env, obj, 0);
    for (i = 0; i < result.Length; i++) {
        result.Data[i] = *(bytes + i);
    }
    (*env)->ReleaseByteArrayElements(env, obj, bytes, JNI_ABORT);
    return result;
}

void getCharArray(JNIEnv* env, jstring str, char_t** result) {
    jsize size = (*env)->GetStringLength(env, str);
    const jchar* chars = (*env)->GetStringChars(env, str, 0);
    jsize i = 0;
    *result = (char_t*)calloc((size + 1), sizeof(char_t));
    for (i = 0; i < size; i++) {
        (*result)[i] = (char_t)chars[i];
    }
    (*env)->ReleaseStringChars(env, str, chars);
}
