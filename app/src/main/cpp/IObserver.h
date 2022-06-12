//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_IOBSERVER_H
#define FFMPEGGLPLAY_IOBSERVER_H
#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>
class IObserver :public XThread{
public:
    //观察者接收数据函数
    virtual void update(XData data){}
    virtual void setName(const char *name){}
    //主体函数 添加观察者(线程安全)
    void addObs(IObserver *obs);
    //通知所有观察者(线程安全)
    void notify(XData data);

protected:
    const char *name= 0;
    std::vector<IObserver *> vObs;
    std::mutex mux;//线程互斥锁
};
#endif //FFMPEGGLPLAY_IOBSERVER_H
