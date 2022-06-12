//
// Created by Administrator on 2022/6/11.
//

#include "FFPlayerBuilder.h"
#include "FFDecode.h"
#include "FFDeMux.h"
#include "SLAudioPlay.h"
#include "FFResample.h"
#include "OpenGLVideoView.h"
void FFPlayerBuilder::initHard(void *vm) {
    FFDecode::initHard(vm);
}
IDeMux *FFPlayerBuilder::createDeMux() {
    IDeMux *ff = new FFDeMux();
    return ff;
}
IDecode *FFPlayerBuilder::createDecode(){
    IDecode *ff = new FFDecode();
    return ff;
}
IResample *FFPlayerBuilder::createResample(){
    IResample *ff = new FFResample();
    return ff;
}
IOpenGLVideoView *FFPlayerBuilder::createVideoView(){
    IOpenGLVideoView *ff = new OpenGLVideoView();
    return ff;
}
IAudioPlay *FFPlayerBuilder::createAudioPlay(){
    IAudioPlay *ff = new SLAudioPlay();
    return ff;
}
IPlayer *FFPlayerBuilder::createPlayer(unsigned char index){
    return IPlayer::get(index);
}