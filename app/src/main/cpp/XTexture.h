//
// Created by Administrator on 2022/6/8.
//

#ifndef FFMPEGGLPLAY_XTEXTURE_H
#define FFMPEGGLPLAY_XTEXTURE_H
enum XTextureType{
    TEXTURE_YUV420p = 0,
    TEXTURE_NV12=25,
    TEXTURE_NV21=26
};

class XTexture {
public:
    static XTexture *create();

    virtual bool init(void *win,XTextureType type=TEXTURE_YUV420p) = 0;

    virtual void draw(unsigned char *data[], int width, int height) = 0;

    virtual void drop() = 0;
    virtual ~XTexture(){};
protected:
    XTexture(){};
};


#endif //FFMPEGGLPLAY_XTEXTURE_H
