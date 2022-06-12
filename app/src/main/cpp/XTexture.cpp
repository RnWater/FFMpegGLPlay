//
// Created by Administrator on 2022/6/8.
//
#include "XTexture.h"
#include "XLog.h"
#include "OpenGLShader.h"
#include "OpenEGL.h"

class VXTexture : public XTexture {
public:
    OpenGLShader sh;
    XTextureType type;
    std::mutex mux;
    virtual void drop(){
        mux.lock();
        OpenEGL::get()->close();
        sh.close();
        mux.unlock();
        delete this;
    }
    virtual bool init(void *win,XTextureType type) {
        mux.lock();
        OpenEGL::get()->close();
        sh.close();
        this->type = type;
        if (!win) {
            mux.unlock();
            LOGE("XTexture Init failed win is NULL");
            return false;
        }
        if (!OpenEGL::get()->init(win)) {
            mux.unlock();
            return false;
        }
        sh.init((OpenGLShaderType)type);
        mux.unlock();
        return true;
    }

    virtual void draw(unsigned char *data[], int width, int height) {
        mux.lock();
        sh.getTexture(0, width, height, data[0]);//Y
        if (type = TEXTURE_YUV420p) {
            sh.getTexture(1, width/2, height/2, data[1]);//U
            sh.getTexture(2, width/2, height/2, data[2]);//V
        } else{
            sh.getTexture(1, width/2, height/2, data[1], true);//UV
        }
        sh.draw();
        OpenEGL::get()->draw();
        mux.unlock();
    }
};

XTexture *XTexture::create() {
    return new VXTexture();
}