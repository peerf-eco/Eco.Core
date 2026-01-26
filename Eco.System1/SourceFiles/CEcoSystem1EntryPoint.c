/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoSystem1EntryPoint
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoSystem1EntryPoint
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoSystem1.h"

extern int16_t ECOCALLMETHOD EcoMain(IEcoUnknown* pISys);

/* ECO_OS */
#ifdef ECO_AVR8

int main(int argc, char **argv) {
    int result = 0;
    int tmpFlag = 0;
    IEcoSystem1* pISys = 0;

    result = createCEcoSystem1_00000100(argc, argv, &pISys);

    result = EcoMain((IEcoUnknown*)pISys);

    deleteCEcoSystem1_00000100(pISys);
    pISys = 0;

	return result;
}

#endif /* ECO_AVR8 */

#ifdef ECO_STM32

int main(int argc, char **argv) {
    int result = 0;
    int tmpFlag = 0;
    IEcoSystem1* pISys = 0;

    result = createCEcoSystem1_00000100(argc, argv, &pISys);

    result = EcoMain((IEcoUnknown*)pISys);

    deleteCEcoSystem1_00000100(pISys);
    pISys = 0;

	return result;
}

#endif /* ECO_STM32 */

#ifdef ECO_BCM283X
int main() {
    int result = 0;
    int tmpFlag = 0;
    IEcoSystem1* pISys = 0;

    result = createCEcoSystem1_00000100(0, 0, &pISys);

    result = EcoMain((IEcoUnknown*)pISys);

    deleteCEcoSystem1_00000100(pISys);
    pISys = 0;

	return result;
}
#endif

/* Win32 */
#ifdef ECO_WINDOWS
//#include "windows.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _DEBUG
#include <CRTDBG.H>
#endif
int _cdecl main(int argc, char **argv) {
    int16_t result = 0;
    uint32_t countBus = 0;
    uint32_t countSys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryManager1* pIMemMgr = 0;
    int tmpFlag = 0;
    IEcoSystem1* pISys = 0;
    ECOMEMORYMANAGER1STATUS status = {0};

#ifdef _DEBUG
    /* Настройка отладочной информации */
    tmpFlag = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetDbgFlag(tmpFlag);

    _CrtMemDumpAllObjectsSince(NULL);

#endif

    /* Создание системного интерфейса приложения */
    result = createCEcoSystem1_00000100(argc, argv, &pISys);
    if (result != 0 || pISys == 0) {
        /* Освобождение в случае ошибки */
        goto Exit;
    }
#ifdef _DEBUG
    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Exit;
    }
    countBus = pIBus->pVTbl->AddRef(pIBus);
    countSys = pISys->pVTbl->AddRef(pISys);

    /* Получение интерфейса управления памятью */
    pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryManager1, (void**) &pIMemMgr);
    if (result != 0 || pIMemMgr == 0) {
        /* Возврат в случае ошибки */
        goto Exit;
    }
    pIMemMgr->pVTbl->get_Status(pIMemMgr, &status);
    _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, "Memory Status :\r\n Total Memory - %d\r\n Free Memory - %d\r\n Used Blocks - %d\r\n Bus Reference Count - %d\r\n Sys Reference Count - %d\r\n", status.totalSize, status.freeSize, status.usedBlocks, countBus, countSys);
#endif

    /* Передача управления точке входа */
    result = EcoMain((IEcoUnknown*)pISys);

#ifdef _DEBUG


    countBus = pIBus->pVTbl->Release(pIBus);
    countSys = pISys->pVTbl->Release(pISys);
    countBus = pIMemMgr->pVTbl->Release(pIMemMgr);
    countBus = pIBus->pVTbl->Release(pIBus);

#endif


Exit:
    /* Освобождение системного интерфейса */
    deleteCEcoSystem1_00000100(pISys);
    /* или  pISys->pVTbl->Release(pISys); */
    //pISys = 0;

#ifdef _DEBUG
    if (pIMemMgr != 0) {
        pIMemMgr->pVTbl->get_Status(pIMemMgr, &status);
    }
    _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, "Memory Status :\r\n Total Memory - %d\r\n Free Memory - %d\r\n Used Blocks - %d\r\n Bus Reference Count - %d (must 1)\r\n Sys Reference Count - %d (must 1)\r\n", status.totalSize, status.freeSize, status.usedBlocks, countBus, countSys);
    _CrtMemDumpAllObjectsSince(NULL);
#endif
    return result;
}

#endif /* ECO_WINDOWS */

/* Mac OS */
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>
#endif /* __APPLE__ */

#ifdef __APPLE__

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE

int main(int argc, char **argv) {
    int result = 0;
    IEcoSystem1* pISys = 0;

    /* Создание системного интерфейса приложения */
    result = createCEcoSystem1_00000100(argc, argv, &pISys);

    /* Передача управления точке входа */
    result = EcoMain((IEcoUnknown*)pISys);

    /* Освобождение системного интерфейса */
    pISys->pVTbl->Release(pISys);
    pISys = 0;

    return result;
}

#elif TARGET_OS_MAC

#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int result = 0;
    IEcoSystem1* pISys = 0;

    /* Создание системного интерфейса приложения */
    result = createCEcoSystem1_00000100(argc, argv, &pISys);

    /* Передача управления точке входа */
    result = EcoMain((IEcoUnknown*)pISys);

    /* Освобождение системного интерфейса */
    pISys->pVTbl->Release(pISys);
    pISys = 0;

    return result;
}
#endif

#endif

#ifdef LINUX
#define int8_t no_char
#define wchar_t no_short
#define int64_t no_int

#include <stdlib.h>

int main(int argc, char **argv) {
    int result = 0;
    IEcoSystem1* pISys = 0;

    /* Создание системного интерфейса приложения */
    result = createCEcoSystem1_00000100(argc, argv, &pISys);

    /* Передача управления точке входа */
    result = EcoMain((IEcoUnknown*)pISys);

    /* Освобождение системного интерфейса */
    pISys->pVTbl->Release(pISys);
    pISys = 0;

    return result;
}
#endif
#ifdef __ANDROID__

#include <stdlib.h>
#include <jni.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android/sensor.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "eco_system1", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "eco_system1", __VA_ARGS__))

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "eco_system1", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif


static void CEcoSystem1_00000100_free_saved_state(struct CEcoSystem1_00000100* pCMe) {
    pthread_mutex_lock(&pCMe->m_xMutex);
    if (pCMe->m_pSavedState != NULL) {
        free(pCMe->m_pSavedState);
        pCMe->m_pSavedState = NULL;
        pCMe->m_iSavedStateSize = 0;
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);
}

int8_t CEcoSystem1_00000100_android_app_read_cmd(struct CEcoSystem1_00000100* pCMe) {
    int8_t cmd;
    if (read(pCMe->m_iMsgRead, &cmd, sizeof(cmd)) == sizeof(cmd)) {
        switch (cmd) {
            case APP_CMD_SAVE_STATE:
                CEcoSystem1_00000100_free_saved_state(pCMe);
                break;
        }
        return cmd;
    } else {
     //   LOGE("No data on command pipe!");
    }
    return -1;
}



void CEcoSystem1_00000100_android_app_pre_exec_cmd(struct CEcoSystem1_00000100* pCMe, int8_t cmd) {
    static int32_t  format = WINDOW_FORMAT_RGBX_8888;
    switch (cmd) {
        case APP_CMD_INPUT_CHANGED:
           LOGV("APP_CMD_INPUT_CHANGED\n");
            pthread_mutex_lock(&pCMe->m_xMutex);
            if (pCMe->m_pInputQueue != NULL) {
                AInputQueue_detachLooper(pCMe->m_pInputQueue);
            }
            pCMe->m_pInputQueue = pCMe->m_pPendingInputQueue;
            if (pCMe->m_pInputQueue != NULL) {
                LOGI("Attaching input queue to looper");
                AInputQueue_attachLooper(pCMe->m_pInputQueue,
                        pCMe->m_pLooper, LOOPER_ID_INPUT, NULL,
                        &pCMe->m_xInputPollSource);
            }
            pthread_cond_broadcast(&pCMe->m_xCond);
            pthread_mutex_unlock(&pCMe->m_xMutex);
            break;

        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW\n");
            pthread_mutex_lock(&pCMe->m_xMutex);
            pCMe->m_pWindow = pCMe->m_pPendingWindow;
                ANativeWindow_setBuffersGeometry(pCMe->m_pWindow,
                                                 ANativeWindow_getWidth(pCMe->m_pWindow),
                                                 ANativeWindow_getHeight(pCMe->m_pWindow),
                                                 format
                );
              //  drawSomething( android_app) ;
            pthread_cond_broadcast(&pCMe->m_xCond);
            pthread_mutex_unlock(&pCMe->m_xMutex);
            break;

        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW\n");
            pthread_cond_broadcast(&pCMe->m_xCond);
            break;

        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            LOGI("activityState=%d\n", cmd);
            pthread_mutex_lock(&pCMe->m_xMutex);
            pCMe->m_iActivityState = cmd;
            pthread_cond_broadcast(&pCMe->m_xCond);
            pthread_mutex_unlock(&pCMe->m_xMutex);
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGI("APP_CMD_CONFIG_CHANGED\n");
            AConfiguration_fromAssetManager(pCMe->m_pConfig,
                    pCMe->m_pActivity->assetManager);
            //print_cur_config(android_app);
            break;

        case APP_CMD_DESTROY:
            LOGI("APP_CMD_DESTROY\n");
            pCMe->m_iDestroyRequested = 1;
            break;
    }
}

void CEcoSystem1_00000100_android_app_post_exec_cmd(struct CEcoSystem1_00000100* pCMe, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW\n");
            pthread_mutex_lock(&pCMe->m_xMutex);
            pCMe->m_pWindow = NULL;
            pthread_cond_broadcast(&pCMe->m_xCond);
            pthread_mutex_unlock(&pCMe->m_xMutex);
            break;

        case APP_CMD_SAVE_STATE:
            LOGI("APP_CMD_SAVE_STATE\n");
            pthread_mutex_lock(&pCMe->m_xMutex);
            pCMe->m_iStateSaved = 1;
            pthread_cond_broadcast(&pCMe->m_xCond);
            pthread_mutex_unlock(&pCMe->m_xMutex);
            break;

        case APP_CMD_RESUME:
            CEcoSystem1_00000100_free_saved_state(pCMe);
            break;
    }
}

//void app_dummy() {
//
//}

static void CEcoSystem1_00000100_android_app_destroy(struct CEcoSystem1_00000100* pCMe) {
    LOGI("android_app_destroy!");
    CEcoSystem1_00000100_free_saved_state(pCMe);
    pthread_mutex_lock(&pCMe->m_xMutex);
    if (pCMe->m_pInputQueue != NULL) {
        AInputQueue_detachLooper(pCMe->m_pInputQueue);
    }
    AConfiguration_delete(pCMe->m_pConfig);
    pCMe->m_iDestroyed = 1;
    pthread_cond_broadcast(&pCMe->m_xCond);
    pthread_mutex_unlock(&pCMe->m_xMutex);
    // Can't touch android_app object after this.
}

static void CEcoSystem1_00000100_process_input(struct CEcoSystem1_00000100* pCMe, struct CEcoSystem1_00000100_android_poll_source* source) {
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(pCMe->m_pInputQueue, &event) >= 0) {
        LOGI("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(pCMe->m_pInputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        if (pCMe->OnInputEvent != NULL) handled = pCMe->OnInputEvent(pCMe, event);
        AInputQueue_finishEvent(pCMe->m_pInputQueue, event, handled);
    }
}

static void CEcoSystem1_00000100_process_cmd(struct CEcoSystem1_00000100* pCMe, struct CEcoSystem1_00000100_android_poll_source* source) {
    int8_t cmd = CEcoSystem1_00000100_android_app_read_cmd(pCMe);
    CEcoSystem1_00000100_android_app_pre_exec_cmd(pCMe, cmd);
    if (pCMe->OnAppCmd != NULL) pCMe->OnAppCmd(pCMe, cmd);
    CEcoSystem1_00000100_android_app_post_exec_cmd(pCMe, cmd);
}

static void* CEcoSystem1_00000100_android_app_entry(void* param) {
    int result = 0;
    IEcoSystem1* pISys = 0;
    CEcoSystem1_00000100* pCMe = (CEcoSystem1_00000100*)param;
    LOGI("android_app_entry\n");
    pCMe->m_pConfig = AConfiguration_new();
    AConfiguration_fromAssetManager(pCMe->m_pConfig, pCMe->m_pActivity->assetManager);

    //print_cur_config(pCMe->app);

    pCMe->m_xCmdPollSource.id = LOOPER_ID_MAIN;
    pCMe->m_xCmdPollSource.app = pCMe;
    pCMe->m_xCmdPollSource.process = CEcoSystem1_00000100_process_cmd;
    pCMe->m_xInputPollSource.id = LOOPER_ID_INPUT;
    pCMe->m_xInputPollSource.app = pCMe;
    pCMe->m_xInputPollSource.process = CEcoSystem1_00000100_process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, pCMe->m_iMsgRead, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
            &pCMe->m_xCmdPollSource);
    pCMe->m_pLooper = looper;

    pthread_mutex_lock(&pCMe->m_xMutex);
    pCMe->m_iRunning = 1;
    pthread_cond_broadcast(&pCMe->m_xCond);
    pthread_mutex_unlock(&pCMe->m_xMutex);

        // Read all pending events.
    int ident;
    int events;
    struct CEcoSystem1_00000100_android_poll_source* source;
    while(pCMe->m_pWindow == 0x0) {
        LOGI("**************while(android_app->window == 0x0) *******************\n");
        while ((ident = ALooper_pollAll(0, NULL, &events, (void **) &source)) >= 0) {
            // Process this event.
            if (source != NULL) {
                source->process(pCMe, source);
            }

            // Check if we are exiting.
            if (pCMe->m_iDestroyRequested != 0) {
               
                //return;
            }
        }
    }


    /* Передача управления точке входа */
    result = EcoMain((IEcoUnknown*)pCMe);

    LOGI("**************exit android_app_entry *******************\n");
    CEcoSystem1_00000100_android_app_destroy(pCMe);
    return NULL;
}




static struct CEcoSystem1_00000100* CEcoSystem1_00000100_android_app_create(CEcoSystem1_00000100* pCMe, ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    //pCMe->app = (struct android_app*)malloc(sizeof(struct android_app));
    //memset(pCMe->app, 0, sizeof(struct android_app));
    //pCMe->app->activity = activity;

    pthread_mutex_init(&pCMe->m_xMutex, NULL);
    pthread_cond_init(&pCMe->m_xCond, NULL);

    if (savedState != NULL) {
        pCMe->m_pSavedState = malloc(savedStateSize);
        pCMe->m_iSavedStateSize = savedStateSize;
        memcpy(pCMe->m_pSavedState, savedState, savedStateSize);
    }

    int msgpipe[2];
    if (pipe(msgpipe)) {
        LOGI("could not create pipe: %s", strerror(errno));
        return NULL;
    }
    pCMe->m_iMsgRead = msgpipe[0];
    pCMe->m_iMsgWrite = msgpipe[1];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&pCMe->m_Thread, &attr, CEcoSystem1_00000100_android_app_entry, pCMe);

    // Wait for thread to start.
    pthread_mutex_lock(&pCMe->m_xMutex);
    while (!pCMe->m_iRunning) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);

    return pCMe;
}



static void CEcoSystem1_00000100_android_app_write_cmd(struct CEcoSystem1_00000100* pCMe, int8_t cmd) {
    if (write(pCMe->m_iMsgWrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGI("Failure writing android_app cmd: %s\n", strerror(errno));
    }
}

static void CEcoSystem1_00000100_android_app_set_input(struct CEcoSystem1_00000100* pCMe, AInputQueue* inputQueue) {
    pthread_mutex_lock(&pCMe->m_xMutex);
    pCMe->m_pPendingInputQueue = inputQueue;
    CEcoSystem1_00000100_android_app_write_cmd(pCMe, APP_CMD_INPUT_CHANGED);
    while (pCMe->m_pInputQueue != pCMe->m_pPendingInputQueue) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);
}

static void CEcoSystem1_00000100_android_app_set_window(struct CEcoSystem1_00000100* pCMe, ANativeWindow* window) {
    pthread_mutex_lock(&pCMe->m_xMutex);
    if (pCMe->m_pPendingWindow != NULL) {
        CEcoSystem1_00000100_android_app_write_cmd(pCMe, APP_CMD_TERM_WINDOW);
    }
    pCMe->m_pPendingWindow = window;
    if (window != NULL) {
        CEcoSystem1_00000100_android_app_write_cmd(pCMe, APP_CMD_INIT_WINDOW);
    }
    while (pCMe->m_pWindow != pCMe->m_pPendingWindow) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);
}

static void CEcoSystem1_00000100_android_app_set_activity_state(struct CEcoSystem1_00000100* pCMe, int8_t cmd) {
    pthread_mutex_lock(&pCMe->m_xMutex);
    CEcoSystem1_00000100_android_app_write_cmd(pCMe, cmd);
    while (pCMe->m_iActivityState != cmd) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);
}

static void CEcoSystem1_00000100_android_app_free(struct CEcoSystem1_00000100* pCMe) {
    pthread_mutex_lock(&pCMe->m_xMutex);
    CEcoSystem1_00000100_android_app_write_cmd(pCMe, APP_CMD_DESTROY);
    while (!pCMe->m_iDestroyed) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }
    pthread_mutex_unlock(&pCMe->m_xMutex);

    close(pCMe->m_iMsgRead);
    close(pCMe->m_iMsgWrite);
    pthread_cond_destroy(&pCMe->m_xCond);
    pthread_mutex_destroy(&pCMe->m_xMutex);
    //free(pCMe);
}

static void CEcoSystem1_00000100_onDestroy(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_free((struct CEcoSystem1_00000100*)activity->instance);
}

static void CEcoSystem1_00000100_onStart(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_set_activity_state((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_START);
}

static void CEcoSystem1_00000100_onResume(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_set_activity_state((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_RESUME);
}

static void* CEcoSystem1_00000100_onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
    struct CEcoSystem1_00000100* pCMe = (struct CEcoSystem1_00000100*)activity->instance;
    void* savedState = NULL;

    pthread_mutex_lock(&pCMe->m_xMutex);
    pCMe->m_iStateSaved = 0;
    CEcoSystem1_00000100_android_app_write_cmd(pCMe, APP_CMD_SAVE_STATE);
    while (!pCMe->m_iStateSaved) {
        pthread_cond_wait(&pCMe->m_xCond, &pCMe->m_xMutex);
    }

    if (pCMe->m_pSavedState != NULL) {
        savedState = pCMe->m_pSavedState;
        *outLen = pCMe->m_iSavedStateSize;
        pCMe->m_pSavedState = NULL;
        pCMe->m_iSavedStateSize = 0;
    }

    pthread_mutex_unlock(&pCMe->m_xMutex);

    return savedState;
}

static void CEcoSystem1_00000100_onPause(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_set_activity_state((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_PAUSE);
}

static void CEcoSystem1_00000100_onStop(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_set_activity_state((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_STOP);
}

static void CEcoSystem1_00000100_onConfigurationChanged(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_write_cmd((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_CONFIG_CHANGED);
}

static void CEcoSystem1_00000100_onLowMemory(ANativeActivity* activity) {
    CEcoSystem1_00000100_android_app_write_cmd((struct CEcoSystem1_00000100*)activity->instance, APP_CMD_LOW_MEMORY);
}

static void CEcoSystem1_00000100_onWindowFocusChanged(ANativeActivity* activity, int focused) {
    CEcoSystem1_00000100_android_app_write_cmd((struct CEcoSystem1_00000100*)activity->instance, focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void CEcoSystem1_00000100_onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    CEcoSystem1_00000100_android_app_set_window((struct CEcoSystem1_00000100*)activity->instance, window);
}

static void CEcoSystem1_00000100_onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    CEcoSystem1_00000100_android_app_set_window((struct CEcoSystem1_00000100*)activity->instance, NULL);
}

static void CEcoSystem1_00000100_onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    CEcoSystem1_00000100_android_app_set_input((struct CEcoSystem1_00000100*)activity->instance, queue);
}

static void CEcoSystem1_00000100_onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    CEcoSystem1_00000100_android_app_set_input((struct CEcoSystem1_00000100*)activity->instance, NULL);
}


JNIEXPORT void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    int result = 0;
    IEcoSystem1* pISys = 0;

    LOGI("ANativeActivity_onCreate: %p\n", activity);

    /* Создание системного интерфейса приложения */
    result = createCEcoSystem1_00000100(0, 0, &pISys);

    ((CEcoSystem1_00000100*)pISys)->m_pActivity = activity;
    ((CEcoSystem1_00000100*)pISys)->m_pSavedState = savedState;
    ((CEcoSystem1_00000100*)pISys)->m_pSavedState = savedStateSize;

    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onDestroy = CEcoSystem1_00000100_onDestroy;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onStart = CEcoSystem1_00000100_onStart;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onResume = CEcoSystem1_00000100_onResume;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onSaveInstanceState = CEcoSystem1_00000100_onSaveInstanceState;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onPause = CEcoSystem1_00000100_onPause;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onStop = CEcoSystem1_00000100_onStop;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onConfigurationChanged = CEcoSystem1_00000100_onConfigurationChanged;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onLowMemory = CEcoSystem1_00000100_onLowMemory;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onWindowFocusChanged = CEcoSystem1_00000100_onWindowFocusChanged;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onNativeWindowCreated = CEcoSystem1_00000100_onNativeWindowCreated;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onNativeWindowDestroyed = CEcoSystem1_00000100_onNativeWindowDestroyed;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onInputQueueCreated = CEcoSystem1_00000100_onInputQueueCreated;
    ((CEcoSystem1_00000100*)pISys)->m_pActivity->callbacks->onInputQueueDestroyed = CEcoSystem1_00000100_onInputQueueDestroyed;

        LOGI("DialogProcessing: create\n");

    ((CEcoSystem1_00000100*)pISys)->m_pActivity->instance = CEcoSystem1_00000100_android_app_create(pISys, ((CEcoSystem1_00000100*)pISys)->m_pActivity, ((CEcoSystem1_00000100*)pISys)->m_pSavedState, ((CEcoSystem1_00000100*)pISys)->m_iSavedStateSize);


    /* Передача управления точке входа осуществляется в потоке, см. CEcoSystem1_00000100_Entry */
    /* result = EcoMain((IEcoUnknown*)pISys); */

    /* Освобождение системного интерфейса */
    /* другое поведение, освободить не можем, передаем управление */
}

#endif
