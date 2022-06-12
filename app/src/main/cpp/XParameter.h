//
// Created by Administrator on 2022/6/7.
//

#ifndef FFMPEGGLPLAY_XPARAMETER_H
#define FFMPEGGLPLAY_XPARAMETER_H
struct AVCodecParameters;
class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;
};
#endif //FFMPEGGLPLAY_XPARAMETER_H
