#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"
#include "XLog.h"

void IPlayerProxy::close() {
    mux.lock();
    if (player) {
        player->close();
    }
    mux.unlock();
}

void IPlayerProxy::init(void *vm) {
    LOGE("IPlayerProxy::init");
    mux.lock();
    if (vm) {
        FFPlayerBuilder::initHard(vm);
    }
    if (!player) {
        player = FFPlayerBuilder::get()->builderPlayer();
    }
    mux.unlock();
}

double IPlayerProxy::playPos() {
    double pos = 0.0;
    mux.lock();
    if (player) {
        pos = player->playPos();
    }
    mux.unlock();
    return pos;
}

bool IPlayerProxy::isPause() {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->getPauseState();
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::pause(bool isP) {
    mux.lock();
    if (player) {
        player->pause(isP);
    }
    mux.unlock();
}

bool IPlayerProxy::seek(double pos) {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->seek(pos);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::open(const char *path) {
    bool re = false;
    mux.lock();
    if (player) {
        player->isHardDecode = isHardDecode;
        re=player->open(path);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::start() {
    bool re = false;
    mux.lock();
    if (player) {
        re=player->start();
    }
    mux.unlock();
    return re;
}

void IPlayerProxy::initView(void *win) {
    mux.lock();
    if (player) {
        player->initView(win);
    }
    mux.unlock();
}