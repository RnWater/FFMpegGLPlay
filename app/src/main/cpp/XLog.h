#ifndef FFMPEGGLPLAY_XLOG_H
#define FFMPEGGLPLAY_XLOG_H
class XLog {

};
#define TAG "FFMPegPlayer"
#ifdef ANDROID
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#else
#define XLOGD(...) printf("XPlay",__VA_ARGS__)
#endif
#endif //FFMPEGGLPLAY_XLOG_H
