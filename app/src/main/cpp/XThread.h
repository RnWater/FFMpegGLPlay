//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_XTHREAD_H
#define FFMPEGGLPLAY_XTHREAD_H
void xSleep(int mis);
class XThread {
public:
    virtual bool start();
    virtual void stop();
    virtual void main() {}

    virtual void setPause(bool isP);
    virtual bool getPauseState(){
        isPausing = isPause;
        return isPause;
    }

protected:
    bool isExit = false;
    bool isRunning = false;
    bool isPause=false;
    bool isPausing = false;
private:
    void threadMain();
};


#endif //FFMPEGGLPLAY_XTHREAD_H
