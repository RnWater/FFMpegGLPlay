//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_OPENGLVIDEOVIEW_H
#define FFMPEGGLPLAY_OPENGLVIDEOVIEW_H
#include "XData.h"
#include "IOpenGLVideoView.h"
class XTexture;
class OpenGLVideoView :public IOpenGLVideoView{
    virtual void setRender(void *win);
    virtual void render(XData data);
    virtual void close();
protected:
    void *view = 0;
    XTexture *txt = 0;
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_OPENGLVIDEOVIEW_H
