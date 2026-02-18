#include "Eco_Core_IEcoUnknown_Client.h"
#include "Eco_Java2ACOM.h"

JNIEXPORT jobject JNICALL Java_Eco_Core_IEcoUnknown_1Client_invokeNative(JNIEnv* env, jobject thisObj, jint VTblIdx, jobjectArray args, jclass returnType) {
    void* pIUnk = 0;
    void* pFunc = 0;
    jsize numArgs = 0;
    char_t* className = 0;
    jsize i = 0;
    ffi_type** ffiArgs;
    void** values;
    bool_t* pValuesAlloc;
    bool_t* ppValuesAlloc;
    bool_t* outValues;
    ffi_cif cif;
    ffi_type rtype = ffi_type_pointer;
    ffi_arg result;
    ffi_status status;
    jobject retObj;
    jclass clazz;
    jmethodID method;
    jfieldID field;

    pIUnk = getPointerToInterface(env, thisObj);
    pFunc = (*(void***)pIUnk)[VTblIdx];
    numArgs = (*env)->GetArrayLength(env, args);
    ffiArgs = (ffi_type**)malloc((numArgs + 1) * sizeof(ffi_type*));
    values = malloc((numArgs + 1) * sizeof(void*));
    pValuesAlloc = calloc(numArgs + 1, sizeof(bool_t));
    ppValuesAlloc = calloc(numArgs + 1, sizeof(bool_t));
    outValues = calloc(numArgs + 1, sizeof(bool_t));
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
            pValuesAlloc[i + 1] = 1;
        } else if (strcmp(className, "java.lang.Long") == 0) {
            long long* value = malloc(sizeof(long long));
            clazz = (*env)->GetObjectClass(env, arg);
            method = (*env)->GetMethodID(env, clazz, "longValue", "()J");
            *value = (*env)->CallLongMethod(env, arg, method);
            ffiArgs[i + 1] = &ffi_type_sint64;
            values[i + 1] = value;
            pValuesAlloc[i + 1] = 1;
        } else if (strcmp(className, "java.lang.String") == 0) {
            char** value = malloc(sizeof(char*));
            getCharArray(env, arg, value);
            ffiArgs[i + 1] = &ffi_type_pointer;
            values[i + 1] = value;
            pValuesAlloc[i + 1] = 1;
            ppValuesAlloc[i + 1] = 1;
        } else if (strcmp(className, "Eco.Core.UGUID") == 0) {
            UGUID** value = malloc(sizeof(UGUID*));
            *value = malloc(sizeof(UGUID));
            getUGUID(env, arg, *value);
            ffiArgs[i + 1] = &ffi_type_pointer;
            values[i + 1] = value;
            pValuesAlloc[i + 1] = 1;
            ppValuesAlloc[i + 1] = 1;
        } else if (strcmp(className, "Eco.Core.OutPointer") == 0) {
            void*** value = malloc(sizeof(void**));
            *value = malloc(sizeof(void*));
            **value = 0;
            ffiArgs[i + 1] = &ffi_type_pointer;
            values[i + 1] = value;
            pValuesAlloc[i + 1] = 1;
            ppValuesAlloc[i + 1] = 1;
            outValues[i + 1] = 1;
        } else {
            void** value = malloc(sizeof(void*));
            *value = getPointerToInterface(env, arg);
            ffiArgs[i + 1] = &ffi_type_pointer;
            values[i + 1] = value;
            pValuesAlloc[i + 1] = 1;
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
    for (i = 0; i < numArgs; i++) {
        if (outValues[i + 1]) {
            jobject arg = (*env)->GetObjectArrayElement(env, args, i);
            clazz = (*env)->GetObjectClass(env, arg);
            field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
            arg = (*env)->GetObjectField(env, arg, field);
            setPointerToInterface(env, arg, *(void**)*(void***)(values[i + 1]));
        }
        if (ppValuesAlloc[i + 1]) {
            free(*(void**)(values[i + 1]));
        }
        if (pValuesAlloc[i + 1]) {
            free(values[i + 1]);
        }
    }
    free(values);
    free(pValuesAlloc);
    free(outValues);

    if (strcmp(className, "int") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Integer");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(I)V");
        retObj = (*env)->NewObject(env, clazz, method, (jint)result);
    } else if (strcmp(className, "long") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Long");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(J)V");
        retObj = (*env)->NewObject(env, clazz, method, (jlong)result);
    } else if (strcmp(className, "bool") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Boolean");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(Z)V");
        retObj = (*env)->NewObject(env, clazz, method, (jboolean)result);
    } else if (strcmp(className, "void") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Void");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
    } else if (strcmp(className, "java.lang.String")) {
        retObj = (*env)->NewStringUTF(env, (char*)result);
    } else if (strcmp(className, "Eco.Core.UGUID") == 0) {
        UGUID* uguid = malloc(sizeof(UGUID));
        getUGUID(env, retObj, uguid);
        clazz = (*env)->FindClass(env, "Eco/Core/UGUID");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
        setUGUID(env, retObj, uguid);
        free(uguid);
    } else {
        clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknown_Client");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
        setPointerToInterface(env, retObj, (void*)result);
    }
    free(className);
    return retObj;
}
