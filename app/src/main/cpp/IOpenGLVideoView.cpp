#include "IOpenGLVideoView.h"
#include "XLog.h"

void IOpenGLVideoView::update(XData data) {
    LOGE("IOpenGLVideoView::update");
    this->render(data);
    LOGE("IOpenGLVideoView::update");
}

void IOpenGLVideoView::setName(const char *name) {
    this->name = name;
}