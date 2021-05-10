//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_AUDIOCHANNEL_H
#define FFMPEGDEMO_AUDIOCHANNEL_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "BaseChannel.h"

extern "C" {
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
};

class AudioChannel : public BaseChannel {

    friend void *audioPlay_t(void *args);

    friend void bqPlayerCallback(SLAndroidSimpleBufferQueueItf queue, void *pContext);


private:
    pthread_t audioDecodeTask;
    pthread_t audioPlayTask;
    SwrContext *swrContext = 0;
    int out_channels;
    int out_sampleSize;
    int out_sampleRate;
    uint8_t *buffer;
    int bufferCount;

    SLObjectItf engineObject = nullptr;
    SLEngineItf engineInterface = nullptr;
    SLObjectItf outputMixObject = nullptr;
    SLObjectItf bqPlayerObject = nullptr;
    SLAndroidSimpleBufferQueueItf bqPLayerBufferQueue = nullptr;
    SLPlayItf bqPlayerInterface = nullptr;

    void _releaseOpenSL();


public:
    AudioChannel(int channelId, JavaCallHelper *_helper, AVCodecContext *_avCodecContext,
                 const AVRational &base);

    virtual  ~AudioChannel();


public:
    virtual void play();

    virtual void stop();

    virtual void decode();


private:

    void _Play();

    int _getData();


};


#endif //FFMPEGDEMO_AUDIOCHANNEL_H
