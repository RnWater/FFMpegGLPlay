//
// Created by Administrator on 2022/6/8.
//

#include "IOpenGLVideoView.h"
#include "XLog.h"

void IOpenGLVideoView::update(XData data) {
    this->render(data);
}

void IOpenGLVideoView::setName(const char *name) {
    this->name = name;
}