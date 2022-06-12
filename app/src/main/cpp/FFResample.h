//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_FFRESAMPLE_H
#define FFMPEGGLPLAY_FFRESAMPLE_H
#include "IResample.h"
struct SwrContext;
class FFResample :public IResample{
public:
    virtual bool open(XParameter in, XParameter out = XParameter());
    virtual XData resample(XData inData);
    virtual void close();
  protected:
    SwrContext *actx = 0;
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_FFRESAMPLE_H
