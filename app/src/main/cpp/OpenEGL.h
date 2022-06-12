//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_OPENEGL_H
#define FFMPEGGLPLAY_OPENEGL_H


class OpenEGL {
public:
    virtual bool init(void *win) = 0;
    virtual void draw() = 0;
    static OpenEGL *get();
    virtual void close() = 0;

protected:
    OpenEGL(){}
};


#endif //FFMPEGGLPLAY_OPENEGL_H
