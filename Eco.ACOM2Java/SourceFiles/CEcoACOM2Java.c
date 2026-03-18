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

static void addClassPath(JNIEnv* env, char_t* classpath) {
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



static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_QueryInterface(IEcoUnknownPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env;
    jclass clazz;
    jclass pIUnkClazz;
    jmethodID method;
    jfieldID field;
    jobject obj;
    int16_t result = 0;

    env = proxy->m_env;

    pIUnkClazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
    method = (*env)->GetMethodID(env, pIUnkClazz, "<init>", "()V");
    obj = (*env)->NewObject(env, pIUnkClazz, method);

    clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    method = (*env)->GetMethodID(env, clazz, "QueryInterface", "(LEco/Core/UGUID;LEco/Core/IEcoUnknownPtr;)I");

    result = (int16_t) (*env)->CallIntMethod(env, proxy->m_obj, method, getUGUIDObj(env, riid), obj);
    if (result != 0) {
        return result;
    }

    field = (*env)->GetFieldID(env, pIUnkClazz, "iUnk", "LEco/Core/IEcoUnknown;");
    obj = (*env)->GetObjectField(env, obj, field);

    return createProxyForInterface(env, riid, obj, proxy->m_pIMem, ppv);
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_AddRef(IEcoUnknownPtr_t me) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "AddRef", "()I");
    uint32_t result = (uint32_t) (*env)->CallIntMethod(env, proxy->m_obj, method);
    if (result != 0) {
        return result;
    }

    ++proxy->m_cRef;
    return result;
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoUnknown_Release(IEcoUnknownPtr_t me) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "Release", "()I");
    uint32_t result = (uint32_t) (*env)->CallIntMethod(env, proxy->m_obj, method);
    if (result != 0) {
        return result;
    }

    if (--proxy->m_cRef == 0) {
        (*env)->DeleteGlobalRef(env, proxy->m_obj);
        proxy->m_pIMem->pVTbl->Free(proxy->m_pIMem, proxy);
    }
    return result;
}


static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorX_QueryInterface(IEcoCalculatorXPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    return EcoJavaProxy_IEcoUnknown_QueryInterface((IEcoUnknownPtr_t)me, riid, ppv);
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorX_AddRef(IEcoCalculatorXPtr_t me) {
    return EcoJavaProxy_IEcoUnknown_AddRef((IEcoUnknownPtr_t)me);
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorX_Release(IEcoCalculatorXPtr_t me) {
    return EcoJavaProxy_IEcoUnknown_Release((IEcoUnknownPtr_t)me);
}

static int32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorX_Addition(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "Addition", "(II)I");
    return (int32_t) (*env)->CallIntMethod(env, proxy->m_obj, method, a, b);
}

static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorX_Subtraction(IEcoCalculatorXPtr_t me, int16_t a, int16_t b) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "Subtraction", "(II)I");
    return (int16_t) (*env)->CallIntMethod(env, proxy->m_obj, method, a, b);
}


static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorY_QueryInterface(IEcoCalculatorYPtr_t me, const UGUID* riid, voidptr_t* ppv) {
    return EcoJavaProxy_IEcoUnknown_QueryInterface((IEcoUnknownPtr_t)me, riid, ppv);
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorY_AddRef(IEcoCalculatorYPtr_t me) {
    return EcoJavaProxy_IEcoUnknown_AddRef((IEcoUnknownPtr_t)me);
}

static uint32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorY_Release(IEcoCalculatorYPtr_t me) {
    return EcoJavaProxy_IEcoUnknown_Release((IEcoUnknownPtr_t)me);
}

static int32_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorY_Multiplication(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "Multiplication", "(II)I");
    return (int32_t) (*env)->CallIntMethod(env, proxy->m_obj, method, a, b);
}

static int16_t ECOCALLMETHOD EcoJavaProxy_IEcoCalculatorY_Division(IEcoCalculatorYPtr_t me, int16_t a, int16_t b) {
    EcoJavaProxy* proxy = (EcoJavaProxy*)me;
    JNIEnv* env = proxy->m_env;
    jclass clazz = (*env)->GetObjectClass(env, proxy->m_obj);
    jmethodID method = (*env)->GetMethodID(env, clazz, "Division", "(II)I");
    return (int16_t) (*env)->CallIntMethod(env, proxy->m_obj, method, a, b);
}


static IEcoUnknownVTbl g_EcoJavaProxy_IEcoUnknown = {
    EcoJavaProxy_IEcoUnknown_QueryInterface,
    EcoJavaProxy_IEcoUnknown_AddRef,
    EcoJavaProxy_IEcoUnknown_Release
};

static IEcoCalculatorXVTbl g_EcoJavaProxy_IEcoCalculatorX = {
    EcoJavaProxy_IEcoCalculatorX_QueryInterface,
    EcoJavaProxy_IEcoCalculatorX_AddRef,
    EcoJavaProxy_IEcoCalculatorX_Release,
    EcoJavaProxy_IEcoCalculatorX_Addition,
    EcoJavaProxy_IEcoCalculatorX_Subtraction
};

static IEcoCalculatorXVTbl g_EcoJavaProxy_IEcoCalculatorY = {
    EcoJavaProxy_IEcoCalculatorY_QueryInterface,
    EcoJavaProxy_IEcoCalculatorY_AddRef,
    EcoJavaProxy_IEcoCalculatorY_Release,
    EcoJavaProxy_IEcoCalculatorY_Multiplication,
    EcoJavaProxy_IEcoCalculatorY_Division
};



static jobject getUGUIDObj(JNIEnv* env, const UGUID* uguid) {
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

static int16_t createProxyForInterface(JNIEnv* env, const UGUID* riid, jobject obj, IEcoMemoryAllocator1* pIMem, voidptr_t* ppv) {
    EcoJavaProxy* proxy = 0;
    if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        proxy = (EcoJavaProxy*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoJavaProxy));
        if (proxy == 0) return ERR_ECO_OUTOFMEMORY;
        proxy->pVTbl = (void*) &g_EcoJavaProxy_IEcoUnknown;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        proxy = (EcoJavaProxy*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoJavaProxy));
        if (proxy == 0) return ERR_ECO_OUTOFMEMORY;
        proxy->pVTbl = (void*) &g_EcoJavaProxy_IEcoCalculatorX;
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        proxy = (EcoJavaProxy*) pIMem->pVTbl->Alloc(pIMem, sizeof(EcoJavaProxy));
        if (proxy == 0) return ERR_ECO_OUTOFMEMORY;
        proxy->pVTbl = (void*) &g_EcoJavaProxy_IEcoCalculatorY;
    } else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    proxy->m_cRef = 1;
    proxy->m_pIMem = pIMem;
    proxy->m_env = env;
    proxy->m_obj = (*env)->NewGlobalRef(env, obj);
    *ppv = proxy;
    return ERR_ECO_SUCCESES;
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
static int16_t ECOCALLMETHOD CEcoACOM2Java_3F41E2AA_RegisterComponent(/*in*/ IEcoACOM2JavaPtr_t me, /*in*/ char_t* classpath, /* in */ char_t* classname, /*in*/ const UGUID* rcid) {
    CEcoACOM2Java_3F41E2AA* pCMe = (CEcoACOM2Java_3F41E2AA*)me;
    JNIEnv* env;
    jclass* clazz;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    env = pCMe->m_env;
    addClassPath(env, classpath);

    clazz = (jclass*) malloc(sizeof(jclass));
    *clazz = (*env)->FindClass(env, classname);
    if (*clazz == 0) {
        free(clazz);
        return ERR_ECO_COMPONENT_NOTFOUND;
    }

    pCMe->m_components->pVTbl->Add(pCMe->m_components, (void*) rcid);
    pCMe->m_components->pVTbl->Add(pCMe->m_components, (void*) clazz);

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
    IEcoList1* list;
    uint32_t index = 0;
    void* item = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    list = pCMe->m_components;
    index = list->pVTbl->IndexOf(list, rcid);
    if (index == -1) {
        return ERR_ECO_INVALIDARG;
    }

    list->pVTbl->RemoveAt(list, index);
    item = list->pVTbl->Item(list, index);
    free(item);
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
    JNIEnv* env;
    jclass clazz;
    jmethodID method;
    jfieldID field;
    jobject componentObj;
    jobject pIUnkObj;
    jobject iUnkObj;
    int16_t result = 0;
    uint32_t index = 0;

    if (me == 0) {
        return ERR_ECO_POINTER;
    }

    env = pCMe->m_env;
    index = pCMe->m_components->pVTbl->IndexOf(pCMe->m_components, (void*) rcid);
    if (index == -1) {
        return ERR_ECO_COMPONENT_NOTFOUND;
    }

    clazz = *(jclass*)(pCMe->m_components->pVTbl->Item(pCMe->m_components, index + 1));
    method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    componentObj = (*env)->NewObject(env, clazz, method);

    clazz = (*env)->FindClass(env, "Eco/Core/IEcoUnknownPtr");
    method = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    pIUnkObj = (*env)->NewObject(env, clazz, method);

    clazz = (*env)->GetObjectClass(env, componentObj);
    method = (*env)->GetMethodID(env, clazz, "create", "(LEco/Core/IEcoUnknownPtr;)I");
    result = (int16_t) (*env)->CallIntMethod(env, componentObj, method, pIUnkObj);
    if (result != 0) {
        return result;
    }

    clazz = (*env)->GetObjectClass(env, pIUnkObj);
    field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
    iUnkObj = (*env)->GetObjectField(env, pIUnkObj, field);

    clazz = (*env)->GetObjectClass(env, iUnkObj);
    method = (*env)->GetMethodID(env, clazz, "QueryInterface", "(LEco/Core/UGUID;LEco/Core/IEcoUnknownPtr;)I");
    result = (int16_t) (*env)->CallIntMethod(env, iUnkObj, method, getUGUIDObj(env, riid), pIUnkObj);
    if (result != 0) {
        return result;
    }

    clazz = (*env)->GetObjectClass(env, pIUnkObj);
    field = (*env)->GetFieldID(env, clazz, "iUnk", "LEco/Core/IEcoUnknown;");
    iUnkObj = (*env)->GetObjectField(env, pIUnkObj, field);

    return createProxyForInterface(env, riid, iUnkObj, pCMe->m_pIMem, ppv);
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

    JavaVMInitArgs vm_args;

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

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoList1, 0, &IID_IEcoList1, (void**) &pCMe->m_components);
    if (result != 0 || pCMe->m_components == 0) {
        return result;
    }

    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = (JavaVMOption*) malloc(sizeof(JavaVMOption));
    vm_args.options[0].optionString = "-Djava.class.path=C:\\Programming\\Eco.Core\\Eco.Java2ACOM\\BuildFiles\\artifacts\\Eco_Java2ACOM_jar\\Eco.Java2ACOM.jar";
    vm_args.ignoreUnrecognized = 0;
    result = (int16_t)JNI_CreateJavaVM(&pCMe->m_jvm, (void**) &pCMe->m_env, &vm_args);
    free(vm_args.options);

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
        /* Freeing */
        if (pCMe->m_jvm != 0) {
            (*pCMe->m_jvm)->DestroyJavaVM(pCMe->m_jvm);
        }
        if (pCMe->m_components != 0) {
            pCMe->m_components->pVTbl->Clear(pCMe->m_components);
            pCMe->m_components->pVTbl->Release(pCMe->m_components);
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
    0,  /* m_jvm */
    0,  /* m_env */
    0  /* m_components */
};
