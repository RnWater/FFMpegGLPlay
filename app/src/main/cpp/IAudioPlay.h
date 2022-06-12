//
// Created by Administrator on 2022/6/9.
//

#ifndef FFMPEGGLPLAY_IAUDIOPLAY_H
#define FFMPEGGLPLAY_IAUDIOPLAY_H
#include "XParameter.h"
#include "IObserver.h"
#include <list>
class IAudioPlay: public IObserver {
public:
    virtual void update(XData data);
    virtual XData getData();
    virtual bool startPlay(XParameter out) = 0;

    virtual void close() = 0;
    virtual void clear();
    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //FFMPEGGLPLAY_IAUDIOPLAY_H
