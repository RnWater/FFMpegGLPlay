//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_FFDEMUX_H
#define FFMPEGGLPLAY_FFDEMUX_H
#include "IDeMux.h"
struct AVFormatContext;
class FFDeMux : public IDeMux{
public:
    virtual bool open(const char *url);
    virtual XParameter getVPara();//获取视频的参数
    virtual XParameter getAPara();//获取音频的参数
    virtual XData read();
    //seek 位置 pos 0.0~1.0
    virtual bool seek(double pos);
    virtual void close();
    FFDeMux();

private:
    AVFormatContext *ic = 0;
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FFMPEGGLPLAY_FFDEMUX_H
