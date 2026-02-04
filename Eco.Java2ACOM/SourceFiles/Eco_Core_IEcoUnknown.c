#include "Eco_Core_IEcoUnknown.h"
#include "Eco_Java2ACOM.h"

JNIEXPORT jobject JNICALL Java_Eco_Core_IEcoUnknown_invokeNative(JNIEnv* env, jobject thisObj, jint VTblIdx, jobjectArray args, jclass returnType) {
    void* pIUnk = 0;
    void* pFunc = 0;
    jsize numArgs = 0;
    char_t* className = 0;
    jsize i = 0;
    ffi_type** ffiArgs;
    void** values;
    ffi_cif cif;
    ffi_type rtype = ffi_type_pointer;
    ffi_arg result;
    ffi_status status;
    jobject retObj;
    jclass clazz;
    jmethodID method;

    pIUnk = getPointerToInterface(env, thisObj);
    pFunc = (*(void***)pIUnk)[VTblIdx];
    numArgs = (*env)->GetArrayLength(env, args);
    ffiArgs = (ffi_type**)malloc((numArgs + 1) * sizeof(ffi_type*));
    values = malloc((numArgs + 1) * sizeof(void*));
    ffiArgs[0] = &ffi_type_pointer;
    values[0] = &pIUnk;
    for (i = 0; i < numArgs; i++) {
        jobject arg = (*env)->GetObjectArrayElement(env, args, i);
        getObjectClassName(env, arg, &className);
        if (strcmp(className, "java.lang.Integer") == 0) {
            int* value = malloc(sizeof(int));
            clazz = (*env)->GetObjectClass(env, arg);
            method = (*env)->GetMethodID(env, clazz, "intValue", "()I");
            *value = (*env)->CallIntMethod(env, arg, method);
            ffiArgs[i + 1] = &ffi_type_sint32;
            values[i + 1] = value;
        } else if (strcmp(className, "java.lang.Long") == 0) {
            long long* value = malloc(sizeof(long long));
            clazz = (*env)->GetObjectClass(env, arg);
            method = (*env)->GetMethodID(env, clazz, "longValue", "()J");
            *value = (*env)->CallLongMethod(env, arg, method);
            ffiArgs[i + 1] = &ffi_type_sint64;
            values[i + 1] = value;
        }
        free(className);
    }
    getClassName(env, returnType, &className);
    if (strcmp(className, "int") == 0) {
        rtype = ffi_type_sint32;
    } else if (strcmp(className, "long") == 0) {
        rtype = ffi_type_sint64;
    } else if (strcmp(className, "void") == 0) {
        rtype = ffi_type_void;
    }

    status = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, numArgs + 1, &rtype, ffiArgs);
    if (status == FFI_OK) {
        ffi_call(&cif, FFI_FN(pFunc), &result, values);
    }

    free(ffiArgs);
    free(values);
    for (i = 0; i < numArgs; i++) {
        free(values[i + 1]);
    }

    if (strcmp(className, "int") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Integer");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(I)V");
        retObj = (*env)->NewObject(env, clazz, method, (jint)result);
    } else if (strcmp(className, "long") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Long");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(J)V");
        retObj = (*env)->NewObject(env, clazz, method, (jlong)result);
    } else if (strcmp(className, "void") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Void");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
    }
    free(className);
    return retObj;
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_QueryInterface(JNIEnv* env, jobject thisObj, jobject riidObj, jobject iUnk) {
    IEcoUnknown* pIUnk = 0;
    int16_t result = 0;
    UGUID riid = {0};
    void* pv = 0;

    pIUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    riid = getUGUID(env, riidObj);
    result = pIUnk->pVTbl->QueryInterface(pIUnk, &riid, &pv);
    if (result != 0) {
        return result;
    }
    setPointerToInterface(env, iUnk, pv);
    return result;
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_AddRef(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* pIEcoUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    return pIEcoUnk->pVTbl->AddRef(pIEcoUnk);
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknown_Release(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* pIEcoUnk = (IEcoUnknown*)getPointerToInterface(env, thisObj);
    return pIEcoUnk->pVTbl->Release(pIEcoUnk);
}
