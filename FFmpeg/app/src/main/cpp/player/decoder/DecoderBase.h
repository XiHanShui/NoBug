//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_DECODERBASE_H
#define FFMPEGDEMO_DECODERBASE_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
};

#include <thread>
#include "Decoder.h"

#define  MAX_PATH 2048;
#define  DELAY_THRESHOLD 100 //100ms
using namespace std;

enum DecoderState {
    STATE_UNKNOWN,
    STATE_DECODING,
    STATE_PAUSE,
    STATE_STOP
};
enum DecoderMsg {

    MSG_DECODER_INIT_ERROR,
    MSG_DECODER_READY,
    MSG_DECODER_DONE,
    MSG_DECODER_RENDER,
    MSG_DECODER_TIME

};


class DecoderBase : public Decoder {
public:
    DecoderBase() {};

    virtual ~DecoderBase() {};

    virtual void start();

    virtual void pause();

    virtual void stop();

    float getDuration() override {
        return 0;
    }

    void seekToPosition(float position) override {

    }

    float getCurrentPosition() override {
        return 0;
    }

    void setMessageCallback(void *context, MessageCallback callback) override {

    }


private:
    int initFFDecoder();

    void unInitDecoder();

    void startDecodingThread();

    void decodingLoop();

    void updateTimeStamp();

    long avSync();

    int decodeOnePacket();

    static void doAVDecoding(DecoderBase *decoder);


};


#endif //FFMPEGDEMO_DECODERBASE_H
