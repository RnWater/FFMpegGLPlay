//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_IRESAMPLE_H
#define FFMPEGGLPLAY_IRESAMPLE_H
#include "IObserver.h"
#include "XParameter.h"
class IResample :public IObserver{
public:
    virtual bool open(XParameter in, XParameter out = XParameter()) = 0;

    virtual void close() = 0;
    virtual XData resample(XData inData) = 0;

    virtual void update(XData data);
    virtual void setName(const char *name) ;
    int outChannels = 2;
    int outFormat = 1;

};


#endif //FFMPEGGLPLAY_IRESAMPLE_H
