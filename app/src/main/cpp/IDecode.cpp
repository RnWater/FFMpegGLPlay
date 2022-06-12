//
// Created by Administrator on 2022/6/7.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::update(XData pkt){
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        packsMutex.lock();
        //阻塞 如果达到最大值就先不加入
        if (packs.size() < maxList) {
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        xSleep(1);
    }
}
/**
 * 线程开始的时候就会执行此函数
 */
void IDecode::main(){
    while (!isExit) {
        packsMutex.lock();
        if (packs.empty()) {
            packsMutex.unlock();
            xSleep(1);
            continue;
        }
        XData pack = packs.front();
        packs.pop_front();
        //发送数据到解码线程，一个数据包，可解码多个结果
        if (this-> sendPacket(pack)) {
            while (!isExit) {
                XData frame = recFrame();
                if (!frame.data) {
                    break;
                }
//                LOGE("发送解码数据%d,format=%d",frame.size,frame.format);
                this->notify(frame);
            }
        }
        pack.drop();
        packsMutex.unlock();
    }
}

void IDecode::setName(const char *name) {
    this->name = name;
}
void IDecode::clear() {
    packsMutex.lock();
    while (!packs.empty()) {
        packs.front().drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
}