//
// Created by Administrator on 2022/6/9.
//

#include "IAudioPlay.h"
void IAudioPlay::clear(){
    framesMutex.lock();
    while (!frames.empty()) {
        frames.front().drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}
void IAudioPlay::update(XData data) {
    if (data.size<=0||!data.data) return;
    while (!isExit) {
        framesMutex.lock();
        if (frames.size() > maxFrame) {
            framesMutex.unlock();
            xSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}
XData IAudioPlay::getData(){
    XData d;
    isRunning = true;
    while (!isExit) {
        if (getPauseState()) {
            xSleep(2);
            continue;
        }
        framesMutex.lock();
        if (!frames.empty()) {
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = d.pts;
            return d;
        }
        framesMutex.unlock();
        xSleep(1);
    }
    isRunning = false;
    return d;
}