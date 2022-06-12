//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_FFDECODE_H
#define FFMPEGGLPLAY_FFDECODE_H
#include "IDecode.h"
#include "XParameter.h"
struct AVCodecContext;
struct AVFrame;
class FFDecode: public IDecode{
public:
    static void initHard(void *vm);
    virtual bool open(XParameter para,bool isHard=false);
    virtual void clear();
    virtual void close();
    virtual bool sendPacket(XData pkt);

    virtual XData recFrame();
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_FFDECODE_H
