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
 *   This header describes the implementation of the CEcoACOM2Java_3F41E2AA component
 * </description>
 *
 * <author>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </author>
 *
 */

#ifndef __C_ECOACOM2JAVA_H__
#define __C_ECOACOM2JAVA_H__

#include "IEcoACOM2Java.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoList1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IEcoTypeLib1.h"
#include "IdEcoTypeLib1.h"
#include "ffi_cdecl.h"
#include <jni.h>

typedef struct CEcoACOM2Java_3F41E2AA* CEcoACOM2Java_3F41E2AAPtr_t;

typedef struct CEcoACOM2Java_3F41E2AA {

    /* IEcoACOM2Java interface function table */
    IEcoACOM2JavaVTbl* m_pVTblIEcoACOM2Java;


    /* Instance initialization */
    int16_t (ECOCALLMETHOD *Init)(/*in*/ CEcoACOM2Java_3F41E2AAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem);
    /* Instance creation */
    int16_t (ECOCALLMETHOD *Create)(/*in*/ CEcoACOM2Java_3F41E2AAPtr_t pCMe, /* in */ IEcoUnknownPtr_t pIUnkSystem, /* in */ IEcoUnknownPtr_t pIUnkOuter);
    /* Deletion */
    void (ECOCALLMETHOD *Delete)(/*in*/ CEcoACOM2Java_3F41E2AAPtr_t pCMe);


    /* Reference counter */
    uint32_t m_cRef;

    /* Interface for memory operations */
    IEcoMemoryAllocator1* m_pIMem;

    /* System interface */
    IEcoSystem1* m_pISys;

    /* Instance data */
    IEcoTypeLib1* m_pITypeLib;
    JavaVM* m_jvm;
    IEcoList1* m_componentFactories;

} CEcoACOM2Java_3F41E2AA;


typedef struct MethodContext {
    struct EcoJavaProxy* proxy;
    uint16_t methodIndex;
    IEcoMethodDescriptor1* methodDesc;
    ffi_closure* closure;
    ffi_cif cif;
    char* jniName;
    char* jniSig;
    jmethodID methodId;
} MethodContext;

struct EcoJavaProxyGroup;

typedef struct EcoJavaProxy {
    void** m_pVTbl;
    uint32_t m_cRef;
    IEcoMemoryAllocator1* m_pIMem;
    IEcoTypeLib1* m_pITypeLib;
    IEcoInterfaceDescriptor1* m_pIDesc;
    JavaVM* m_jvm;
    jobject m_obj;
    MethodContext* m_methods;
    UGUID m_iid;
    struct EcoJavaProxyGroup* m_group;
} EcoJavaProxy;

typedef struct EcoJavaProxyGroup {
    EcoJavaProxy** proxies;
    uint32_t count;
    IEcoMemoryAllocator1* pIMem;
} EcoJavaProxyGroup;

typedef struct EcoTypeMap {
    bool_t isPrimitive;
    const char_t* jniSignature;
    const char_t* jniClassName;
} EcoTypeMap;

#endif /* __C_ECOACOM2JAVA_H__ */
