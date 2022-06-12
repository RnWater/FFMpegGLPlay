//
// Created by Administrator on 2022/6/10.
//

#ifndef FFMPEGGLPLAY_IPLAYER_H
#define FFMPEGGLPLAY_IPLAYER_H
#include "XThread.h"
#include "IDeMux.h"
class IAudioPlay;
class IOpenGLVideoView;
class IResample;
class IDecode;
class IDeMux;
class IPlayer: public XThread{
public:
    static IPlayer *get(unsigned char index = 0);
    virtual bool open(const char *path);
    virtual void close();
    virtual bool start();

    virtual void initView(void *win);

    virtual double playPos();

    virtual bool seek(double pos);

    virtual void pause(bool isP);

    bool isHardDecode = true;
    XParameter outPara;
    IDeMux *deMux=0;
    IDecode *v_decode = 0;
    IDecode *a_decode = 0;
    IResample *resample = 0;
    IOpenGLVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
protected:
    void main();
    std::mutex mux;
    IPlayer(){};
};


#endif //FFMPEGGLPLAY_IPLAYER_H
