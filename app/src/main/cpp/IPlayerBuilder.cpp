#include "IDecode.h"
#include "IDeMux.h"
#include "IAudioPlay.h"
#include "IResample.h"
#include "IOpenGLVideoView.h"
#include "IPlayerBuilder.h"
#include "XLog.h"

IPlayer *IPlayerBuilder::builderPlayer(unsigned char index){
    LOGE("IPlayerBuilder::builderPlayer index=%d",index);
    IPlayer *play = createPlayer(index);
    //解封装
    IDeMux *de = createDeMux();
    //视频解码
    IDecode *v_decode = createDecode();
    //音频解码
    IDecode *a_decode = createDecode();
    //解码器观察解封装
    de->addObs(v_decode);
    de->addObs(a_decode);
    //显示观察视频解码器
    IOpenGLVideoView *videoView = createVideoView();
    v_decode->addObs(videoView);
    //重采样观察者解码器
    IResample *resample = createResample();
    a_decode->addObs(resample);
    //音频播放观察重采样
    IAudioPlay *audioPlay = createAudioPlay();
    resample->addObs(audioPlay);

    play->deMux = de;
    play->a_decode = a_decode;
    play->v_decode = v_decode;
    play->resample = resample;
    play->audioPlay = audioPlay;
    play->videoView = videoView;
    return play;
}