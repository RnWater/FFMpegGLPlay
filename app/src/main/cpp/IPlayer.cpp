//
// Created by Administrator on 2022/6/10.
//

#include "IPlayer.h"
#include "IAudioPlay.h"
#include "FFDecode.h"
#include "OpenGLVideoView.h"
#include "XLog.h"
#include "FFResample.h"

IPlayer *IPlayer::get(unsigned char index){
    static IPlayer p[256];
    return &p[index];
}
void IPlayer::main() {
    while (!isExit) {
        mux.lock();
        if (!audioPlay || !v_decode) {
            mux.unlock();
            xSleep(2);
            continue;
        }
        int apts = audioPlay->pts;
        v_decode->synPts = apts;
        mux.unlock();
        xSleep(2);
    }
}
void IPlayer::close() {
    mux.lock();
    XThread::stop();
    if (deMux) {
        deMux->stop();
    }
    if (v_decode) {
        v_decode->stop();
    }
    if (a_decode) {
        a_decode->stop();
    }
    if (audioPlay) {
        audioPlay->stop();
    }
    if (v_decode) {
        v_decode->clear();
    }
    if (a_decode) {
        a_decode->clear();
    }
    if (audioPlay) {
        audioPlay->clear();
    }
    if (v_decode) {
        v_decode->close();
    }
    if (a_decode) {
        a_decode->close();
    }
    if (audioPlay) {
        audioPlay->close();
    }
    if (deMux) {
        deMux->close();
    }
    mux.unlock();
}
double IPlayer::playPos() {
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if (deMux) {
        total = deMux->totalMs;
    }
    if (total > 0) {
        if (v_decode) {
            pos = (double) v_decode->pts / (double) total;
        }
    }
    mux.unlock();
    return pos;
}
void IPlayer::pause(bool isP) {
    mux.lock();
    XThread::setPause(isP);
    if (deMux) {
        deMux->setPause(isP);
    }
    if (v_decode) {
        v_decode->setPause(isP);
    }
    if (a_decode) {
        a_decode->setPause(isP);
    }
    if (audioPlay) {
        audioPlay->setPause(isP);
    }
    mux.unlock();
}
bool IPlayer::seek(double pos) {
    bool re = false;
    if(!deMux) return false;
    pause(pos);
    mux.lock();
    //清理缓冲
    if (v_decode) {
        v_decode->clear();
    }
    if (a_decode) {
        a_decode->clear();
    }
    if (audioPlay) {
        audioPlay->clear();
    }
    re = deMux->seek(pos);
    if (!v_decode) {
        mux.unlock();
        pause(false);
        return re;
    }
    int seekPts = pos * deMux->totalMs;
    while (!isExit) {
        XData pkt = deMux->read();
        if (pkt.size<=0)break;
        if (pkt.isAudio) {
            if (pkt.pts < seekPts) {
                pkt.drop();
                continue;
            }
            deMux->notify(pkt);
            continue;
        }
        v_decode->sendPacket(pkt);
        pkt.drop();
        XData data = v_decode->recFrame();
        if (data.size <= 0) {
            continue;
        }
        if (data.pts >= seekPts) {
            break;
        }
    }
    mux.unlock();
    pause(false);
    return re;
}
bool IPlayer::open(const char *path) {
    close();
    mux.lock();
    //解封装
    if(!deMux || !deMux->open(path)){
        mux.unlock();
        return false;
    }
    //解码 解码可能不需要，如果是解封之后就是原始数据
    if(!v_decode || !v_decode->open(deMux->getVPara(),isHardDecode)){
        LOGE("v_decode->Open %s failed!",path);
    }
    if(!a_decode || !a_decode->open(deMux->getAPara())){
        LOGE("a_decode->Open %s failed!",path);
    }
    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    //if(outPara.sample_rate <= 0)
    outPara = deMux->getAPara();
    if(!resample || !resample->open(deMux->getAPara(),outPara)){
        LOGE("resample->Open %s failed!",path);
    }
    mux.unlock();
}

bool IPlayer::start() {
    mux.lock();
    if(v_decode)
        v_decode->start();
    if(!deMux || !deMux->start()){
        mux.unlock();
        LOGE("deMux->Start failed!");
        return false;
    }
    if(a_decode)
        a_decode->start();
    if(audioPlay)
        audioPlay->startPlay(outPara);
    XThread::start();
    mux.unlock();

    return true;
}

void IPlayer::initView(void *win) {
    if(videoView){
        videoView->close();
        videoView->setRender(win);
    }
}