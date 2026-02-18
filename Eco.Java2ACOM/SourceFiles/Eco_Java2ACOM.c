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

void getUGUID(JNIEnv* env, jobject uguidObj, UGUID* result) {
    jclass clazz = (*env)->GetObjectClass(env, uguidObj);
    jfieldID field = 0;
    jobject obj = 0;
    jbyte* bytes;
    uint16_t i = 0;
    field = (*env)->GetFieldID(env, clazz, "Preamble", "B");
    result->Preamble = (*env)->GetByteField(env, uguidObj, field);
    field = (*env)->GetFieldID(env, clazz, "Length", "B");
    result->Length = (*env)->GetByteField(env, uguidObj, field);
    field = (*env)->GetFieldID(env, clazz, "Data", "[B");
    obj = (*env)->GetObjectField(env, uguidObj, field);
    bytes = (*env)->GetByteArrayElements(env, obj, 0);
    for (i = 0; i < result->Length; i++) {
        result->Data[i] = *(bytes + i);
    }
    (*env)->ReleaseByteArrayElements(env, obj, bytes, JNI_ABORT);
}

void setUGUID(JNIEnv* env, jobject uguidObj, UGUID* uguid) {
    jclass clazz = (*env)->GetObjectClass(env, uguidObj);
    jfieldID field = 0;
    jobject obj = 0;
    uint16_t i = 0;
    field = (*env)->GetFieldID(env, clazz, "Preamble", "B");
    (*env)->SetByteField(env, uguidObj, field, uguid->Preamble);
    field = (*env)->GetFieldID(env, clazz, "Length", "B");
    (*env)->SetByteField(env, uguidObj, field, uguid->Length);
    field = (*env)->GetFieldID(env, clazz, "Data", "[B");
    obj = (*env)->GetObjectField(env, uguidObj, field);
    for (i = 0; i < uguid->Length; i++) {
        (*env)->SetObjectArrayElement(env, obj, i, uguid->Data[i]);
    }
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

void getObjectClassName(JNIEnv* env, jobject obj, char_t** result) {
    jclass clazz = (*env)->GetObjectClass(env, obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "getClass", "()Ljava/lang/Class;");
    jobject classObj = (*env)->CallObjectMethod(env, obj, method);
    jstring str;
    clazz = (*env)->GetObjectClass(env, classObj);
    method = (*env)->GetMethodID(env, clazz, "getName", "()Ljava/lang/String;");
    str = (jstring)(*env)->CallObjectMethod(env, classObj, method);
    getCharArray(env, str, result);
}

void getClassName(JNIEnv* env, jclass classObj, char_t** result) {
    jclass clazz = (*env)->FindClass(env, "java/lang/Class");
    jmethodID method = (*env)->GetMethodID(env, clazz, "getName", "()Ljava/lang/String;");
    jstring str = (jstring)(*env)->CallObjectMethod(env, classObj, method);
    getCharArray(env, str, result);
}
