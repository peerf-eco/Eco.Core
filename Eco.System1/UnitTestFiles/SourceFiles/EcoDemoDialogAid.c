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

#include "IEcoSystem1.h"
#include "IEcoAndroidNativeApp1.h"
#include "AndroidApp.h"

extern IEcoSystem1* g_pISys;


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


static void free_saved_state(struct android_app* android_app) {
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->savedState != NULL) {
        free(android_app->savedState);
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }
    pthread_mutex_unlock(&android_app->mutex);
}

int8_t android_app_read_cmd(struct android_app* android_app) {
    int8_t cmd;
    if (read(android_app->msgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
        switch (cmd) {
            case APP_CMD_SAVE_STATE:
                free_saved_state(android_app);
                break;
        }
        return cmd;
    } else {
        LOGE("No data on command pipe!");
    }
    return -1;
}

static void print_cur_config(struct android_app* android_app) {
    char lang[2], country[2];
    AConfiguration_getLanguage(android_app->config, lang);
    AConfiguration_getCountry(android_app->config, country);

    LOGI("Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
            "keys=%d nav=%d keysHid=%d navHid=%d sdk=%d size=%d long=%d "
            "modetype=%d modenight=%d",
            AConfiguration_getMcc(android_app->config),
            AConfiguration_getMnc(android_app->config),
            lang[0], lang[1], country[0], country[1],
            AConfiguration_getOrientation(android_app->config),
            AConfiguration_getTouchscreen(android_app->config),
            AConfiguration_getDensity(android_app->config),
            AConfiguration_getKeyboard(android_app->config),
            AConfiguration_getNavigation(android_app->config),
            AConfiguration_getKeysHidden(android_app->config),
            AConfiguration_getNavHidden(android_app->config),
            AConfiguration_getSdkVersion(android_app->config),
            AConfiguration_getScreenSize(android_app->config),
            AConfiguration_getScreenLong(android_app->config),
            AConfiguration_getUiModeType(android_app->config),
            AConfiguration_getUiModeNight(android_app->config));
}

//void drawSomething(struct android_app* app ) {
//    int i, j;
//    ANativeWindow_Buffer lWindowBuffer;
//    ANativeWindow* lWindow = app->window;
//    ANativeWindow_setBuffersGeometry(lWindow, 0, 0, WINDOW_FORMAT_RGBA_8888);
//    if ( ANativeWindow_lock(lWindow, &lWindowBuffer, NULL) < 0 ) {
//        return;
//    }
//    memset( lWindowBuffer.bits , 0 , lWindowBuffer.stride * lWindowBuffer.height * sizeof (uint32_t));
//    int sqh = 400, sqw = 600;
//    int wst = lWindowBuffer.stride/2 - sqw / 2;
//    int wed = wst + sqw ;
//    int hst = lWindowBuffer.height /2 - sqh / 2;
//    int hed = hst + sqh ;
//    for ( i = hst ; i < hed ; ++i ) {
//        for ( j = wst ; j < wed ; ++j ) {
//            ( ( char*) ( lWindowBuffer.bits)) [ ( i * lWindowBuffer.stride + j )* sizeof(uint32_t)] = (char)40;
//
//            ( ( char*) ( lWindowBuffer.bits))[ ( i * lWindowBuffer.stride + j )* sizeof ( uint32_t ) + 1 ] = ( char ) 191;
//            ( ( char*) ( lWindowBuffer.bits))[ ( i * lWindowBuffer.stride + j )* sizeof ( uint32_t ) + 2 ] = ( char ) 140;
//            ( ( char* ) ( lWindowBuffer.bits) )[ ( i * lWindowBuffer.stride + j)* sizeof(uint32_t) + 3 ] = ( char) 255;
//        }
//    }
//    ANativeWindow_unlockAndPost(lWindow);
//}

void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd) {
    static int32_t  format = WINDOW_FORMAT_RGBX_8888;
    switch (cmd) {
        case APP_CMD_INPUT_CHANGED:
            LOGV("APP_CMD_INPUT_CHANGED\n");
            pthread_mutex_lock(&android_app->mutex);
            if (android_app->inputQueue != NULL) {
                AInputQueue_detachLooper(android_app->inputQueue);
            }
            android_app->inputQueue = android_app->pendingInputQueue;
            if (android_app->inputQueue != NULL) {
                LOGI("Attaching input queue to looper");
                AInputQueue_attachLooper(android_app->inputQueue,
                        android_app->looper, LOOPER_ID_INPUT, NULL,
                        &android_app->inputPollSource);
            }
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = android_app->pendingWindow;
                ANativeWindow_setBuffersGeometry(android_app->window,
                                                 ANativeWindow_getWidth(android_app->window),
                                                 ANativeWindow_getHeight(android_app->window),
                                                 format
                );
              //  drawSomething( android_app) ;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW\n");
            pthread_cond_broadcast(&android_app->cond);
            break;

        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            LOGI("activityState=%d\n", cmd);
            pthread_mutex_lock(&android_app->mutex);
            android_app->activityState = cmd;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGI("APP_CMD_CONFIG_CHANGED\n");
            AConfiguration_fromAssetManager(android_app->config,
                    android_app->activity->assetManager);
            print_cur_config(android_app);
            break;

        case APP_CMD_DESTROY:
            LOGI("APP_CMD_DESTROY\n");
            android_app->destroyRequested = 1;
            break;
    }
}

void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = NULL;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_SAVE_STATE:
            LOGI("APP_CMD_SAVE_STATE\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->stateSaved = 1;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_RESUME:
            free_saved_state(android_app);
            break;
    }
}

//void app_dummy() {
//
//}

static void android_app_destroy(struct android_app* android_app) {
    LOGI("android_app_destroy!");
    free_saved_state(android_app);
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->inputQueue != NULL) {
        AInputQueue_detachLooper(android_app->inputQueue);
    }
    AConfiguration_delete(android_app->config);
    android_app->destroyed = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);
    // Can't touch android_app object after this.
}

static void process_input(struct android_app* app, struct android_poll_source* source) {
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
        LOGI("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        if (app->onInputEvent != NULL) handled = app->onInputEvent(app, event);
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

static void process_cmd(struct android_app* app, struct android_poll_source* source) {
    int8_t cmd = android_app_read_cmd(app);
    android_app_pre_exec_cmd(app, cmd);
    if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
    android_app_post_exec_cmd(app, cmd);
}

static void* android_app_entry(void* param) {
    int result = 0;
    IEcoSystem1* pISys = 0;
    struct android_app* android_app = (struct android_app*)param;
    LOGI("android_app_entry\n");
    android_app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);

    print_cur_config(android_app);

    android_app->cmdPollSource.id = LOOPER_ID_MAIN;
    android_app->cmdPollSource.app = android_app;
    android_app->cmdPollSource.process = process_cmd;
    android_app->inputPollSource.id = LOOPER_ID_INPUT;
    android_app->inputPollSource.app = android_app;
    android_app->inputPollSource.process = process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
            &android_app->cmdPollSource);
    android_app->looper = looper;

    pthread_mutex_lock(&android_app->mutex);
    android_app->running = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);

        // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;
    while(android_app->window == 0x0) {
        while ((ident = ALooper_pollAll(0, NULL, &events, (void **) &source)) >= 0) {
            // Process this event.
            if (source != NULL) {
                source->process(android_app, source);
            }

            // Check if we are exiting.
            if (android_app->destroyRequested != 0) {
               
                //return;
            }
        }
    }


    //android_main(android_app);

    android_app_destroy(android_app);
    return NULL;
}

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------

static struct android_app* android_app_create(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    LOGI("android_app_create\n");
    struct android_app* android_app = (struct android_app*)malloc(sizeof(struct android_app));
    memset(android_app, 0, sizeof(struct android_app));
    android_app->activity = activity;

    pthread_mutex_init(&android_app->mutex, NULL);
    pthread_cond_init(&android_app->cond, NULL);

    if (savedState != NULL) {
        android_app->savedState = malloc(savedStateSize);
        android_app->savedStateSize = savedStateSize;
        memcpy(android_app->savedState, savedState, savedStateSize);
    }

    int msgpipe[2];
    if (pipe(msgpipe)) {
        LOGI("could not create pipe: %s", strerror(errno));
        return NULL;
    }
    android_app->msgread = msgpipe[0];
    android_app->msgwrite = msgpipe[1];

    pthread_attr_t attr; 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&android_app->thread, &attr, android_app_entry, android_app);

    // Wait for thread to start.
    pthread_mutex_lock(&android_app->mutex);
    while (!android_app->running) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);

    return android_app;
}

static void android_app_write_cmd(struct android_app* android_app, int8_t cmd) {
    if (write(android_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGI("Failure writing android_app cmd: %s\n", strerror(errno));
    }
}

static void android_app_set_input(struct android_app* android_app, AInputQueue* inputQueue) {
    pthread_mutex_lock(&android_app->mutex);
    android_app->pendingInputQueue = inputQueue;
    android_app_write_cmd(android_app, APP_CMD_INPUT_CHANGED);
    while (android_app->inputQueue != android_app->pendingInputQueue) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_window(struct android_app* android_app, ANativeWindow* window) {
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->pendingWindow != NULL) {
        android_app_write_cmd(android_app, APP_CMD_TERM_WINDOW);
    }
    android_app->pendingWindow = window;
    if (window != NULL) {
        android_app_write_cmd(android_app, APP_CMD_INIT_WINDOW);
    }
    while (android_app->window != android_app->pendingWindow) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_activity_state(struct android_app* android_app, int8_t cmd) {
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, cmd);
    while (android_app->activityState != cmd) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_free(struct android_app* android_app) {
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, APP_CMD_DESTROY);
    while (!android_app->destroyed) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);

    close(android_app->msgread);
    close(android_app->msgwrite);
    pthread_cond_destroy(&android_app->cond);
    pthread_mutex_destroy(&android_app->mutex);
    free(android_app);
}

static void onDestroy(ANativeActivity* activity) {
    LOGI("Destroy: %p\n", activity);
    android_app_free((struct android_app*)activity->instance);
}

static void onStart(ANativeActivity* activity) {
    LOGI("Start: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity) {
    LOGI("Resume: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen) {
    struct android_app* android_app = (struct android_app*)activity->instance;
    void* savedState = NULL;

    LOGI("SaveInstanceState: %p\n", activity);
    pthread_mutex_lock(&android_app->mutex);
    android_app->stateSaved = 0;
    android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);
    while (!android_app->stateSaved) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }

    if (android_app->savedState != NULL) {
        savedState = android_app->savedState;
        *outLen = android_app->savedStateSize;
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }

    pthread_mutex_unlock(&android_app->mutex);

    return savedState;
}

static void onPause(ANativeActivity* activity) {
    LOGI("Pause: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity) {
    LOGI("Stop: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity* activity) {
    struct android_app* android_app = (struct android_app*)activity->instance;
    LOGI("ConfigurationChanged: %p\n", activity);
    android_app_write_cmd(android_app, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity) {
    struct android_app* android_app = (struct android_app*)activity->instance;
    LOGI("LowMemory: %p\n", activity);
    android_app_write_cmd(android_app, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused) {
    LOGI("WindowFocusChanged: %p -- %d\n", activity, focused);
    android_app_write_cmd((struct android_app*)activity->instance,
            focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    LOGI("NativeWindowCreated: %p -- %p\n", activity, window);
    android_app_set_window((struct android_app*)activity->instance, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    LOGI("NativeWindowDestroyed: %p -- %p\n", activity, window);
    android_app_set_window((struct android_app*)activity->instance, NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue) {
    LOGI("InputQueueCreated: %p -- %p\n", activity, queue);
    android_app_set_input((struct android_app*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue) {
    LOGI("InputQueueDestroyed: %p -- %p\n", activity, queue);
    android_app_set_input((struct android_app*)activity->instance, NULL);
}

int16_t DialogProcessing() {
	
	ANativeActivity* activity;
	void* savedState;
	size_t savedStateSize;
	IEcoAndroidNativeApp1* pINativeApp = 0;  
    LOGI("DialogProcessing: %p\n", activity);
	
    /* Получение интерфейса для работы с Android Application */
    g_pISys->pVTbl->QueryInterface(g_pISys, &IID_IEcoAndroidNativeApp1, (void **)&pINativeApp);
	pINativeApp->pVTbl->get_Activity(pINativeApp, &activity, &savedState, &savedStateSize);
	
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    LOGI("DialogProcessing: create\n");
	
    activity->instance = android_app_create(activity, savedState, savedStateSize);
}
//void engine_handle_cmd(struct android_app* app, int32_t cmd) {
//    //__android_log_print(ANDROID_LOG_INFO, "native-activity", "%d : received data %d", cmd, (int)(app->userData));
//    switch (cmd) {
//        case APP_CMD_INIT_WINDOW:
//            app->userData = (void*)2; 
//        break;
//    }
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <jni.h>
//#include <errno.h>
////
//#include <EGL/egl.h>
//#include <GLES/gl.h>
////
//#include <android/sensor.h>
//#include <android/log.h>
//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
//
#define LOG_ACCELEROMETER false
/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
                                        LOGV("MY ECO -> engine_init_display\n");
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    return 0;
}

//void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
//    float vertices[4] = {0, 0, x2 - x1, y2 - y1};
//    glLoadIdentity();
//    glTranslatef(x1, y1, 0.0);
//    glVertexPointer (2, GL_FLOAT , 0, vertices);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glColor4f(r, g, b, a);
//    glDrawArrays (GL_LINES, 0, 2);
//}
//
//void drawButton(float x, float y, float w, float h) {
//	drawLine( x, y, x, y+h, x, 0.93f, 0.93f, 1.0f);
//	drawLine( x, y+h, x+w, y+h, 0.93f, 0.93f, 0.93f, 1.0f);
//	drawLine( x+w, y+h, x+w, y, 0.93f, 0.93f, 0.93f, 1.0f);
//	drawLine( x+w, y, x, y, 0.93f, 0.93f, 0.93f, 1.0f);
//
//}

/**
 * Just the current frame in the display.
 */
//static void engine_draw_frame(struct engine* engine) {
//    if (engine->display == NULL) {
//        // No display.
//        return;
//    }
//                                            LOGV("MY ECO -> engine_draw_frame\n");
//    // Just fill the screen with a color.
//    //glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
//      //      ((float)engine->state.y)/engine->height, 1);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    	drawButton(-0.80f, 0.50f, 1.6f, 0.20f);  
//	
//	drawButton(-0.80f, 0.25f, 0.35f, 0.20f);  
//	drawButton(-0.35f, 0.25f, 0.35f, 0.20f);  
//	drawButton( 0.10f, 0.25f, 0.35f, 0.20f);  
//	//drawButton( 0.55f, 0.25f, 0.35f, 0.20f);  
//
//	drawButton(-0.80f, 0.0f, 0.35f, 0.20f);  
//	drawButton(-0.35f, 0.0f, 0.35f, 0.20f);  
//	drawButton( 0.10f, 0.0f, 0.35f, 0.20f);  
//	//drawButton( 0.55f, 0.0f, 0.35f, 0.20f);  
//
//	drawButton(-0.80f, -0.25f, 0.35f, 0.20f);  
//	drawButton(-0.35f, -0.25f, 0.35f, 0.20f);  
//	drawButton( 0.10f, -0.25f, 0.35f, 0.20f);  
//	drawButton( 0.55f, -0.25f, 0.35f, 0.20f);  
//
//	drawButton(-0.80f, -0.50f, 0.80f, 0.20f);  
//	//drawButton(-0.35f, -0.50f, 0.35f, 0.20f);  
//	drawButton( 0.10f, -0.50f, 0.35f, 0.20f);  
//	drawButton( 0.55f, -0.50f, 0.35f, 0.20f);  
//
//    eglSwapBuffers(engine->display, engine->surface);
//}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
                                                LOGV("MY ECO -> engine_term_display\n");
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.

 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
                                                    LOGV("MY ECO -> engine_handle_cmd\n");
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
               // engine_init_display(engine);
                //engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
           // engine_draw_frame(engine);
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void android_main(struct android_app* state) {
//   struct engine engine;
//                                                    LOGV("MY ECO -> android_main\n");
//    // Make sure glue isn't stripped.
//    app_dummy();
//    IEcoSystem1* pISys = 0;
//
//    memset(&engine, 0, sizeof(engine));
//    state->userData = &engine;
//    state->onAppCmd = engine_handle_cmd;
//    state->onInputEvent = engine_handle_input;
//    ANativeActivity* nativeActivity = state->activity;
//    const char* internalPath = nativeActivity->internalDataPath;
//   // android_asset_manager = nativeActivity->assetManager;
//
//    engine.app = state;
//    //LOGI("internalPath %s", internalPath);
//    //memcpy(globalPath, internalPath, strlen(internalPath));
//    // Prepare to monitor accelerometer
//    engine.sensorManager = ASensorManager_getInstance();
//    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
//            ASENSOR_TYPE_ACCELEROMETER);
//    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
//            state->looper, LOOPER_ID_USER, NULL, NULL);
//
//    if (state->savedState != NULL) {
//        // We are starting with a previous saved state; restore from it.
//        engine.state = *(struct saved_state*)state->savedState;
//    }
//
//    // loop waiting for stuff to do.
//
//    while (1) {
//        // Read all pending events.
//        int ident;
//        int events;
//        struct android_poll_source* source;
//
//        // If not animating, we will block forever waiting for events.
//        // If animating, we loop until all events are read, then continue
//        // to draw the next frame of animation.
//        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
//                (void**)&source)) >= 0) {
//
//            // Process this event.
//            if (source != NULL) {
//                source->process(state, source);
//            }
//
//            // If a sensor has data, process it now.
//            if (ident == LOOPER_ID_USER) {
//                if (engine.accelerometerSensor != NULL) {
//                    ASensorEvent event;
//                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
//                            &event, 1) > 0) {
//                        //LOGI("accelerometer: x=%f y=%f z=%f",
//                         //       event.acceleration.x, event.acceleration.y,
//                         //       event.acceleration.z);
//                    }
//                }
//            }
//
//            // Check if we are exiting.
//            if (state->destroyRequested != 0) {
//                engine_term_display(&engine);
//                return;
//            }
//        }
//
//        if (engine.animating) {
//            // Done with events; draw next animation frame.
//            engine.state.angle += .01f;
//            if (engine.state.angle > 1) {
//                engine.state.angle = 0;
//            }
//
//            // Drawing is throttled to the screen update rate, so there
//            // is no need to do timing here.
//            engine_draw_frame(&engine);
//            EcoMain((IEcoUnknown*)pISys);
//        }
//    }
//}


//void android_main(struct android_app* app) {
//    int data = 1;
//    int ident;
//    int events;
//    int result = -1;
//    IEcoSystem1* pISys = 0;
//    struct android_poll_source* source;
//
//    // Make sure glue isn't stripped.
//    app_dummy();
//
//    app->userData = (void*)1;
//    app->onAppCmd = engine_handle_cmd;
//
//    // loop waiting for stuff to do.
//    while (1) {
//        // Read all pending events.
//        ident = 0;
//        events = 0;
//
//
//        // If not animating, we will block forever waiting for events.
//        // If animating, we loop until all events are read, then continue
//        // to draw the next frame of animation.
//        if ((ident=ALooper_pollAll( -1, NULL, &events, (void**)&source)) >= 0) {
//
//            // Process this event.
//            if (source != NULL) {
//                source->process(app, source);
//            }
//        }
//        if ((int)app->userData == 2) {
//            /* �������� ���������� ���������� ���������� */
//            //result = createCEcoSystem1(0, 0, &pISys);
//
//            /* ����� �������� ������� ��������� */
//            //result = EcoMain((IEcoUnknown*)pISys);
//            app->userDta = (void*)3;
//
//            /* ������������ ���������� ���������� */
//            //pISys->Release(pISys);
//           // pISys = 0;
//        }
//
//    }
//}

