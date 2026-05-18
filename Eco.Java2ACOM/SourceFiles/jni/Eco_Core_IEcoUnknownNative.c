#include "Eco_Core_IEcoUnknownNative.h"
#include "Eco_Java2ACOM.h"

typedef struct EcoTypeMap {
    bool_t isPrimitive;
    const char_t* jniSignature;
    const char_t* jniClassName;
} EcoTypeMap;

EcoTypeMap ECO_TYPE_MAP[] = {
    { 0, "",                       ""                     }, /* ECO_TYPE_UNDEFINED */
    { 1, "B",                      "java/lang/Byte"       }, /* ECO_TYPE_INT8 */
    { 1, "S",                      "java/lang/Short"      }, /* ECO_TYPE_INT16 */
    { 1, "I",                      "java/lang/Integer"    }, /* ECO_TYPE_INT32 */
    { 1, "J",                      "java/lang/Long"       }, /* ECO_TYPE_INT64 */
    { 1, "B",                      "java/lang/Byte"       }, /* ECO_TYPE_UINT8 */
    { 1, "S",                      "java/lang/Short"      }, /* ECO_TYPE_UINT16 */
    { 1, "I",                      "java/lang/Integer"    }, /* ECO_TYPE_UINT32 */
    { 1, "J",                      "java/lang/Long"       }, /* ECO_TYPE_UINT64 */
    { 1, "F",                      "java/lang/Float"      }, /* ECO_TYPE_FLOAT */
    { 1, "D",                      "java/lang/Double"     }, /* ECO_TYPE_DOUBLE */
    { 1, "Z",                      "java/lang/Boolean"    }, /* ECO_TYPE_BOOLEAN */
    { 1, "C",                      "java/lang/Character"  }, /* ECO_TYPE_CHAR */
    { 1, "C",                      "java/lang/Character"  }, /* ECO_TYPE_WCHAR */
    { 0, "Ljava/lang/String;",     "java/lang/String"     }, /* ECO_TYPE_ASTRING */
    { 0, "Ljava/lang/String;",     "java/lang/String"     }, /* ECO_TYPE_WSTRING */
    { 0, "LEco/Core/IEcoUnknown;", "Eco/Core/IEcoUnknown" }, /* ECO_TYPE_INTERFACE */
    { 0, "LEco/Core/UGUID;",       "Eco/Core/UGUID"       }, /* ECO_TYPE_UGUID */
    { 0, "Ljava/lang/Object;",     "java/lang/Object"     }, /* ECO_TYPE_VOIDPTR */
    { 0, "V",                      "" },                     /* ECO_TYPE_VOID */
};

ffi_type* ECO_FFI_TYPES[20];

ffi_type* GetFfiType(uint16_t typeTag) {
    if (ECO_FFI_TYPES[0] == 0) {
        ECO_FFI_TYPES[ECO_TYPE_UNDEFINED] = 1;
        ECO_FFI_TYPES[ECO_TYPE_INT8]      = &ffi_type_sint8;
        ECO_FFI_TYPES[ECO_TYPE_INT16]     = &ffi_type_sint16;
        ECO_FFI_TYPES[ECO_TYPE_INT32]     = &ffi_type_sint32;
        ECO_FFI_TYPES[ECO_TYPE_INT64]     = &ffi_type_sint64;
        ECO_FFI_TYPES[ECO_TYPE_UINT8]     = &ffi_type_uint8;
        ECO_FFI_TYPES[ECO_TYPE_UINT16]    = &ffi_type_uint16;
        ECO_FFI_TYPES[ECO_TYPE_UINT32]    = &ffi_type_uint32;
        ECO_FFI_TYPES[ECO_TYPE_UINT64]    = &ffi_type_uint64;
        ECO_FFI_TYPES[ECO_TYPE_FLOAT]     = &ffi_type_float;
        ECO_FFI_TYPES[ECO_TYPE_DOUBLE]    = &ffi_type_double;
        ECO_FFI_TYPES[ECO_TYPE_BOOLEAN]   = &ffi_type_uint8;
        ECO_FFI_TYPES[ECO_TYPE_CHAR]      = &ffi_type_sint8;
        ECO_FFI_TYPES[ECO_TYPE_WCHAR]     = &ffi_type_uint16;
        ECO_FFI_TYPES[ECO_TYPE_ASTRING]   = &ffi_type_pointer;
        ECO_FFI_TYPES[ECO_TYPE_WSTRING]   = &ffi_type_pointer;
        ECO_FFI_TYPES[ECO_TYPE_INTERFACE] = &ffi_type_pointer;
        ECO_FFI_TYPES[ECO_TYPE_UGUID]     = &ffi_type_pointer;
        ECO_FFI_TYPES[ECO_TYPE_VOIDPTR]   = &ffi_type_pointer;
        ECO_FFI_TYPES[ECO_TYPE_VOID]      = &ffi_type_void;
    }
    return ECO_FFI_TYPES[typeTag];
}

jobject UGUIDPtrToJavaObject(JNIEnv* env, const UGUID* uguid) {
    jclass clazz;
    jmethodID method;
    jbyteArray dataArr;
    jobject result;

    dataArr = (*env)->NewByteArray(env, uguid->Length);
    (*env)->SetByteArrayRegion(env, dataArr, 0, uguid->Length, uguid->Data);

    clazz = (*env)->FindClass(env, "Eco/Core/UGUID");
    method = (*env)->GetMethodID(env, clazz, "<init>", "(BB[B)V");
    result = (*env)->NewObject(env, clazz, method, (jbyte)uguid->Preamble, (jbyte)uguid->Length, dataArr);

    return result;
}

char_t* UGUIDPtrToTypeLibFileName(const UGUID* uguid) {
    char_t result[256] = "";
    byte_t i;
    for (i = 0; i < uguid->Length; i++) {
        byte_t b = uguid->Data[i] >> 4;
        if (b < 10) {
            result[i * 2] = b + '0';
        } else {
            result[i * 2] = (b - 10) + 'A';
        }
        b = uguid->Data[i] & 0xF;
        if (b < 10) {
            result[i * 2 + 1] = b + '0';
        } else {
            result[i * 2 + 1] = (b - 10) + 'A';
        }
    }
    strcat(result, ".etl");
    return _strdup(result);
}

UGUID JavaObjectToUGUIDPtr(JNIEnv* env, jobject obj) {
    jclass clazz = (*env)->GetObjectClass(env, obj);
    jfieldID field;
    jobject objBytes;
    jbyte* bytes;
    byte_t i;
    UGUID result = {0};

    field = (*env)->GetFieldID(env, clazz, "Preamble", "B");
    result.Preamble = (*env)->GetByteField(env, obj, field);

    field = (*env)->GetFieldID(env, clazz, "Length", "B");
    result.Length = (*env)->GetByteField(env, obj, field);

    field = (*env)->GetFieldID(env, clazz, "Data", "[B");
    objBytes = (*env)->GetObjectField(env, obj, field);
    bytes = (*env)->GetByteArrayElements(env, objBytes, 0);
    memcpy(result.Data, bytes, result.Length);
    (*env)->ReleaseByteArrayElements(env, objBytes, bytes, JNI_ABORT);

    return result;
}

UGUID GetUGUIDFromInterfaceJavaObject(JNIEnv* env, jobject obj) {
    jclass clazz = (*env)->GetObjectClass(env, obj);
    jfieldID field = (*env)->GetStaticFieldID(env, clazz, "IID", "LEco/Core/UGUID;");
    obj = (*env)->GetStaticObjectField(env, clazz, field);
    return JavaObjectToUGUIDPtr(env, obj);
}

jobject GetInterfaceObjectFromIEcoUnknownPtrObject(JNIEnv* env, jobject pIUnk) {
    jclass clazz = (*env)->GetObjectClass(env, pIUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
    return (*env)->GetObjectField(env, pIUnk, field);
}

IEcoInterfaceDescriptor1* GetInterfaceDescriptorByUGUID(IEcoTypeLib1* pITypeLib, const UGUID* riid) {
    IEcoInterfaceDirectory1* pIDirectory = 0;
    IEcoInterfaceDirectoryEntry1* pIEntry = 0;
    IEcoInterfaceDescriptor1* pIDesc = 0;
    char_t* fileName = 0;
    int16_t result = 0;

    fileName = UGUIDPtrToTypeLibFileName(riid);
    result = pITypeLib->pVTbl->LoadFile(pITypeLib, fileName, &pIDirectory);
    if (result != 0) {
        char_t* rtPath = getenv("ECO_FRAMEWORK_RT");
        char_t filePath[256] = "";
        strcpy(filePath, rtPath);
        strcat(filePath, "/");
        strcat(filePath, fileName);
        result = pITypeLib->pVTbl->LoadFile(pITypeLib, filePath, &pIDirectory);
    }

    if (result == 0) {
        result = pIDirectory->pVTbl->GetEntryByIID(pIDirectory, riid, &pIEntry);
        result = pIEntry->pVTbl->get_Descriptor(pIEntry, &pIDesc);
    }

    return pIDesc;
}

IEcoInterfaceDescriptor1* GetInterfaceDescriptor(JNIEnv* env, jobject iUnk) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pIDesc", "J");
    IEcoInterfaceDescriptor1* pIDesc = (IEcoInterfaceDescriptor1*) (*env)->GetLongField(env, iUnk, field);

    if (pIDesc == 0) {
        IEcoTypeLib1* pITypeLib = GetTypeLibPointer(env, iUnk);
        UGUID riid = GetUGUIDFromInterfaceJavaObject(env, iUnk);
        pIDesc = GetInterfaceDescriptorByUGUID(pITypeLib, &riid);
        (*env)->SetLongField(env, iUnk, field, (jlong)pIDesc);
    }

    return pIDesc;
}

void SetInterfaceDescriptor(JNIEnv* env, jobject iUnk, IEcoInterfaceDescriptor1* pIDesc) {
    jclass clazz = (*env)->GetObjectClass(env, iUnk);
    jfieldID field = (*env)->GetFieldID(env, clazz, "pIDesc", "J");
    (*env)->SetLongField(env, iUnk, field, (jlong)pIDesc);
}

void JavaObjectToParam(JNIEnv* env, jobject obj, uint16_t typeTag, uint8_t flags, void** arg) {
    if (flags & ECO_PARAM_OUT) {
        *arg = malloc(sizeof(void*));
        arg = *arg;
    }
    *arg = malloc(ECO_TYPE_SIZE[typeTag]);
    if (ECO_TYPE_MAP[typeTag].isPrimitive) {
        jclass clazz = (*env)->GetObjectClass(env, obj);
        jfieldID field = (*env)->GetFieldID(env, clazz, "value", ECO_TYPE_MAP[typeTag].jniSignature);
        if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
            **(jbyte**)arg = (*env)->GetByteField(env, obj, field);
        } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
            **(jshort**)arg = (*env)->GetShortField(env, obj, field);
        } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
            **(jint**)arg = (*env)->GetIntField(env, obj, field);
        } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
            **(jlong**)arg = (*env)->GetLongField(env, obj, field);
        } else if (typeTag == ECO_TYPE_FLOAT) {
            **(jfloat**)arg = (*env)->GetFloatField(env, obj, field);
        } else if (typeTag == ECO_TYPE_DOUBLE) {
            **(jdouble**)arg = (*env)->GetDoubleField(env, obj, field);
        } else if (typeTag == ECO_TYPE_BOOLEAN) {
            **(jboolean**)arg = (*env)->GetBooleanField(env, obj, field);
        } else if (typeTag == ECO_TYPE_CHAR) {
            **(char_t**)arg = (*env)->GetCharField(env, obj, field);
        } else if (typeTag == ECO_TYPE_WCHAR) {
            **(wchar_t**)arg = (*env)->GetCharField(env, obj, field);
        }
    } else if (typeTag == ECO_TYPE_ASTRING) {
        **(char_t***)arg = (*env)->GetStringUTFChars(env, obj, 0);
    } else if (typeTag == ECO_TYPE_WSTRING) {
        **(wchar_t***)arg = (*env)->GetStringChars(env, obj, 0);
    } else if (typeTag == ECO_TYPE_INTERFACE) {
        if (flags & ECO_PARAM_OUT) {
            obj = GetInterfaceObjectFromIEcoUnknownPtrObject(env, obj);
        }
        **(void***)arg = GetPointerToInterface(env, obj);
    } else if (typeTag == ECO_TYPE_UGUID) {
        **(UGUID***)arg = malloc(sizeof(UGUID));
        ***(UGUID***)arg = JavaObjectToUGUIDPtr(env, obj);;
    } else if (typeTag == ECO_TYPE_VOIDPTR) {
        **(jobject**)arg = (*env)->NewGlobalRef(env, obj);
    }
}

void ParamToJavaObject(JNIEnv* env, void* arg, uint16_t typeTag, jobject* obj) {
    if (ECO_TYPE_MAP[typeTag].isPrimitive) {
        jclass clazz = (*env)->GetObjectClass(env, *obj);
        jfieldID field = (*env)->GetFieldID(env, clazz, "value", ECO_TYPE_MAP[typeTag].jniSignature);
        if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
            (*env)->SetByteField(env, *obj, field, **(jbyte**)arg);
        } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
            (*env)->SetShortField(env, *obj, field, **(jshort**)arg);
        } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
            (*env)->SetIntField(env, *obj, field, **(jint**)arg);
        } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
            (*env)->SetLongField(env, *obj, field, **(jlong**)arg);
        } else if (typeTag == ECO_TYPE_FLOAT) {
            (*env)->SetFloatField(env, *obj, field, **(jfloat**)arg);
        } else if (typeTag == ECO_TYPE_DOUBLE) {
            (*env)->SetDoubleField(env, *obj, field, **(jdouble**)arg);
        } else if (typeTag == ECO_TYPE_BOOLEAN) {
            (*env)->SetBooleanField(env, *obj, field, **(jboolean**)arg);
        } else if (typeTag == ECO_TYPE_CHAR) {
            (*env)->SetCharField(env, *obj, field, **(char_t**)arg);
        } else if (typeTag == ECO_TYPE_WCHAR) {
            (*env)->SetCharField(env, *obj, field, **(wchar_t**)arg);
        }
    } else if (typeTag == ECO_TYPE_ASTRING) {
        *obj = (*env)->NewStringUTF(env, **(char_t***)arg);
    } else if (typeTag == ECO_TYPE_WSTRING) {
        *obj = (*env)->NewString(env, **(wchar_t***)arg, strlen(**(wchar_t***)arg));
    } else if (typeTag == ECO_TYPE_INTERFACE) {
        jobject iUnk = GetInterfaceObjectFromIEcoUnknownPtrObject(env, *obj);
        SetPointerToInterface(env, iUnk, **(void***)arg);
    } else if (typeTag == ECO_TYPE_UGUID) {
        *obj = UGUIDPtrToJavaObject(env, **(UGUID***)arg);
    } else if (typeTag == ECO_TYPE_VOIDPTR) {
        *obj = **(jobject**)arg;
    }
}

jobject ResultToJavaObject(JNIEnv* env, ffi_arg* result, uint16_t typeTag) {
    EcoTypeMap* ecoType = &ECO_TYPE_MAP[typeTag];
    if (ecoType->isPrimitive) {
        jclass clazz;
        jmethodID method;
        char_t jniSig[5];
        sprintf(jniSig, "(%s)V", ecoType->jniSignature);
        clazz = (*env)->FindClass(env, ecoType->jniClassName);
        method = (*env)->GetMethodID(env, clazz, "<init>", jniSig);
        if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
            return (*env)->NewObject(env, clazz, method, *(jbyte*)result);
        } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
            return (*env)->NewObject(env, clazz, method, *(jshort*)result);
        } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
            return (*env)->NewObject(env, clazz, method, *(jint*)result);
        } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
            return (*env)->NewObject(env, clazz, method, *(jlong*)result);
        } else if (typeTag == ECO_TYPE_FLOAT) {
            return (*env)->NewObject(env, clazz, method, *(jfloat*)result);
        } else if (typeTag == ECO_TYPE_DOUBLE) {
            return (*env)->NewObject(env, clazz, method, *(jdouble*)result);
        } else if (typeTag == ECO_TYPE_BOOLEAN) {
            return (*env)->NewObject(env, clazz, method, *(jboolean*)result);
        } else if (typeTag == ECO_TYPE_CHAR) {
            return (*env)->NewObject(env, clazz, method, *(char_t*)result);
        } else if (typeTag == ECO_TYPE_WCHAR) {
            return (*env)->NewObject(env, clazz, method, *(wchar_t*)result);
        }
    } else if (typeTag == ECO_TYPE_ASTRING) {
        return (*env)->NewStringUTF(env, *(char_t**)result);
    } else if (typeTag == ECO_TYPE_WSTRING) {
        return (*env)->NewString(env, *(char_t**)result, strlen(*(wchar_t**)result));
    } else if (typeTag == ECO_TYPE_UGUID) {
        return UGUIDPtrToJavaObject(env, *(UGUID**)result);
    } else if (typeTag == ECO_TYPE_VOIDPTR) {
        return *(jobject*)result;
    } else if (typeTag == ECO_TYPE_VOID) {
        jclass clazz = (*env)->FindClass(env, "java/lang/Void");
        jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
        return (*env)->NewObject(env, clazz, method);
    }
}

JNIEXPORT jshort JNICALL Java_Eco_Core_IEcoUnknownNative_QueryInterface(JNIEnv* env, jobject thisObj, jobject riidObj, jobject pIUnkObj) {
    IEcoUnknown* me = (IEcoUnknown*) GetPointerToInterface(env, thisObj);
    UGUID riid = JavaObjectToUGUIDPtr(env, riidObj);
    void* pv = 0;
    int16_t result = me->pVTbl->QueryInterface(me, &riid, &pv);
    jobject iUnk = GetInterfaceObjectFromIEcoUnknownPtrObject(env, pIUnkObj);
    SetPointerToInterface(env, iUnk, pv);
    return result;
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknownNative_AddRef(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* me = (IEcoUnknown*) GetPointerToInterface(env, thisObj);
    return me->pVTbl->AddRef(me);
}

JNIEXPORT jint JNICALL Java_Eco_Core_IEcoUnknownNative_Release(JNIEnv* env, jobject thisObj) {
    IEcoUnknown* me = (IEcoUnknown*) GetPointerToInterface(env, thisObj);
    return me->pVTbl->Release(me);
}

JNIEXPORT jobject JNICALL Java_Eco_Core_IEcoUnknownNative_GlobalDispatcher(JNIEnv* env, jobject thisObj, jint VTblIdx, jobjectArray args) {
    IEcoInterfaceDescriptor1* pIDesc = GetInterfaceDescriptor(env, thisObj);
    IEcoMethodDescriptor1* pIMethod = 0;
    IEcoParamDescriptor1* pIParam = 0;
    uint8_t count = (*env)->GetArrayLength(env, args);
    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint8_t index = 0;

    jobject result = 0;
    void* pIUnk = 0;
    void* pFunc = 0;

    ffi_status status;
    ffi_cif cif;
    ffi_arg cResult;

    void** cArgs = (void**) malloc(sizeof(void*) * (count + 1));
    ffi_type** ffiTypes = (ffi_type**) malloc(sizeof(ffi_type*) * (count + 1));
    ffi_type* retType = 0;

    pIUnk = GetPointerToInterface(env, thisObj);
    pFunc = (*(void***)pIUnk)[VTblIdx];

    cArgs[0] = &pIUnk;
    ffiTypes[0] = &ffi_type_pointer;

    pIDesc->pVTbl->get_MethodAtIndex(pIDesc, VTblIdx - 3, &pIMethod);
    for (index = 0; index < count; index++) {
        jobject arg = (*env)->GetObjectArrayElement(env, args, index);

        pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        pIParam->pVTbl->get_Type(pIParam, &typeTag);
        flags = pIParam->pVTbl->get_Flags(pIParam);

        JavaObjectToParam(env, arg, typeTag, flags, &cArgs[index + 1]);
        if (flags & ECO_PARAM_OUT) {
            ffiTypes[index + 1] = &ffi_type_pointer;
        } else {
            ffiTypes[index + 1] = GetFfiType(typeTag);
        }
    }

    pIMethod->pVTbl->get_Result(pIMethod, &pIParam);
    pIParam->pVTbl->get_Type(pIParam, &typeTag);
    retType = GetFfiType(typeTag);

    status = ffi_prep_cif(&cif, FFI_STDCALL, count + 1, retType, ffiTypes);
    if (status == FFI_OK) {
        ffi_call(&cif, FFI_FN(pFunc), &cResult, cArgs);
    }
    result = ResultToJavaObject(env, &cResult, typeTag);

    for (index = 0; index < count; index++) {
        pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        flags = pIParam->pVTbl->get_Flags(pIParam);
        if (flags & ECO_PARAM_OUT) {
            jobject arg = (*env)->GetObjectArrayElement(env, args, index);
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            ParamToJavaObject(env, cArgs[index + 1], typeTag, &arg);
        }
    }

    for (index = 0; index < count; index++) {
        pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        pIParam->pVTbl->get_Type(pIParam, &typeTag);
        flags = pIParam->pVTbl->get_Flags(pIParam);
        if ((flags & ECO_PARAM_OUT) && typeTag == ECO_TYPE_UGUID) {
            free(**(void***)(cArgs[index + 1]));
        }
        if ((flags & ECO_PARAM_OUT) || typeTag == ECO_TYPE_UGUID) {
            free(*(void**)(cArgs[index + 1]));
        }
        free(cArgs[index + 1]);
    }
    free(cArgs);
    free(ffiTypes);

    return result;
}
