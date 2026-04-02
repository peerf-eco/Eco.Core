#include "Eco_System_CEcoSystem.h"
#include "Eco_Java2ACOM.h"
#include "CEcoSystem1.h"

JNIEXPORT jint JNICALL Java_Eco_System_CEcoSystem_createCEcoSystem(JNIEnv* env, jclass thisClass, jobjectArray args, jobject iSys) {
    int16_t argc = (*env)->GetArrayLength(env, args);
    char_t** argv = (char_t**)malloc(argc * sizeof(char_t*));
    IEcoSystem1* pISys = 0;
    int16_t result = 0;
    int16_t i = 0;

    for (i = 0; i < argc; i++) {
        jstring arg = (jstring)(*env)->GetObjectArrayElement(env, args, i);
        getCharArray(env, arg, argv + i);
    }
    result = createCEcoSystem1_00000100(argc, argv, &pISys);
    if (result != 0) {
        return result;
    }
    setPointerToInterface(env, iSys, pISys);
    return result;
}

JNIEXPORT void JNICALL Java_Eco_System_CEcoSystem_deleteCEcoSystem(JNIEnv* env, jclass thisClass, jobject iSys) {
    deleteCEcoSystem1_00000100((IEcoSystem1*)getPointerToInterface(env, iSys));
}
