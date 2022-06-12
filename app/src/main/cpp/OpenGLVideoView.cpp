//
// Created by Administrator on 2022/6/8.
//

#include "OpenGLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void OpenGLVideoView::setRender(void *win) {
    view = win;
}
void OpenGLVideoView::close(){
    mux.lock();
    if (txt) {
        txt->drop();
        txt = 0;
    }
    mux.unlock();
}
void OpenGLVideoView::render(XData data) {
    if (!view) {
        return;
    }
    if (!txt) {
        LOGE("OpenGLVideoView txt will init!");
        txt = XTexture::create();
        txt->init(view);
    }
    txt->draw(data.datas, data.width, data.height);
}
