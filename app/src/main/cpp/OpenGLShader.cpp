//
// Created by Administrator on 2022/6/8.
//

#include "OpenGLShader.h"
#include "XLog.h"
#include <GLES2/gl2.h>

#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //顶点坐标
        attribute vec2 aTexCoord; //材质顶点坐标
        varying vec2 vTexCoord;   //输出的材质坐标
        void main(){
            vTexCoord = vec2(aTexCoord.x,1.0-aTexCoord.y);
            gl_Position = aPosition;
        }
);
//片元着色器,软解码和部分x86硬解码
static const char *fragYUV420P = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);


//片元着色器,软解码和部分x86硬解码
static const char *fragNV12 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).a - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);
//片元着色器,软解码和部分x86硬解码
static const char *fragNV21 = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uvTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uvTexture,vTexCoord).a - 0.5;
            yuv.b = texture2D(uvTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb,1.0);
        }
);

/**
 * 根据类型创建shader
 * @param code
 * @param type
 * @return
 */
static GLuint initShader(const char *code, GLint type) {
    GLuint sh = glCreateShader(type);
    if (sh == 0) {
        LOGE("glCreateShader failed!");
        return 0;
    }
    glShaderSource(sh,
                   1,//shader的数量
                   &code,//shader的代码
                   0);
    glCompileShader(sh);//编译shader
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGE("glCompileShader failed!");
        return 0;
    }
    return sh;
}

bool OpenGLShader::init(OpenGLShaderType type) {
    close();
    mux.lock();
    vsh = initShader(vertexShader, GL_VERTEX_SHADER);
    if (vsh == 0) {
        mux.unlock();
        LOGE("InitShader GL_VERTEX_SHADER failed!");
        return false;
    }
    switch (type) {
        case SHADER_YUV420p:
            fsh = initShader(fragYUV420P, GL_FRAGMENT_SHADER);
            break;
        case SHADER_NV12:
            fsh = initShader(fragNV12, GL_FRAGMENT_SHADER);
            break;
        case SHADER_NV21:
            fsh = initShader(fragNV21, GL_FRAGMENT_SHADER);
            break;
        default:
            mux.unlock();
            LOGE("SHADER format is error");
            return false;
    }
    LOGE("InitShader GL_VERTEX_SHADER success!");
    if (fsh == 0) {
        mux.unlock();
        LOGE("InitShader GL_FRAGMENT_SHADER failed!");
        return false;
    }
    LOGE("InitShader GL_FRAGMENT_SHADER success!");
    program = glCreateProgram();
    if (program == 0) {
        LOGE("glCreateProgram failed!");
        return false;
    }
    //渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    //链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        mux.unlock();
        LOGE("glLinkProgram failed!");
        return false;
    }
    //使用program
    glUseProgram(program);
    static float vers[] = {
            1.0f, -1.0f, 0.0f,//右下
            -1.0f, -1.0f, 0.0f,//左下
            1.0f, 1.0f, 0.0f,//右上
            -1.0f, 1.0f, 0.0f,//左上
    };
    GLuint apo = (GLuint) glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apo);
    //3个为一组共12个 float类型
    glVertexAttribPointer(apo, 3, GL_FLOAT, GL_FALSE, 12, vers);
    //加入材质坐标数据
    static float txts[] = {
            1.0f, 0.0f, //右下
            0.0f, 0.0f,//左下
            1.0f, 1.0f,//右上
            0.0, 1.0//左上
    };
    GLuint atex = (GLuint) glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    //2个为一组共8个 float类型
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);
    glUniform1i( glGetUniformLocation(program,"yTexture"),0); //对于纹理第1层
    switch (type) {
        case SHADER_YUV420p:
            glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //对于纹理第2层
            glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //对于纹理第3层
            break;
        case SHADER_NV21:
        case SHADER_NV12:
            glUniform1i(glGetUniformLocation(program, "uvTexture"), 1); //对于纹理第2层
            break;
    }
    mux.unlock();
    LOGE("初始化Shader成功！");
    return true;
}

void OpenGLShader::getTexture(unsigned int index, int width, int height, unsigned char *buf,bool isa) {
    unsigned int format = GL_LUMINANCE;
    if (isa) {
        format = GL_LUMINANCE_ALPHA;
    }
    mux.lock();
    if (texts[index] == 0) {
        //初始化材质
        glGenTextures(1, &texts[index]);
        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D, texts[index]);
        //缩小的过滤器
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format,
                     width, height,
                     0,
                     format,
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL);
    }
    //激活第i层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texts[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, buf);
    mux.unlock();
}

void OpenGLShader::draw() {
    mux.lock();
    if (!program) {
        mux.unlock();
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mux.unlock();
}

void OpenGLShader::close() {
    mux.lock();
    if (program) {
        glDeleteProgram(program);
    }
    if (fsh) {
        glDeleteShader(fsh);
    }
    if (vsh) {
        glDeleteShader(vsh);
    }
    for (int i = 0; i < sizeof(texts) / sizeof(unsigned int); ++i) {
        if (texts[i]) {
            glDeleteTextures(1, &texts[i]);
        }
        texts[i] = 0;
    }
    mux.unlock();
}