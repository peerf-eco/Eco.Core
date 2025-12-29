/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoSystem1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию фабрики для компонента
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_ECO_SYSTEM_1_H__
#define __C_ECO_SYSTEM_1_H__

#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IEcoCommandArguments1.h"
#include "IEcoSystemInformation1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IEcoInterfaceBus1FileExt.h"
#include "IEcoInterfaceBus1NetExt.h"
#include "IEcoError1.h"

#ifdef ECO_ANDROID

#include "IEcoAndroidNativeApp1.h"

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

enum {
    /**
     * Looper data ID of commands coming from the app's main thread, which
     * is returned as an identifier from ALooper_pollOnce().  The data for this
     * identifier is a pointer to an android_poll_source structure.
     * These can be retrieved and processed with android_app_read_cmd()
     * and android_app_exec_cmd().
     */
    LOOPER_ID_MAIN = 1,

    /**
     * Looper data ID of events coming from the AInputQueue of the
     * application's window, which is returned as an identifier from
     * ALooper_pollOnce().  The data for this identifier is a pointer to an
     * android_poll_source structure.  These can be read via the inputQueue
     * object of android_app.
     */
    LOOPER_ID_INPUT = 2,

    /**
     * Start of user-defined ALooper identifiers.
     */
    LOOPER_ID_USER = 3,
};

enum {
    /**
     * Command from main thread: the AInputQueue has changed.  Upon processing
     * this command, android_app->inputQueue will be updated to the new queue
     * (or NULL).
     */
    APP_CMD_INPUT_CHANGED,

    /**
     * Command from main thread: a new ANativeWindow is ready for use.  Upon
     * receiving this command, android_app->window will contain the new window
     * surface.
     */
    APP_CMD_INIT_WINDOW,

    /**
     * Command from main thread: the existing ANativeWindow needs to be
     * terminated.  Upon receiving this command, android_app->window still
     * contains the existing window; after calling android_app_exec_cmd
     * it will be set to NULL.
     */
    APP_CMD_TERM_WINDOW,

    /**
     * Command from main thread: the current ANativeWindow has been resized.
     * Please redraw with its new size.
     */
    APP_CMD_WINDOW_RESIZED,

    /**
     * Command from main thread: the system needs that the current ANativeWindow
     * be redrawn.  You should redraw the window before handing this to
     * android_app_exec_cmd() in order to avoid transient drawing glitches.
     */
    APP_CMD_WINDOW_REDRAW_NEEDED,

    /**
     * Command from main thread: the content area of the window has changed,
     * such as from the soft input window being shown or hidden.  You can
     * find the new content rect in android_app::contentRect.
     */
    APP_CMD_CONTENT_RECT_CHANGED,

    /**
     * Command from main thread: the app's activity window has gained
     * input focus.
     */
    APP_CMD_GAINED_FOCUS,

    /**
     * Command from main thread: the app's activity window has lost
     * input focus.
     */
    APP_CMD_LOST_FOCUS,

    /**
     * Command from main thread: the current device configuration has changed.
     */
    APP_CMD_CONFIG_CHANGED,

    /**
     * Command from main thread: the system is running low on memory.
     * Try to reduce your memory use.
     */
    APP_CMD_LOW_MEMORY,

    /**
     * Command from main thread: the app's activity has been started.
     */
    APP_CMD_START,

    /**
     * Command from main thread: the app's activity has been resumed.
     */
    APP_CMD_RESUME,

    /**
     * Command from main thread: the app should generate a new saved state
     * for itself, to restore from later if needed.  If you have saved state,
     * allocate it with malloc and place it in android_app.savedState with
     * the size in android_app.savedStateSize.  The will be freed for you
     * later.
     */
    APP_CMD_SAVE_STATE,

    /**
     * Command from main thread: the app's activity has been paused.
     */
    APP_CMD_PAUSE,

    /**
     * Command from main thread: the app's activity has been stopped.
     */
    APP_CMD_STOP,

    /**
     * Command from main thread: the app's activity is being destroyed,
     * and waiting for the app thread to clean up and exit before proceeding.
     */
    APP_CMD_DESTROY,
};

struct CEcoSystem1_00000100;

struct CEcoSystem1_00000100_android_poll_source {
    // The identifier of this source.  May be LOOPER_ID_MAIN or
    // LOOPER_ID_INPUT.
    int32_t id;

    // The android_app this ident is associated with.
    struct CEcoSystem1_00000100* app;

    // Function to call to perform the standard processing of data from
    // this source.
    void (*process)(struct CEcoSystem1_00000100* me, struct CEcoSystem1_00000100_android_poll_source* source);
};


#endif

typedef struct CEcoSystem1_00000100 {

    /* Таблица функций интерфейса IEcoSystem1 */
    IEcoSystem1VTbl* m_pVTblISys;

    /* Таблица функций интерфейса IEcoSystemInformation1 */
    IEcoSystemInformation1VTbl* m_pVTblISysInfo;

    /* Таблица функций интерфейса IEcoCommandArguments1 */
    IEcoCommandArguments1VTbl* m_pVTblIArgs;

    /* Таблица функций интерфейса IEcoInterfaceBus1 */
    IEcoInterfaceBus1VTbl* m_pVTblIBus;

    /* Таблица функций интерфейса IEcoError1 */
    IEcoError1VTbl* m_pVTblIErr;

#ifdef ECO_MEM_EXT
    /* Таблица функций интерфейса IEcoInterfaceBus1MemExt */
    IEcoInterfaceBus1MemExtVTbl* m_pVTblIMemExt;

#ifdef ECO_FILE_EXT
    /* Таблица функций интерфейса IEcoInterfaceBus1FileExt */
    IEcoInterfaceBus1FileExtVTbl* m_pVTblIFileExt;

#ifdef ECO_NET_EXT
    /* Таблица функций интерфейса IEcoInterfaceBus1NetExt */
    IEcoInterfaceBus1NetExtVTbl* m_pVTblINetExt;

#ifdef ECO_ANDROID
    /* Таблица функций интерфейса IEcoAndroidNativeApp1 */
    IEcoAndroidNativeApp1VTbl* m_pVTblIApp;
#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Данные экземпляра */
    int16_t m_argc;
    char_t **m_argv;
    void* m_data;
    char_t* m_Name;
    char_t* m_Id;
    char_t* m_Path;

    /* Указатель на интерфейс управления памятью */
    IEcoMemoryManager1* m_pIMemMgr;

    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* m_pIBus;
    /* Указатель на интерфейс работы с ошибками */
    IEcoError1* m_pIErr;
#ifdef ECO_MEM_EXT
    IEcoInterfaceBus1MemExt* m_pIMemExt;
#ifdef ECO_FILE_EXT
    IEcoInterfaceBus1FileExt* m_pIFileExt;
#ifdef ECO_NET_EXT
    IEcoInterfaceBus1NetExt* m_pINetExt;
#ifdef ECO_ANDROID

    /* Вспомогательные функции */
    void (*OnAppCmd)(struct CEcoSystem1_00000100* me, int32_t cmd);
    int32_t (*OnInputEvent)(struct CEcoSystem1_00000100* me, AInputEvent* event);

    /* Экземпляр объекта ANativeActivity, в котором запущено это приложение. */
    ANativeActivity* m_pActivity;

    /* Когфигурация приложения. */
    AConfiguration* m_pConfig;

    /* Обработчик событий в потоке приложения. */
    ALooper* m_pLooper;

    /* Окно поверхности для отображения графики. */
    ANativeWindow* m_pWindow;

    /* Очередь входных событий */
    AInputQueue* m_pInputQueue;

    /* Это сохраненное состояние последнего экземпляра, указанное во время создания.
       Значение равно NULL, если состояния не было. Вы можете использовать это по своему усмотрению;
       память будет оставаться свободной до тех пор, пока вы не вызовете android_app_exec_cmd() для APP_CMD_RESUME,
       после чего она будет освобождена, а для SavedState будет установлено значение NULL.
       Эти переменные следует изменять только при обработке APP_CMD_SAVE_STATE, после чего они будут инициализированы значением NULL,
       и вы можете преобразовать свое состояние и поместить информацию сюда. В этом случае память будет освобождена для вас позже.
    */
    void* m_pSavedState;
    size_t m_iSavedStateSize;


    ARect m_xContentRect;
    int m_iActivityState;
    int m_iDestroyRequested;

    pthread_mutex_t m_xMutex;
    pthread_cond_t m_xCond;

    int m_iMsgRead;
    int m_iMsgWrite;

    pthread_t m_Thread;

    struct CEcoSystem1_00000100_android_poll_source m_xCmdPollSource;
    struct CEcoSystem1_00000100_android_poll_source m_xInputPollSource;

    int m_iRunning;
    int m_iStateSaved;
    int m_iDestroyed;
    int m_iRedrawNeeded;
    AInputQueue* m_pPendingInputQueue;
    ANativeWindow* m_pPendingWindow;
    ARect m_xPendingContentRect;

#endif /* ECO_ANDROID */
#endif /* ECO_NET_EXT */
#endif /* ECO_FILE_EXT */
#endif /* ECO_MEM_EXT */

} CEcoSystem1_00000100, *CEcoSystem1_00000100Ptr;

int16_t ECOCALLMETHOD createCEcoSystem1_00000100(/* in */ int16_t argc, /* in */ char_t **argv, /* out */ IEcoSystem1** ppIEcoSys);
void ECOCALLMETHOD deleteCEcoSystem1_00000100(/* in */ IEcoSystem1* pIEcoSys);

#endif /* __C_ECO_SYSTEM_1_H__ */
