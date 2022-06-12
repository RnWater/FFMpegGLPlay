//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_IOPENGLVIDEOVIEW_H
#define FFMPEGGLPLAY_IOPENGLVIDEOVIEW_H
#include "IObserver.h"
#include "XData.h"
class IOpenGLVideoView :public IObserver{
public:
    virtual void setRender(void *win) = 0;
    virtual void render(XData data) = 0;
    virtual void update(XData data);
    virtual void setName(const char *name);
    virtual void close() = 0;
};


#endif //FFMPEGGLPLAY_IOPENGLVIDEOVIEW_H
