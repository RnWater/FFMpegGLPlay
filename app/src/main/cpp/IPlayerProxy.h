//
// Created by Administrator on 2022/6/11.
//

#ifndef FFMPEGGLPLAY_IPLAYERPROXY_H
#define FFMPEGGLPLAY_IPLAYERPROXY_H

#include "IPlayer.h"
#include "mutex"

class IPlayerProxy : public IPlayer {
public:
    static IPlayerProxy *get() {
        static IPlayerProxy px;
        return &px;
    }
    void init(void *vm = 0);

    virtual bool open(const char *path);

    virtual bool seek(double pos);

    virtual void close();

    virtual bool start();

    virtual void initView(void *win);

    virtual void pause(bool isP);

    virtual bool isPause();

    virtual double playPos();

protected:
    IPlayerProxy() {}

    IPlayer *player = 0;
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_IPLAYERPROXY_H
