/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 *
 * <summary>
 *   CEcoACOM2Java_3F41E2AA
 * </summary>
 *
 * <description>
 *   This source code describes the implementation of the interfaces for CEcoACOM2Java_3F41E2AA
 * </description>
 *
 * <author>
 *   Copyright (c) 2026 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoACOM2Java.h"
#include "IdEcoList1.h"


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

EcoJavaProxy* CreateEcoJavaProxy(JNIEnv*, jobject, IEcoInterfaceDescriptor1*, IEcoMemoryAllocator1*, IEcoTypeLib1*);

JNIEnv* GetCurrentEnv(JavaVM* jvm) {
    JNIEnv* env = 0;
    jint status = 0;
    if (jvm == 0) {
        return 0;
    }
    status = (*jvm)->GetEnv(jvm, (void**)&env, JNI_VERSION_1_8);
    if (status == JNI_EDETACHED) {
        if ((*jvm)->AttachCurrentThread(jvm, (void**)&env, 0) != 0) {
            return 0;
        }
    } else if (status != JNI_OK) {
        return 0;
    }
    return env;
}

jobject UGUIDPtrToJavaObject(JNIEnv* env, const UGUID* uguid) {
    jclass clazz;
    jmethodID method;
    jbyteArray dataArr;

    dataArr = (*env)->NewByteArray(env, uguid->Length);
    if (dataArr == NULL) return NULL;
    (*env)->SetByteArrayRegion(env, dataArr, 0, uguid->Length, uguid->Data);
    if ((*env)->ExceptionCheck(env)) return NULL;

    clazz = (*env)->FindClass(env, "Eco/Core/UGUID");
    if (clazz == NULL) return NULL;
    method = (*env)->GetMethodID(env, clazz, "<init>", "(BB[B)V");
    if (method == NULL) return NULL;
    {
        jobject obj = (*env)->NewObject(env, clazz, method, (jbyte)uguid->Preamble, (jbyte)uguid->Length, dataArr);
        if ((*env)->ExceptionCheck(env)) return NULL;
        return obj;
    }
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
    if (field == NULL) return result;
    result.Preamble = (*env)->GetByteField(env, obj, field);

    field = (*env)->GetFieldID(env, clazz, "Length", "B");
    if (field == NULL) return result;
    result.Length = (*env)->GetByteField(env, obj, field);

    field = (*env)->GetFieldID(env, clazz, "Data", "[B");
    if (field == NULL) return result;
    objBytes = (*env)->GetObjectField(env, obj, field);
    if (objBytes == NULL) return result;
    bytes = (*env)->GetByteArrayElements(env, objBytes, 0);
    if (bytes == NULL) return result;
    memcpy(result.Data, bytes, result.Length);
    (*env)->ReleaseByteArrayElements(env, objBytes, bytes, JNI_ABORT);

    return result;
}

UGUID GetUGUIDFromInterfaceJavaObject(JNIEnv* env, jobject obj) {
    jclass clazz = (*env)->GetObjectClass(env, obj);
    jfieldID field = (*env)->GetStaticFieldID(env, clazz, "IID", "LEco/Core/UGUID;");
    UGUID empty = {0};
    if (field == NULL) return empty;
    obj = (*env)->GetStaticObjectField(env, clazz, field);
    if (obj == NULL) return empty;
    return JavaObjectToUGUIDPtr(env, obj);
}

void AddClassPath(JNIEnv* env, char_t* classpath) {
    jclass clazz;
    jmethodID method;
    jobject fileObj;
    jobject loaderObj;

    clazz = (*env)->FindClass(env, "java/io/File");
    method = (*env)->GetMethodID(env, clazz, "<init>", "(Ljava/lang/String;)V");
    fileObj = (*env)->NewObject(env, clazz, method, (*env)->NewStringUTF(env, classpath));

    method = (*env)->GetMethodID(env, clazz, "toURI", "()Ljava/net/URI;");
    fileObj = (*env)->CallObjectMethod(env, fileObj, method);

    clazz = (*env)->GetObjectClass(env, fileObj);
    method = (*env)->GetMethodID(env, clazz, "toURL", "()Ljava/net/URL;");
    fileObj = (*env)->CallObjectMethod(env, fileObj, method);

    clazz = (*env)->FindClass(env, "java/lang/ClassLoader");
    method = (*env)->GetStaticMethodID(env, clazz, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    loaderObj = (*env)->CallStaticObjectMethod(env, clazz, method);

    clazz = (*env)->FindClass(env, "java/net/URLClassLoader");
    method = (*env)->GetMethodID(env, clazz, "addURL", "(Ljava/net/URL;)V");
    (*env)->CallVoidMethod(env, loaderObj, method, fileObj);
}

char_t* GenerateJniSignature(IEcoMethodDescriptor1* pIMethod) {
    char_t sig[256] = "(";
    uint8_t count = pIMethod->pVTbl->get_ParamCount(pIMethod);
    IEcoParamDescriptor1* pIParam = 0;
    uint16_t typeTag = 0;
    uint8_t index = 0;
    uint8_t flags = 0;
    int16_t result = 0;

    for (index = 0; index < count; index++) {
        result = pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        result = pIParam->pVTbl->get_Type(pIParam, &typeTag);
        flags = pIParam->pVTbl->get_Flags(pIParam);
        if (flags & ECO_PARAM_OUT) {
            sprintf(sig + strlen(sig), "L%s;", ECO_TYPE_MAP[typeTag].jniClassName);
        } else if (flags & ECO_PARAM_IN) {
            strcat(sig, ECO_TYPE_MAP[typeTag].jniSignature);
        }
    }
    strcat(sig, ")");

    result = pIMethod->pVTbl->get_Result(pIMethod, &pIParam);
    result = pIParam->pVTbl->get_Type(pIParam, &typeTag);
    strcat(sig, ECO_TYPE_MAP[typeTag].jniSignature);

    return _strdup(sig);
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
    free(fileName);

    if (result == 0) {
        result = pIDirectory->pVTbl->GetEntryByIID(pIDirectory, riid, &pIEntry);
        result = pIEntry->pVTbl->get_Descriptor(pIEntry, &pIDesc);
    }

    return pIDesc;
}

void ParamToJavaValue(JNIEnv* env, void* arg, uint16_t typeTag, uint8_t flags, jvalue* jarg) {
    EcoTypeMap* ecoType = &ECO_TYPE_MAP[typeTag];
    if (ecoType->isPrimitive) {
        if (flags & ECO_PARAM_OUT) {
            jclass clazz;
            jmethodID method;
            char_t valueOfSig[64];
            jvalue args[1];
            sprintf(valueOfSig, "(%s)L%s;", ecoType->jniSignature, ecoType->jniClassName);
            clazz = (*env)->FindClass(env, ecoType->jniClassName);
            if (clazz == NULL) return;
            method = (*env)->GetStaticMethodID(env, clazz, "valueOf", valueOfSig);
            if (method == NULL) return;
            if (flags & ECO_PARAM_IN) {
                if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
                    args[0].b = **(jbyte**)arg;
                } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
                    args[0].s = **(jshort**)arg;
                } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
                    args[0].i = **(jint**)arg;
                } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
                    args[0].j = **(jlong**)arg;
                } else if (typeTag == ECO_TYPE_FLOAT) {
                    args[0].f = **(jfloat**)arg;
                } else if (typeTag == ECO_TYPE_DOUBLE) {
                    args[0].d = **(jdouble**)arg;
                } else if (typeTag == ECO_TYPE_BOOLEAN) {
                    args[0].z = **(jboolean**)arg;
                } else if (typeTag == ECO_TYPE_CHAR) {
                    args[0].c = **(char_t**)arg;
                } else if (typeTag == ECO_TYPE_WCHAR) {
                    args[0].c = **(wchar_t**)arg;
                }
            } else {
                args[0].j = 0LL;
            }
            jarg->l = (*env)->CallStaticObjectMethodA(env, clazz, method, args);
            if ((*env)->ExceptionCheck(env)) return;
        } else if (flags & ECO_PARAM_IN) {
            if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
                jarg->b = *(jbyte*)arg;
            } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
                jarg->s = *(jshort*)arg;
            } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
                jarg->i = *(jint*)arg;
            } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
                jarg->j = *(jlong*)arg;
            } else if (typeTag == ECO_TYPE_FLOAT) {
                jarg->f = *(jfloat*)arg;
            } else if (typeTag == ECO_TYPE_DOUBLE) {
                jarg->d = *(jdouble*)arg;
            } else if (typeTag == ECO_TYPE_BOOLEAN) {
                jarg->z = *(jboolean*)arg;
            } else if (typeTag == ECO_TYPE_CHAR) {
                jarg->c = *(char_t*)arg;
            } else if (typeTag == ECO_TYPE_WCHAR) {
                jarg->c = *(wchar_t*)arg;
            }
        }
    } else if (typeTag == ECO_TYPE_ASTRING) {
        if (flags & ECO_PARAM_IN) {
            if (flags & ECO_PARAM_OUT) {
                jarg->l = (*env)->NewStringUTF(env, **(char_t***)arg);
            } else {
                jarg->l = (*env)->NewStringUTF(env, *(char_t**)arg);
            }
        } else {
            jarg->l = (*env)->NewStringUTF(env, "");
        }
    } else if (typeTag == ECO_TYPE_WSTRING) {
        if (flags & ECO_PARAM_IN) {
            if (flags & ECO_PARAM_OUT) {
                jarg->l = (*env)->NewString(env, **(wchar_t***)arg, strlen(**(wchar_t***)arg));
            } else {
                jarg->l = (*env)->NewString(env, *(wchar_t**)arg, strlen(*(wchar_t**)arg));
            }
        } else {
            jarg->l = (*env)->NewString(env, "", 0);
        }
    } else if (typeTag == ECO_TYPE_INTERFACE) {
        if (flags & ECO_PARAM_OUT) {
            jclass clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
            if (flags & ECO_PARAM_IN) {
                jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "(LEco/Core/IEcoUnknown;)V");
                jarg->l = (*env)->NewObject(env, clazz, method, (*(EcoJavaProxy**)arg)->m_obj);
            } else {
                jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
                jarg->l = (*env)->NewObject(env, clazz, method);
            }
        } else if (flags & ECO_PARAM_IN) {
            jarg->l = (*(EcoJavaProxy**)arg)->m_obj;
        }
    } else if (typeTag == ECO_TYPE_UGUID) {
        if (flags & ECO_PARAM_IN) {
            jarg->l = UGUIDPtrToJavaObject(env, *(UGUID**)arg);
        } else if (flags & ECO_PARAM_OUT) {
            jclass clazz = (*env)->FindClass(env, "Eco/Core/UGUID");
            jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
            jarg->l = (*env)->NewObject(env, clazz, method);
        }
    } else if (typeTag == ECO_TYPE_VOIDPTR) {
        if (flags & ECO_PARAM_IN) {
            jclass clazz = (*env)->FindClass(env, "java/lang/Integer");
            jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "(I)V");
            jarg->l = (*env)->NewObject(env, clazz, method, *(uint32_t*)arg);
        } else if (flags & ECO_PARAM_OUT) {
            jclass clazz = (*env)->FindClass(env, "java/lang/Object");
            jmethodID method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
            jarg->l = (*env)->NewObject(env, clazz, method);
        }
    }
}

void JavaValueToParam(EcoJavaProxy* proxy, jvalue jarg, uint16_t typeTag, void** arg) {
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    if (env == 0) return;
    if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
        **(jbyte**)arg = jarg.b;
    } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
        **(jshort**)arg = jarg.s;
    } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
        **(jint**)arg = jarg.i;
    } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
        **(jlong**)arg = jarg.j;
    } else if (typeTag == ECO_TYPE_FLOAT) {
        **(jfloat**)arg = jarg.f;
    } else if (typeTag == ECO_TYPE_DOUBLE) {
        **(jdouble**)arg = jarg.d;
    } else if (typeTag == ECO_TYPE_BOOLEAN) {
        **(jboolean**)arg = jarg.b;
    } else if (typeTag == ECO_TYPE_CHAR) {
        **(char_t**)arg = jarg.c;
    } else if (typeTag == ECO_TYPE_WCHAR) {
        **(wchar_t**)arg = jarg.c;
    } else if (typeTag == ECO_TYPE_ASTRING) {
        **(char_t***)arg = (*env)->GetStringUTFChars(env, jarg.l, 0);
    } else if (typeTag == ECO_TYPE_WSTRING) {
        **(wchar_t***)arg = (*env)->GetStringChars(env, jarg.l, 0);
    } else if (typeTag == ECO_TYPE_INTERFACE) {
        jclass clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
        jfieldID field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
        jobject obj = (*env)->GetObjectField(env, jarg.l, field);
        UGUID riid = GetUGUIDFromInterfaceJavaObject(env, obj);
        IEcoInterfaceDescriptor1* pIDesc = GetInterfaceDescriptorByUGUID(proxy->m_pITypeLib, &riid);
        CreateEcoJavaProxy(env, obj, pIDesc, proxy->m_pIMem, proxy->m_pITypeLib);
    } else if (typeTag == ECO_TYPE_UGUID) {
        **(UGUID**)arg = JavaObjectToUGUIDPtr(env, jarg.l);
    } else if (typeTag == ECO_TYPE_VOIDPTR) {
        **(jobject**)arg = jarg.l;
    }
}

void CallJavaMethod(EcoJavaProxy* proxy, jobject obj, jmethodID method, jvalue* jArgs, uint16_t typeTag, void* ret) {
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    if (env == 0) return;
    if (typeTag == ECO_TYPE_INT8 || typeTag == ECO_TYPE_UINT8) {
        *(jbyte*)ret = (*env)->CallByteMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_INT16 || typeTag == ECO_TYPE_UINT16) {
        *(jshort*)ret = (*env)->CallShortMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_INT32 || typeTag == ECO_TYPE_UINT32) {
        *(jint*)ret = (*env)->CallIntMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_INT64 || typeTag == ECO_TYPE_UINT64) {
        *(jlong*)ret = (*env)->CallLongMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_FLOAT) {
        *(jfloat*)ret = (*env)->CallFloatMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_DOUBLE) {
        *(jdouble*)ret = (*env)->CallDoubleMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_BOOLEAN) {
        *(jboolean*)ret = (*env)->CallBooleanMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_CHAR) {
        *(char_t*)ret = (*env)->CallCharMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_WCHAR) {
        *(wchar_t*)ret = (*env)->CallCharMethodA(env, obj, method, jArgs);
    } else if (typeTag == ECO_TYPE_VOID) {
        (*env)->CallVoidMethodA(env, obj, method, jArgs);
    } else {
        jobject retObj = (*env)->CallObjectMethodA(env, obj, method, jArgs);
        if (typeTag == ECO_TYPE_ASTRING) {
            *(char_t**)ret = (*env)->GetStringUTFChars(env, retObj, 0);
        } else if (typeTag == ECO_TYPE_WSTRING) {
            *(wchar_t**)ret = (*env)->GetStringChars(env, retObj, 0);
        } else if (typeTag == ECO_TYPE_INTERFACE) {
            UGUID riid = GetUGUIDFromInterfaceJavaObject(env, retObj);
            IEcoInterfaceDescriptor1* pIDesc = GetInterfaceDescriptorByUGUID(proxy->m_pITypeLib, &riid);
            CreateEcoJavaProxy(env, retObj, pIDesc, proxy->m_pIMem, proxy->m_pITypeLib);
        } else if (typeTag == ECO_TYPE_UGUID) {
            *(UGUID*)ret = JavaObjectToUGUIDPtr(env, retObj);
        } else if (typeTag == ECO_TYPE_VOIDPTR) {
            *(jobject*)ret = retObj;
        }
    }
}


static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    IEcoInterfaceDescriptor1* pIDesc = 0;
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    jclass pIUnkClazz;
    jclass clazz;
    jmethodID method;
    jfieldID field;
    jobject obj;
    int16_t result = 0;

    if (env == 0) return ERR_ECO_FAIL;

    pIUnkClazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
    method = (*env)->GetMethodID(env, pIUnkClazz, "<init>", "()V");
    obj = (*env)->NewObject(env, pIUnkClazz, method);

    clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    method = (*env)->GetMethodID(env, clazz, "QueryInterface", "(LEco/Core/UGUID;LEco/Core/IEcoUnknownPtr;)S");

    result = (*env)->CallShortMethod(env, proxy->m_obj, method, UGUIDPtrToJavaObject(env, riid), obj);
    if (result != 0) {
        return result;
    }

    field = (*env)->GetFieldID(env, pIUnkClazz, "iUnk", "LEco/Core/IEcoUnknown;");
    obj = (*env)->GetObjectField(env, obj, field);

    pIDesc = GetInterfaceDescriptorByUGUID(proxy->m_pITypeLib, riid);
    *(EcoJavaProxy**)ppv = CreateEcoJavaProxy(env, obj, pIDesc, proxy->m_pIMem, proxy->m_pITypeLib);

    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_AddRef(IEcoUnknownPtr_t me) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    jclass clazz;
    jmethodID method;
    uint32_t result;

    if (env == 0) return ERR_ECO_FAIL;

    clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    method = (*env)->GetMethodID(env, clazz, "AddRef", "()I");
    result = (uint32_t) (*env)->CallIntMethod(env, proxy->m_obj, method);
    ++proxy->m_cRef;
    return result;
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_Release(IEcoUnknownPtr_t me) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    jclass clazz;
    jmethodID method;
    uint32_t result;

    if (env == 0) return ERR_ECO_FAIL;

    clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    method = (*env)->GetMethodID(env, clazz, "Release", "()I");
    result = (uint32_t) (*env)->CallIntMethod(env, proxy->m_obj, method);
    if (--proxy->m_cRef == 0) {
        uint16_t count = proxy->m_pIDesc->pVTbl->get_MethodCount(proxy->m_pIDesc);
        uint16_t index = 0;
        for (; index < count; index++) {
            ffi_closure_free(proxy->m_methods[index].closure);
        }
        (*env)->DeleteGlobalRef(env, proxy->m_obj);
        proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy);
    }
    return result;
}

void EcoJavaProxy_GlobalDispatcher(ffi_cif* cif, void* ret, void** args, void* userData) {
    MethodContext* ctx = (MethodContext*) userData;
    EcoJavaProxy* proxy = ctx->proxy;
    JNIEnv* env = GetCurrentEnv(proxy->m_jvm);
    IEcoMethodDescriptor1* pIMethod = ctx->methodDesc;
    uint8_t count = pIMethod->pVTbl->get_ParamCount(pIMethod);
    jvalue* jArgs = 0;
    IEcoParamDescriptor1* pIParam = 0;
    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint8_t index = 0;

    if (env == 0) return;

    jArgs = (jvalue*) proxy->m_pIMem->pVTbl->Alloc(proxy->m_pIMem, sizeof(jvalue) * count);

    if (ctx->methodId == 0) {
        jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
        ctx->methodId = (*env)->GetMethodID(env, clazz, ctx->jniName, ctx->jniSig);
    }

    for (index = 0; index < count; index++) {
        pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        pIParam->pVTbl->get_Type(pIParam, &typeTag);
        flags = pIParam->pVTbl->get_Flags(pIParam);
        ParamToJavaValue(env, args[index + 1], typeTag, flags, &jArgs[index]);
    }

    pIMethod->pVTbl->get_Result(pIMethod, &pIParam);
    pIParam->pVTbl->get_Type(pIParam, &typeTag);
    CallJavaMethod(proxy, proxy->m_obj, ctx->methodId, jArgs, typeTag, ret);

    for (index = 0; index < count; index++) {
        pIMethod->pVTbl->GetParamAtIndex(pIMethod, index, &pIParam);
        flags = pIParam->pVTbl->get_Flags(pIParam);
        if (flags & ECO_PARAM_OUT) {
            pIParam->pVTbl->get_Type(pIParam, &typeTag);
            JavaValueToParam(proxy, jArgs[index], typeTag, &args[index + 1]);
        }
    }
    proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, jArgs);
}

EcoJavaProxy* CreateEcoJavaProxy(JNIEnv* env, jobject obj, IEcoInterfaceDescriptor1* pIDesc, IEcoMemoryAllocator1* pIMem, IEcoTypeLib1* pITypeLib) {
    uint16_t mCount = pIDesc->pVTbl->get_MethodCount(pIDesc);
    EcoJavaProxy* proxy = (EcoJavaProxy*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoJavaProxy));
    uint16_t mIndex = 0;
    int16_t result = 0;
    proxy->m_pVTbl = (void**) pIMem->pVTbl->Alloc(pIMem, sizeof(void*) * (mCount + 3));
    proxy->m_cRef = 1;
    proxy->m_pIMem = pIMem;
    proxy->m_pITypeLib = pITypeLib;
    proxy->m_pIDesc = pIDesc;
    (*env)->GetJavaVM(env, &proxy->m_jvm);
    proxy->m_obj = (*env)->NewGlobalRef(env, obj);
    proxy->m_methods = (MethodContext*) pIMem->pVTbl->Alloc(pIMem, sizeof(MethodContext) * mCount);

    proxy->m_pVTbl[0] = EcoJavaProxy_IEcoUnknown_QueryInterface;
    proxy->m_pVTbl[1] = EcoJavaProxy_IEcoUnknown_AddRef;
    proxy->m_pVTbl[2] = EcoJavaProxy_IEcoUnknown_Release;

    for (mIndex = 0; mIndex < mCount; mIndex++) {
        MethodContext* ctx = &proxy->m_methods[mIndex];
        IEcoMethodDescriptor1* pIMethod = 0;
        IEcoParamDescriptor1* pIParam = 0;
        uint16_t typeTag = 0;
        uint8_t flags = 0;
        uint8_t pCount = 0;
        uint8_t pIndex = 0;
        ffi_type** argTypes = 0;
        ffi_type* retType = 0;

        result = pIDesc->pVTbl->get_MethodAtIndex(pIDesc, mIndex, &pIMethod);
        ctx->proxy = proxy;
        ctx->methodIndex = mIndex;
        ctx->methodDesc = pIMethod;
        pCount = pIMethod->pVTbl->get_ParamCount(pIMethod);
        argTypes = (ffi_type**) pIMem->pVTbl->Alloc(pIMem, sizeof(ffi_type*) * (pCount + 1));
        argTypes[0] = &ffi_type_pointer;
        for (pIndex = 0; pIndex < pCount; pIndex++) {
            result = pIMethod->pVTbl->GetParamAtIndex(pIMethod, pIndex, &pIParam);
            result = pIParam->pVTbl->get_Type(pIParam, &typeTag);
            flags = pIParam->pVTbl->get_Flags(pIParam);
            if (flags & ECO_PARAM_OUT) {
                argTypes[pIndex + 1] = &ffi_type_pointer;
            } else if (flags & ECO_PARAM_IN) {
                argTypes[pIndex + 1] = GetFfiType(typeTag);
            }
        }
        result = pIMethod->pVTbl->get_Result(pIMethod, &pIParam);
        result = pIParam->pVTbl->get_Type(pIParam, &typeTag);
        retType = GetFfiType(typeTag);

        ctx->closure = ffi_closure_alloc(sizeof(ffi_closure), &proxy->m_pVTbl[mIndex + 3]);
        ffi_prep_cif(&ctx->cif, FFI_STDCALL, pCount + 1, retType, argTypes);
        ffi_prep_closure_loc(ctx->closure, &ctx->cif, EcoJavaProxy_GlobalDispatcher, ctx, proxy->m_pVTbl[mIndex + 3]);

        result = pIMethod->pVTbl->get_Name(pIMethod, &ctx->jniName);
        ctx->jniSig = GenerateJniSignature(pIMethod);
    }

    return proxy;
}


/*
 *
 * <summary>
 *   QueryInterface Function
 * </summary>
 *
 * <description>
 *   QueryInterface function for the IEcoACOM2Java interface
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_QueryInterface(/* in */ IEcoACOM2JavaPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;

    /* Pointer Validation */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Validate and retrieve requested interface */
    if ( IsEqualUGUID(riid, &IID_IEcoACOM2Java) ) {
        *ppv = &pCMe->m_pVTblIEcoACOM2Java;
        pCMe->m_pVTblIEcoACOM2Java->AddRef((IEcoACOM2Java*)pCMe);
    }
	
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoACOM2Java;
        pCMe->m_pVTblIEcoACOM2Java->AddRef((IEcoACOM2Java*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   AddRef Function
 * </summary>
 *
 * <description>
 *   AddRef function for the IEcoACOM2Java interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_AddRef(/* in */ IEcoACOM2JavaPtr_t me) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return atomicincrement_int32_t(&pCMe->m_cRef);
}

/*
 *
 * <summary>
 *   Release Function
 * </summary>
 *
 * <description>
 *   Release function for the IEcoACOM2Java interface
 * </description>
 *
 */
static uint32_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_Release(/* in */ IEcoACOM2JavaPtr_t me) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;

    /* Pointer Validation */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Decrementing the component's reference count */
    atomicdecrement_int32_t(&pCMe->m_cRef);
    /* If the count is zero, free the instance data */
    if ( pCMe->m_cRef == 0 ) {
        pCMe->Delete(pCMe);
		
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <summary>
 *   CreateJavaVM Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_CreateJavaVM(/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ char_t* classpath, /* in */ uint64_t minHeapSize, /* in */ uint64_t maxHeapSize) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    JavaVMInitArgs vm_args;
    void* env = 0;
    jint i = 0;
    jint result = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    vm_args.version = JNI_VERSION_1_8;
    result = JNI_GetDefaultJavaVMInitArgs(&vm_args);
    vm_args.nOptions = 3;
    vm_args.options = (JavaVMOption*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(JavaVMOption) * vm_args.nOptions);
    vm_args.options[0].optionString = (char_t*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, (strlen(classpath) + 19));
    vm_args.options[1].optionString = (char_t*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, 25);
    vm_args.options[2].optionString = (char_t*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, 25);
    sprintf(vm_args.options[0].optionString, "-Djava.class.path=%s", classpath);
    sprintf(vm_args.options[1].optionString, "-Xms%llu", minHeapSize);
    sprintf(vm_args.options[2].optionString, "-Xmx%llu", maxHeapSize);
    result = JNI_CreateJavaVM(&pCMe->m_jvm, &env, &vm_args);
    for (i = 0; i < vm_args.nOptions; i++) {
        pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, vm_args.options[i].optionString);
    }
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, vm_args.options);

    return (int16_t) result;
}

/*
 *
 * <summary>
 *   RegisterComponent Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_RegisterComponent(/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ const UGUID* rcid, /*in*/ char_t* classpath, /*in*/ char_t* factoryClassname) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    JNIEnv* env = 0;
    jclass clazz = 0;
    jmethodID method = 0;
    jobject localObj = 0;
    jobject* obj = 0;
    UGUID* rcidCopy = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    env = GetCurrentEnv(pCMe->m_jvm);
    if (env == 0) {
        return ERR_ECO_FAIL;
    }
    AddClassPath(env, classpath);

    clazz = (*env)->FindClass(env, factoryClassname);
    if (clazz == 0) {
        return ERR_ECO_COMPONENT_NOTFOUND;
    }

    method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    if (method == 0) {
        return ERR_ECO_FAIL;
    }
    localObj = (*env)->NewObject(env, clazz, method);
    if (localObj == 0) {
        return ERR_ECO_FAIL;
    }

    obj = (jobject*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(jobject));
    *obj = (*env)->NewGlobalRef(env, localObj);
    (*env)->DeleteLocalRef(env, localObj);

    rcidCopy = (UGUID*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(UGUID));
    pCMe->m_pIMem->pVTbl->Copy(pCMe->m_pIMem, rcidCopy, (void*)rcid, sizeof(UGUID));

    pCMe->m_componentFactories->pVTbl->Add(pCMe->m_componentFactories, rcidCopy);
    pCMe->m_componentFactories->pVTbl->Add(pCMe->m_componentFactories, obj);

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   UnRegisterComponent Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_UnRegisterComponent(/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ const UGUID* rcid) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    JNIEnv* env = 0;
    IEcoList1* list = 0;
    uint32_t count = 0;
    uint32_t index = 0;
    void* rcidCopy = 0;
    void* objHolder = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    env = GetCurrentEnv(pCMe->m_jvm);
    if (env == 0) {
        return ERR_ECO_FAIL;
    }

    list = pCMe->m_componentFactories;
    count = list->pVTbl->Count(list);
    for (index = 0; index < count; index += 2) {
        if (IsEqualUGUID(list->pVTbl->Item(list, index), rcid)) {
            break;
        }
    }
    if (index >= count) {
        return ERR_ECO_INVALIDARG;
    }

    rcidCopy  = list->pVTbl->Item(list, index);
    objHolder = list->pVTbl->Item(list, index + 1);

    (*env)->DeleteGlobalRef(env, *(jobject*)objHolder);
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, objHolder);
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, rcidCopy);

    list->pVTbl->RemoveAt(list, index);
    list->pVTbl->RemoveAt(list, index);

    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   QueryComponent Function
 * </summary>
 *
 * <description>
 *   Function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_QueryComponent(/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ const UGUID* rcid, /*in*/ IEcoUnknownPtr_t pIUnkOuter, /*in*/ const UGUID* riid, /*out*/ voidptr_t* ppv) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    IEcoInterfaceDescriptor1* pIDesc;
    JNIEnv* env;
    jclass clazz;
    jmethodID method;
    jfieldID field;
    jobject factoryObj;
    jobject obj;
    int16_t result = 0;
    uint32_t count = 0;
    uint32_t index = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    env = GetCurrentEnv(pCMe->m_jvm);
    if (env == 0) {
        return ERR_ECO_FAIL;
    }

    count = pCMe->m_componentFactories->pVTbl->Count(pCMe->m_componentFactories);
    for (index = 0; index < count; index += 2) {
        if (IsEqualUGUID(pCMe->m_componentFactories->pVTbl->Item(pCMe->m_componentFactories, index), rcid)) {
            break;
        }
    }
    if (index >= count) {
        return ERR_ECO_COMPONENT_NOTFOUND;
    }

    clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
    method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
    obj = (*env)->NewObject(env, clazz, method);

    factoryObj = *(jobject*)(pCMe->m_componentFactories->pVTbl->Item(pCMe->m_componentFactories, index + 1));
    clazz = (*env)->GetObjectClass(env, factoryObj);
    method = (*env)->GetMethodID(env, clazz, "Alloc", "(LEco/Core/IEcoUnknown;LEco/Core/IEcoUnknown;LEco/Core/UGUID;LEco/Core/IEcoUnknownPtr;)S");
    result = (*env)->CallShortMethod(env, factoryObj, method, 0, 0, UGUIDPtrToJavaObject(env, riid), obj);
    if (result != 0) {
        return result;
    }

    obj = (*env)->GetObjectField(env, obj, field);
    pIDesc = GetInterfaceDescriptorByUGUID(pCMe->m_pITypeLib, riid);
    *(EcoJavaProxy**)ppv = CreateEcoJavaProxy(env, obj, pIDesc, pCMe->m_pIMem, pCMe->m_pITypeLib);

    return ERR_ECO_SUCCESES;
}




/*
 *
 * <summary>
 *   Init Function
 * </summary>
 *
 * <description>
 *   Instance initialization function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD initCEcoACOM2Java_3F41E2AA(/*in*/ CEcoACOM2Java_3F41E2AAPtr_t me, /* in */ IEcoUnknownPtr_t pIUnkSystem) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    IEcoInterfaceBus1* pIBus = 0;

    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    int16_t result = ERR_ECO_POINTER;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Pointer Validation */
    if (me == 0 ) {
        return result;
    }

    /* Storing the pointer to the system interface */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Getting the interface for working with the interface bus */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Getting the component ID for working with memory */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Getting the memory allocator interface */
    result = pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);
    /* Check */
    if (result != 0 || pCMe->m_pIMem == 0) {
        result = ERR_ECO_GET_MEMORY_ALLOCATOR;
    }

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoTypeLib1, 0, &IID_IEcoTypeLib1, (void**) &pCMe->m_pITypeLib);
    if (result != 0 || pCMe->m_pITypeLib == 0) {
        return result;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_componentFactories);
    if (result != 0 || pCMe->m_componentFactories == 0) {
        return result;
    }

    /* Freeing */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <summary>
 *   Create Function
 * </summary>
 *
 * <description>
 *   Instance creation function
 * </description>
 *
 */
static int16_t ECOCALLMETHOD createCEcoACOM2Java_3F41E2AA(/* in */ CEcoACOM2Java_3F41E2AAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter) {
    int16_t result = ERR_ECO_POINTER;

    /* Pointer Validation */
    if (pCMe == 0) {
        return result; /* ERR_ECO_POINTER */
    }



    return ERR_ECO_SUCCESES;
}

/*
 *
 * <summary>
 *   Delete Function
 * </summary>
 *
 * <description>
 *   Instance freeing function
 * </description>
 *
 */
static void ECOCALLMETHOD deleteCEcoACOM2Java_3F41E2AA(/* in */ CEcoACOM2Java_3F41E2AAPtr_t pCMe) {
    IEcoMemoryAllocator1* pIMem = 0;

    if (pCMe != 0 ) {
        pIMem = pCMe->m_pIMem;
        if (pCMe->m_componentFactories != 0) {
            IEcoList1* list = pCMe->m_componentFactories;
            JNIEnv* env = GetCurrentEnv(pCMe->m_jvm);
            uint32_t count = list->pVTbl->Count(list);
            uint32_t i = 0;
            for (i = 0; i < count; i += 2) {
                void* rcidCopy  = list->pVTbl->Item(list, i);
                void* objHolder = list->pVTbl->Item(list, i + 1);
                if (objHolder != 0) {
                    if (env != 0) {
                        (*env)->DeleteGlobalRef(env, *(jobject*)objHolder);
                    }
                    pIMem->pVTbl->Free(pIMem, objHolder);
                }
                if (rcidCopy != 0) {
                    pIMem->pVTbl->Free(pIMem, rcidCopy);
                }
            }
            list->pVTbl->Clear(list);
            list->pVTbl->Release(list);
        }
        if (pCMe->m_jvm != 0) {
            (*pCMe->m_jvm)->DestroyJavaVM(pCMe->m_jvm);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* IEcoACOM2Java Virtual Table */
IEcoACOM2JavaVTbl g_xED2D1283E26348DCB9A889E10C8C3657VTbl_3F41E2AA = {
    CEcoACOM2Java_3F41E2AA_QueryInterface,
    CEcoACOM2Java_3F41E2AA_AddRef,
    CEcoACOM2Java_3F41E2AA_Release,
    CEcoACOM2Java_3F41E2AA_CreateJavaVM,
    CEcoACOM2Java_3F41E2AA_RegisterComponent,
    CEcoACOM2Java_3F41E2AA_UnRegisterComponent,
    CEcoACOM2Java_3F41E2AA_QueryComponent
};



/* Object Instance */
CEcoACOM2Java_3F41E2AA g_xCEcoACOM2Java_3F41E2AA = {
    &g_xED2D1283E26348DCB9A889E10C8C3657VTbl_3F41E2AA,
   
    initCEcoACOM2Java_3F41E2AA,
    createCEcoACOM2Java_3F41E2AA,
    deleteCEcoACOM2Java_3F41E2AA,
    1, /* m_cRef */
    0, /* m_pISys */
    0, /* m_pISys */
    0,  /* m_pITypeLib */
    0,  /* m_jvm */
    0  /* m_componentFactories */
};
