//
// Created by WJ on 2021/4/17.
//

#ifndef FFMPEGPLAYER_BASECHANNEL_H
#define FFMPEGPLAYER_BASECHANNEL_H
extern "C" {
#include <libavcodec/avcodec.h>
};

#include "JavaCallHelper.h"
#include "SafeQueue.h"


class BaseChannel {
public:
    BaseChannel(int _channelId, JavaCallHelper *_helper, AVCodecContext *_avCodecContext,
                AVRational base) : channelId(_channelId), helper(_helper),
                                   avCodecContext(_avCodecContext), time_base((base)) {
        pkt_queue.setReleaseHandle(releaseAvPacket);
        frame_queue.setReleaseHandle(releaseAvFrame);
    };

    virtual  ~BaseChannel() {

        if (avCodecContext) {
            avcodec_close(avCodecContext);
            avcodec_free_context(&avCodecContext);
            avCodecContext = nullptr;
        }
        pkt_queue.clear();
        frame_queue.clear();

    }


    virtual void play() = 0;

    virtual void stop() = 0;

    virtual void decode() = 0;


    void setEnable(bool enable) {
        pkt_queue.setEnable(enable);
        frame_queue.setEnable(enable);
    }

    static void releaseAvPacket(AVPacket *&packet) {
        if (packet) {
            av_packet_free(&packet);
            packet = nullptr;
        }
    }

    static void releaseAvFrame(AVFrame *&frame) {
        if (frame) {
            av_frame_free(&frame);
            frame = nullptr;
        }
    }


public:
    int channelId;
    JavaCallHelper *helper;
    AVCodecContext *avCodecContext;
    AVRational time_base;
    SafeQueue<AVPacket *> pkt_queue;
    SafeQueue<AVFrame *> frame_queue;
    bool isPlaying = false;
    double clock = 0;

};


#endif //FFMPEGPLAYER_BASECHANNEL_H
