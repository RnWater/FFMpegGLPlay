//
// Created by Administrator on 2022/6/7.
//

#include "FFDeMux.h"
#include "XLog.h"
extern "C"{
#include <libavformat/avformat.h>
}
//时间基数
static double r2d(AVRational r){
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}
bool FFDeMux::seek(double pos){
    if (pos <= 0 || pos > 1) {
        return false;
    }
    bool re = false;
    mux.lock();
    if (!ic) {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(ic);
    long long seekPts = 0;
    //获取拖动的位置的pts
    seekPts = ic->streams[videoStream]->duration * pos;
    //往后寻找
    re = av_seek_frame(ic, videoStream, seekPts, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return re;
}
void FFDeMux::close(){
    mux.lock();
    if (ic) {
        avformat_close_input(&ic);
    }
    mux.unlock();
}
bool FFDeMux::open(const char *url) {
    //打开之前先做关闭的操作
    close();
    mux.lock();
    int re = avformat_open_input(&ic, url, 0, 0);//打开的文件不知道是什么封装类型所以fmt为0
    if (re != 0) {//0是打开成功
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        LOGE("FFDeMux open %s failed",url);
        return false;
    }
    LOGE("FFDeMux open %s success",url);
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        LOGE("av_format_find_stream_info %s failed!",url);
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);
    mux.unlock();
    getVPara();
    getAPara();
    return true;
}
//获取视频的参数
XParameter FFDeMux::getVPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        LOGE("getVPara failed! ic is NULL");
        return XParameter();
    }
    int re=av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        LOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}
//获取音频的参数
XParameter FFDeMux::getAPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        LOGE("getVPara failed! ic is NULL");
        return XParameter();
    }
    int re=av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        LOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}
//读取一帧
XData FFDeMux::read(){
    mux.lock();
    if (!ic) {
        mux.unlock();
        return XData();
    }
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) {
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }
    XData data;
    data.data = (unsigned char *)pkt;
    data.size = pkt->size;
    if (pkt->stream_index == audioStream) {
        data.isAudio = true;
    } else if (pkt->stream_index == videoStream) {
        data.isAudio = false;
    } else{
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }
    //pts和dts的求值
    pkt->pts = pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    data.pts = (int)pkt->pts;
    mux.unlock();
    return data;
}
//在解封装里面初始化注册和网络
FFDeMux::FFDeMux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;//只初始化一次
        //注册解封装
        av_register_all();
        //初始化解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
    }
}
