//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_IDEMUX_H
#define FFMPEGGLPLAY_IDEMUX_H
#include "IObserver.h"
#include "XData.h"
#include "XThread.h"
#include "XParameter.h"
//解封装模块
class IDeMux: public IObserver{
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool open(const char *url)=0;
    virtual bool seek(double pos) = 0;
    virtual void close() = 0;
    virtual XData read() = 0;
    virtual XParameter getVPara() = 0;//获取视频的参数
    virtual XParameter getAPara() = 0;//获取音频的参数
    virtual void setName(char *name);
    int totalMs = 0;
protected:
    virtual void main();
};


#endif //FFMPEGGLPLAY_IDEMUX_H
