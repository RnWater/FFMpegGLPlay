//
// Created by Administrator on 2022/6/7.
//
#include "XThread.h"
#include <thread>
#include "XLog.h"
using namespace std;
void xSleep(int mis){
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}
bool XThread::start(){
    isExit = false;
    isPause = false;
    thread th(&XThread::threadMain, this);
    th.detach();
    return true;
}

void XThread::setPause(bool isP) {
    isPause = isP;
    for (int i = 0; i < 10; i++) {
        if (isPausing == isP) {
            break;
        }
        xSleep(10);
    }
}
void XThread::stop(){
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRunning) {
            LOGE("stop线程停止成功！");
            return;
        }
        xSleep(1);
    }
    LOGE("Stop 停止线程超时!");
}
void XThread::threadMain(){
    isRunning = true;
    main();
    isRunning = false;//线程执行完毕之后赋值为false
}