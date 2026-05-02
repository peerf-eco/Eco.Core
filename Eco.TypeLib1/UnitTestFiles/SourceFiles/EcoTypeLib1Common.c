/*
 * <character encoding>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </character encoding>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "IEcoBase1.h"
#include "IEcoTypeLib1.h"
#include "IEcoTypeLib1Builder.h"
#include "IEcoInterfaceDirectory1.h"
#include "IEcoInterfaceDirectoryEntry1.h"
#include "IEcoInterfaceDescriptor1.h"
#include "IEcoMethodDescriptor1.h"
#include "IEcoParamDescriptor1.h"

#include "EcoTypeLib1Common.h"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_BOLD    "\x1b[1m"

const char_t* ECO_TYPE_NAME[] = {
    "undefined", /* ECO_TYPE_UNDEFINED */
    "int8",      /* ECO_TYPE_INT8 */
    "int16",     /* ECO_TYPE_INT16 */
    "int32",     /* ECO_TYPE_INT32 */
    "int64",     /* ECO_TYPE_INT64 */
    "uint8",     /* ECO_TYPE_UINT8 */
    "uint16",    /* ECO_TYPE_UINT16 */
    "uint32",    /* ECO_TYPE_UINT32 */
    "uint64",    /* ECO_TYPE_UINT64 */
    "float",     /* ECO_TYPE_FLOAT */
    "double",    /* ECO_TYPE_DOUBLE */
    "bool",      /* ECO_TYPE_BOOLEAN */
    "char",      /* ECO_TYPE_CHAR */
    "wchar",     /* ECO_TYPE_WCHAR */
    "astring",   /* ECO_TYPE_ASTRING */
    "wstring",   /* ECO_TYPE_WSTRING */
    "interface", /* ECO_TYPE_INTERFACE */
    "uguid",     /* ECO_TYPE_UGUID */
    "voidptr",   /* ECO_TYPE_VOIDPTR */
    "void"       /* ECO_TYPE_VOID */
};

char_t* UGUIDPtrToTypeLibFileName(const UGUID* uguid) {
    char_t result[256] = "";
    byte_t i;
    byte_t b;

    for (i = 0; i < uguid->Length; i++) {
        b = uguid->Data[i] >> 4;
        result[i * 2] = (b < 10) ? (b + '0') : ((b - 10) + 'A');

        b = uguid->Data[i] & 0xF;
        result[i * 2 + 1] = (b < 10) ? (b + '0') : ((b - 10) + 'A');
    }

    strcat(result, ".etl");
    return _strdup(result);
}

void LoadAndInspect(IEcoTypeLib1* pTypeLib, const UGUID* riid) {
    IEcoInterfaceDirectory1* pDir = NULL;
    IEcoInterfaceDirectoryEntry1* pEntry = NULL;
    IEcoInterfaceDescriptor1* pDesc = NULL;
    IEcoMethodDescriptor1* pMethod = NULL;
    IEcoParamDescriptor1* pParam = NULL;

    char_t* fileName = NULL;
    char_t* name = NULL;

    uint16_t typeTag = 0;
    uint8_t flags = 0;
    uint16_t mCount = 0;
    uint8_t pCount = 0;
    uint16_t mIndex = 0;
    uint16_t pIndex = 0;

    fileName = UGUIDPtrToTypeLibFileName(riid);
    if (pTypeLib->pVTbl->LoadFile(pTypeLib, fileName, &pDir) == 0) {
        if (pDir->pVTbl->GetEntryByIID(pDir, riid, &pEntry) == 0) {
            pEntry->pVTbl->get_Name(pEntry, &name);

            printf(COLOR_BOLD COLOR_CYAN "\nInterface: '%s'" COLOR_RESET "\n", name);

            pEntry->pVTbl->get_Descriptor(pEntry, &pDesc);
            mCount = pDesc->pVTbl->get_MethodCount(pDesc);

            for (mIndex = 0; mIndex < mCount; mIndex++) {
                pDesc->pVTbl->get_MethodAtIndex(pDesc, mIndex, &pMethod);
                pMethod->pVTbl->get_Name(pMethod, &name);
                pMethod->pVTbl->get_Result(pMethod, &pParam);
                pParam->pVTbl->get_Type(pParam, &typeTag);
                pCount = pMethod->pVTbl->get_ParamCount(pMethod);

                printf("  " COLOR_GREEN "Method:" COLOR_RESET
                       " [" COLOR_BOLD "%s" COLOR_RESET "] ("
                       COLOR_YELLOW "%s" COLOR_RESET ")\n",
                       name, ECO_TYPE_NAME[typeTag]);

                for (pIndex = 0; pIndex < pCount; pIndex++) {
                    pMethod->pVTbl->GetParamAtIndex(pMethod, pIndex, &pParam);
                    pParam->pVTbl->get_Name(pParam, &name);
                    pParam->pVTbl->get_Type(pParam, &typeTag);
                    flags = pParam->pVTbl->get_Flags(pParam);

                    printf("    " COLOR_MAGENTA "Param:" COLOR_RESET
                           " <" COLOR_BOLD "%s" COLOR_RESET "> ("
                           COLOR_YELLOW "%s" COLOR_RESET ", "
                           COLOR_BLUE "0x%.2X" COLOR_RESET ")\n",
                           name, ECO_TYPE_NAME[typeTag], flags);

                    pParam->pVTbl->Release(pParam);
                }
                pMethod->pVTbl->Release(pMethod);
            }
            pDesc->pVTbl->Release(pDesc);
            pEntry->pVTbl->Release(pEntry);
            printf("\n");
        }
        pDir->pVTbl->Release(pDir);
    }
    free(fileName);
}
