//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_OPENGLSHADER_H
#define FFMPEGGLPLAY_OPENGLSHADER_H

#include <mutex>

enum OpenGLShaderType{
    SHADER_YUV420p = 0,
    SHADER_NV12 = 25,//手机摄像头
    SHADER_NV21 = 26//手机摄像头
};
class OpenGLShader {
public:
    virtual bool init(OpenGLShaderType type=SHADER_YUV420p);

    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buf,bool isa= false);

    virtual void draw();

    virtual void close();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;
};


#endif //FFMPEGGLPLAY_OPENGLSHADER_H
