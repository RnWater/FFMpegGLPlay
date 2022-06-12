//
// Created by Administrator on 2022/6/11.
//

#ifndef FFMPEGGLPLAY_FFPLAYERBUILDER_H
#define FFMPEGGLPLAY_FFPLAYERBUILDER_H
#include "IPlayerBuilder.h"
class FFPlayerBuilder: public IPlayerBuilder {

public:
    static void initHard(void *vm);
    static FFPlayerBuilder *get(){
        FFPlayerBuilder pb;
        return &pb;
    }
protected:
    FFPlayerBuilder(){};
    virtual IDeMux *createDeMux();
    virtual IDecode *createDecode();
    virtual IResample *createResample();
    virtual IOpenGLVideoView *createVideoView();
    virtual IAudioPlay *createAudioPlay();
    virtual IPlayer *createPlayer(unsigned char index = 0);
};


#endif //FFMPEGGLPLAY_FFPLAYERBUILDER_H
