//
// Created by Administrator on 2022/6/7.
//

#include "IObserver.h"
#include "XLog.h"

void IObserver::addObs(IObserver *obs) {
    if (!obs) return;
    mux.lock();
    vObs.push_back(obs);
    LOGE("我的名字%s",obs->name);
    mux.unlock();
}
void IObserver::notify(XData data) {
    mux.lock();
    for (int i = 0; i < vObs.size(); i++) {
        vObs[i]->update(data);
    }
    mux.unlock();
}