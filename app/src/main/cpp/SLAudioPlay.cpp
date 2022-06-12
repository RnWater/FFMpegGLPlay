//
// Created by Administrator on 2022/6/9.
//

#include "SLAudioPlay.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "XLog.h"

static SLObjectItf engineSL = NULL;
static SLObjectItf mix=NULL;
static SLObjectItf player=NULL;
static SLPlayItf iPlayer = NULL;
static SLEngineItf eng = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue=NULL;
SLAudioPlay::SLAudioPlay(){
    buf = new unsigned char[1024 * 1024];
}
SLAudioPlay::~SLAudioPlay(){
    delete buf;
    buf = 0;
}
static SLEngineItf createSL(){
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        return NULL;
    }
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re!=SL_RESULT_SUCCESS)return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re!=SL_RESULT_SUCCESS) return NULL;
    return en;
}
static void pcmCall(SLAndroidSimpleBufferQueueItf bf,void *slPlayer){
    SLAudioPlay *sp = (SLAudioPlay *) slPlayer;
    if (!sp) return;
    sp->playCall((void *) bf);
}
bool SLAudioPlay::startPlay(XParameter out){
    close();
    mux.lock();
    eng = createSL();
    if (!eng) {
        mux.unlock();
        LOGE("createSL failed!");
        return false;
    }
    //2 创建混音器
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        return false;
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        LOGE("mix failed");
        return false;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outputMix, 0};
    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm={
            SL_DATAFORMAT_PCM,
            (SLuint32) out.channels,
            (SLuint32)out.sample_rate*1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN//字节序小端
    };
    SLDataSource ds = {&que, &pcm};
    //4创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        return false;
    }
    LOGE("CreateAudioPlayer success!");
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iPlayer);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        return false;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        return false;
    }
    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue, pcmCall, this);
    //设置为播放状态
    (*iPlayer)->SetPlayState(iPlayer, SL_PLAYSTATE_PLAYING);
    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue, "", 1);
    LOGE("SLAudioPlay::StartPlay success!");
    isExit = false;
    mux.unlock();
    return true;
}

void SLAudioPlay::playCall(void *bufq){
    if (!bufq) return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;
    XData d = getData();
    if (d.size <= 0) {
        return;
    }
    if (!buf) {
        return;
    }
    LOGE("playCall");
    memcpy(buf, d.data, d.size);
    mux.lock();
    if(pcmQue&&(*pcmQue)) {
        (*pcmQue)->Enqueue(pcmQue, buf, d.size);
    }
    mux.unlock();
    d.drop();
}

void SLAudioPlay::close() {
    IAudioPlay::clear();
    mux.lock();
    if (iPlayer && (*iPlayer)) {
        (*iPlayer)->SetPlayState(iPlayer, SL_PLAYSTATE_STOPPED);
    }
    if (pcmQue && (*pcmQue)) {
        (*pcmQue)->Clear(pcmQue);
    }
    if (player && (*player)) {
        (*player)->Destroy(player);
    }
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    if (engineSL && (*engineSL)) {
        (*engineSL)->Destroy(engineSL);
    }
    engineSL = 0;
    mix = 0;
    player = 0;
    iPlayer = 0;
    pcmQue = 0;
    eng = 0;
    mux.unlock();
}