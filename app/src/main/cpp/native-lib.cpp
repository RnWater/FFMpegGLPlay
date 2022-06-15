#include <jni.h>
#include <string>
#include "XLog.h"
#include "IDeMux.h"
#include "FFDeMux.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "IResample.h"
#include "FFResample.h"
#include "IOpenGLVideoView.h"
#include "OpenGLVideoView.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "IAudioPlay.h"
#include "SLAudioPlay.h"

extern "C"{
#include <libavcodec/avcodec.h>
}
#include "IPlayerProxy.h"
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    IPlayerProxy::get()->init(vm);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_henry_ffmpegglplay_MyOpenGLVideoView_initView(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::get()->initView(win);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_henry_ffmpegglplay_MainActivity_openVideo(JNIEnv *env, jobject thiz, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    if (path == nullptr) {
        env->ReleaseStringUTFChars(path_, path);
        return;
    }
    IPlayerProxy::get()->open(path);
    IPlayerProxy::get()->start();
    env->ReleaseStringUTFChars(path_, path);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_henry_ffmpegglplay_MainActivity_getPlayPos(JNIEnv *env, jobject thiz) {
    return IPlayerProxy::get()->playPos();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_henry_ffmpegglplay_MainActivity_setPlayState(JNIEnv *env, jobject thiz, jboolean is_p) {
    IPlayerProxy::get()->pause(!IPlayerProxy::get()->getPauseState());
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_henry_ffmpegglplay_MainActivity_getPlayState(JNIEnv *env, jobject thiz) {
    return IPlayerProxy::get()->getPauseState();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_henry_ffmpegglplay_MainActivity_seek(JNIEnv *env, jobject thiz, jdouble pos) {
    IPlayerProxy::get()->seek(pos);
}