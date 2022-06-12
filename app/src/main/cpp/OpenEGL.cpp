//
// Created by Administrator on 2022/6/8.
//

#include "OpenEGL.h"
#include <android/native_window.h>
#include <mutex>
#include "XLog.h"
#include "EGL/egl.h"

class MyEGL : public OpenEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;
    virtual void close() {
        mux.lock();
        if (display == EGL_NO_DISPLAY) {
            mux.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display,surface);
        }
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }
    virtual void draw() {
        mux.lock();
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mux.unlock();
            return;
        }
        eglSwapBuffers(display, surface);
        mux.unlock();
    }

    virtual bool init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *)win;
        close();
        mux.lock();
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            mux.unlock();
            LOGE("eglGetDisplay failed!");
            return false;
        }
        LOGE("eglGetDisplay success");
        if (EGL_TRUE!=eglInitialize(display, 0, 0)) {
            mux.unlock();
            LOGE("eglInitialize failed!");
            return false;
        }
        LOGE("eglInitialize success!");
        EGLint configSpec[]={
                EGL_RED_SIZE,8,
                EGL_GREEN_SIZE,8,
                EGL_BLUE_SIZE,8,
                EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config=0;
        EGLint numConfigs=0;
        if (EGL_TRUE!=eglChooseConfig(display, configSpec, &config, 1, &numConfigs)) {
            mux.unlock();
            LOGE("eglChooseConfig failed!");
            return false;
        }
        LOGE("eglChooseConfig success!");
        surface = eglCreateWindowSurface(display, config, nwin, NULL);
        //创建并打开EGL上下文
        const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (context == EGL_NO_CONTEXT) {
            mux.unlock();
            LOGE("eglCreateContext failed!");
            return false;
        }
        LOGE("eglCreateContext success!");
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            mux.unlock();
            LOGE("eglMakeCurrent failed!");
            return false;
        }
        LOGE("eglMakeCurrent success!");
        mux.unlock();
        return true;
    }
};

OpenEGL *OpenEGL::get() {
    static MyEGL egl;
    return &egl;
}