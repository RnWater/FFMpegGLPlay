//
// Created by Administrator on 2022/6/11.
//

#ifndef FFMPEGGLPLAY_IPLAYERBUILDER_H
#define FFMPEGGLPLAY_IPLAYERBUILDER_H
#include "IPlayer.h"

class IPlayerBuilder {
public:
    virtual IPlayer *builderPlayer(unsigned char index=0);

protected:
    virtual IDeMux *createDeMux() = 0;

    virtual IDecode *createDecode() = 0;

    virtual IResample *createResample() = 0;

    virtual IOpenGLVideoView *createVideoView() = 0;

    virtual IAudioPlay *createAudioPlay() = 0;

    virtual IPlayer *createPlayer(unsigned char index = 0) = 0;
};


#endif //FFMPEGGLPLAY_IPLAYERBUILDER_H
