//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_IDECODE_H
#define FFMPEGGLPLAY_IDECODE_H
#include "XParameter.h"
#include "IObserver.h"
#include <list>
class IDecode:public IObserver{
public:
    //打开解码器
    virtual bool open(XParameter para,bool isHard=false) = 0;

    virtual void close() = 0;
    virtual void clear();
    virtual bool sendPacket(XData pkt) = 0;

    virtual XData recFrame() = 0;

    virtual void update(XData pkt);
    virtual void setName(const char *name);

    bool isAudio = false;
    //最大的缓冲队列
    int maxList = 100;
    //同步时间，再次打开文件要清理
    int synPts = 0;
    int pts = 0;
protected:
    virtual void main();
    std::list<XData> packs;
    std::mutex packsMutex;
};


#endif //FFMPEGGLPLAY_IDECODE_H
