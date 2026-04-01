#include "Eco_Core_IEcoUnknownNative.h"
#include "Eco_Java2ACOM.h"

typedef struct ffiData {
    bool_t pAlloc;
    bool_t ppAlloc;
    bool_t out;
} ffiData;

void getArgument(JNIEnv* env, jobject arg, const char_t* className, ffi_type** ffiType, void** argValue, ffiData* ffiArgData) {
    jclass clazz;
    jmethodID method;
    jfieldID field;

    if (strcmp(className, "java.lang.Byte") == 0) {
        byte_t* value = malloc(sizeof(byte_t));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "byteValue", "()B");
        *value = (*env)->CallByteMethod(env, arg, method);
        *ffiType = &ffi_type_sint8;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Short") == 0) {
        short* value = malloc(sizeof(short));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "shortValue", "()S");
        *value = (*env)->CallShortMethod(env, arg, method);
        *ffiType = &ffi_type_sint16;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Integer") == 0) {
        int* value = malloc(sizeof(int));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "intValue", "()I");
        *value = (*env)->CallIntMethod(env, arg, method);
        *ffiType = &ffi_type_sint32;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Long") == 0) {
        long long* value = malloc(sizeof(long long));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "longValue", "()J");
        *value = (*env)->CallLongMethod(env, arg, method);
        *ffiType = &ffi_type_sint64;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Boolean") == 0) {
        bool_t* value = malloc(sizeof(bool_t));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "booleanValue", "()Z");
        *value = (*env)->CallBooleanMethod(env, arg, method);
        *ffiType = &ffi_type_sint8;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Character") == 0) {
        bool_t* value = malloc(sizeof(bool_t));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "charValue", "()C");
        *value = (*env)->CallCharMethod(env, arg, method);
        *ffiType = &ffi_type_sint8;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Float") == 0) {
        float* value = malloc(sizeof(float));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "floatValue", "()F");
        *value = (*env)->CallFloatMethod(env, arg, method);
        *ffiType = &ffi_type_float;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.Double") == 0) {
        double* value = malloc(sizeof(double));
        clazz = (*env)->GetObjectClass(env, arg);
        method = (*env)->GetMethodID(env, clazz, "doubleValue", "()D");
        *value = (*env)->CallDoubleMethod(env, arg, method);
        *ffiType = &ffi_type_double;
        *argValue = value;
        ffiArgData->pAlloc = 1;
    } else if (strcmp(className, "java.lang.String") == 0) {
        char** value = malloc(sizeof(char*));
        getCharArray(env, arg, value);
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
    } else if (strcmp(className, "Eco.Core.UGUID") == 0) {
        UGUID** value = malloc(sizeof(UGUID*));
        *value = malloc(sizeof(UGUID));
        getUGUID(env, arg, *value);
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
    } else if (strcmp(className, "Eco.Core.IntegerPtr") == 0) {
        int** value = (int**)malloc(sizeof(int*));
        *value = (int*)malloc(sizeof(int));
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
        ffiArgData->out = 1;
    } else if (strcmp(className, "Eco.Core.StringPtr") == 0) {
        char_t*** value = (char_t***)malloc(sizeof(char_t**));
        *value = (char_t**)malloc(sizeof(char_t*));
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
        ffiArgData->out = 1;
    } else if (strcmp(className, "Eco.Core.IEcoUnknownPtr") == 0) {
        void*** value = malloc(sizeof(void**));
        *value = malloc(sizeof(void*));
        **value = 0;
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
        ffiArgData->out = 1;
    } else if (strcmp(className, "Eco.Core.EcoConnectionData") == 0) {
        EcoConnectionData** value = (EcoConnectionData**)malloc(sizeof(EcoConnectionData*));
        *value = (EcoConnectionData*)malloc(sizeof(EcoConnectionData));
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->pAlloc = 1;
        ffiArgData->ppAlloc = 1;
        ffiArgData->out = 1;
    } else {
        void** value = malloc(sizeof(void*));
        *value = getPointerToInterface(env, arg);
        *ffiType = &ffi_type_pointer;
        *argValue = value;
        ffiArgData->ppAlloc = 1;
    }
}

void setOutArg(JNIEnv* env, jobject arg, const char_t* className, void** argValue) {
    jclass clazz;
    jfieldID field;

    clazz = (*env)->GetObjectClass(env, arg);
    if (strcmp(className, "Eco.Core.IntegerPtr") == 0) {
        field = (*env)->GetIntField(env, clazz, "value");
        (*env)->SetIntField(env, arg, field, *(int*)argValue);
    } else if (strcmp(className, "Eco.Core.StringPtr") == 0) {
        field = (*env)->GetFieldID(env, clazz, "value", "Ljava/lang/String;");
        (*env)->SetObjectField(env, arg, field, (*env)->NewStringUTF(env, *(char_t**)argValue));
    } else if (strcmp(className, "Eco.Core.EcoConnectionData") == 0) {
        field = (*env)->GetIntField(env, clazz, "cCookie");
        (*env)->SetIntField(env, arg, field, (jint)(*(EcoConnectionData**)argValue)->cCookie);
        field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
        arg = (*env)->GetObjectField(env, arg, field);
        setPointerToInterface(env, arg, (void*)(*(EcoConnectionData**)argValue)->pUnk);
    } else if (strcmp(className, "Eco.Core.IEcoUnknownPtr") == 0) {
        field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
        arg = (*env)->GetObjectField(env, arg, field);
        setPointerToInterface(env, arg, **(void***)argValue);
    }
}

jobject getRetObj(JNIEnv* env, const char_t* className, ffi_arg result) {
    jclass clazz;
    jmethodID method;
    jobject retObj;

    if (strcmp(className, "void") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Void");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
    } else if (strcmp(className, "byte") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Byte");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(B)V");
        retObj = (*env)->NewObject(env, clazz, method, (jbyte)result);
    } else if (strcmp(className, "short") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Short");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(S)V");
        retObj = (*env)->NewObject(env, clazz, method, (jshort)result);
    } else if (strcmp(className, "int") == 0) {
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
    } else if (strcmp(className, "char") == 0) {
        clazz = (*env)->FindClass(env, "java/lang/Char");
        method = (*env)->GetMethodID(env, clazz, "<init>", "(C)V");
        retObj = (*env)->NewObject(env, clazz, method, (jchar)result);
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
        clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownNative");
        method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        retObj = (*env)->NewObject(env, clazz, method);
        setPointerToInterface(env, retObj, (void*)result);
    }
    return retObj;
}

JNIEXPORT jobject JNICALL Java_Eco_Core_IEcoUnknownNative_invokeNative(JNIEnv* env, jobject thisObj, jint VTblIdx, jobjectArray args, jclass returnType) {
    void* pIUnk = 0;
    void* pFunc = 0;
    jsize numArgs = 0;
    char_t* className = 0;
    jsize i = 0;
    char_t** argsClassNames = 0;
    ffi_type** ffiTypes;
    void** values;
    ffiData* ffiArgsData;
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
    argsClassNames = (char_t**)malloc((numArgs + 1) * sizeof(char_t*));
    ffiTypes = (ffi_type**)malloc((numArgs + 1) * sizeof(ffi_type*));
    values = (void**)malloc((numArgs + 1) * sizeof(void*));
    ffiArgsData = (ffiData*)calloc(numArgs + 1, sizeof(ffiData));
    ffiTypes[0] = &ffi_type_pointer;
    values[0] = &pIUnk;

    for (i = 0; i < numArgs; i++) {
        jobject arg = (*env)->GetObjectArrayElement(env, args, i);
        getObjectClassName(env, arg, &argsClassNames[i + 1]);
        getArgument(env, arg, argsClassNames[i + 1], &ffiTypes[i + 1], &values[i + 1], &ffiArgsData[i + 1]);
    }
    getClassName(env, returnType, &className);
    if (strcmp(className, "void") == 0) {
        rtype = ffi_type_void;
    } else if (strcmp(className, "byte") == 0) {
        rtype = ffi_type_sint8;
    } else if (strcmp(className, "short") == 0) {
        rtype = ffi_type_sint16;
    } else if (strcmp(className, "int") == 0) {
        rtype = ffi_type_sint32;
    } else if (strcmp(className, "long") == 0) {
        rtype = ffi_type_sint64;
    } else if (strcmp(className, "float") == 0) {
        rtype = ffi_type_float;
    } else if (strcmp(className, "double") == 0) {
        rtype = ffi_type_double;
    } else if (strcmp(className, "boolean") == 0) {
        rtype = ffi_type_sint8;
    } else if (strcmp(className, "char") == 0) {
        rtype = ffi_type_sint8;
    } else if (strcmp(className, "java.lang.String") == 0) {
        rtype = ffi_type_pointer;
    }

    status = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, numArgs + 1, &rtype, ffiTypes);
    if (status == FFI_OK) {
        ffi_call(&cif, FFI_FN(pFunc), &result, values);
    }

    free(ffiTypes);
    for (i = 0; i < numArgs; i++) {
        if (ffiArgsData[i + 1].out) {
            jobject arg = (*env)->GetObjectArrayElement(env, args, i);
            setOutArg(env, arg, argsClassNames[i + 1], values[i + 1]);
        }
        if (ffiArgsData[i + 1].ppAlloc) {
            free(*(void**)(values[i + 1]));
        }
        if (ffiArgsData[i + 1].pAlloc) {
            free(values[i + 1]);
        }
        free(argsClassNames[i + 1]);
    }
    free(values);
    free(ffiArgsData);
    free(argsClassNames);

    retObj = getRetObj(env, className, result);
    free(className);
    return retObj;
}
