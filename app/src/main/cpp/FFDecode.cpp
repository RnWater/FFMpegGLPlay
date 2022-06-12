//
// Created by Administrator on 2022/6/7.
//

#include "FFDecode.h"
#include "XLog.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}
void FFDecode::initHard(void *vm) {
    av_jni_set_java_vm(vm, 0);//设置支持硬解码
}

void FFDecode::close() {
    IDecode::clear();
    mux.lock();
    pts = 0;
    if (frame) {
        av_frame_free(&frame);
    }
    if (codec) {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

void FFDecode::clear() {
    IDecode::clear();
    mux.lock();
    if (codec) {
        avcodec_flush_buffers(codec);
    }
    mux.unlock();
}

bool FFDecode::open(XParameter para,bool isHard){
    close();
    if (!para.para) {
        return false;
    }
    AVCodecParameters *p = para.para;
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (isHard) {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if (!cd) {
        LOGE("av_codec_find_decoder %d failed!", p->codec_id);
        return false;
    }
    mux.lock();
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);
    codec->thread_count = 8;
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        LOGE("FFDecode %s",buf);
        return false;
    }
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
        LOGE("av_codec_open2 video success!");
    } else {
        this->isAudio = true;
        LOGE("av_codec_open2 audio success!");
    }
    mux.unlock();
    return true;
}

bool FFDecode::sendPacket(XData pkt){
    if (pkt.size<=0||!pkt.data) return false;
    mux.lock();
    if (!codec) {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, (AVPacket *) pkt.data);
    if (re != 0) {
        mux.unlock();
        return false;
    }
    //清理
    av_packet_unref((AVPacket *) pkt.data);
    mux.unlock();
    return true;
}

XData FFDecode::recFrame(){
    mux.lock();
    if (!codec) {
        mux.unlock();
        return XData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec,frame);
    if (re != 0) {
        mux.unlock();
        return XData();
    }
    XData d;
    d.data = (unsigned char *) frame;
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;//?
        d.width = frame->width;
        d.height = frame->height;
    } else{
        //av_get_bytes_per_sample 单个样本的大小 nb_samples单通道的样本数
        d.size = av_get_bytes_per_sample((AVSampleFormat) frame->format) * frame->nb_samples * 2;
    }
    d.format = frame->format;
    memcpy(d.datas, frame->data, sizeof(d.datas));
    d.pts = frame->pts;
    pts = d.pts;
    mux.unlock();
    return d;
}