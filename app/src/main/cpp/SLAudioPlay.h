//
// Created by Administrator on 2022/6/9.
//

#ifndef FFMPEGGLPLAY_SLAUDIOPLAY_H
#define FFMPEGGLPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual bool startPlay(XParameter out);
    virtual void close();
    void playCall(void *bufq);
    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_SLAUDIOPLAY_H
