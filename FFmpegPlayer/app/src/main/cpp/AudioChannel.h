//
// Created by WJ on 2021/4/17.
//

#ifndef FFMPEGPLAYER_AUDIOCHANNEL_H
#define FFMPEGPLAYER_AUDIOCHANNEL_H

#include "BaseChannel.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C" {
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
};

class AudioChannel : public BaseChannel {
    friend void *audioPlay_t(void *args);
    friend void bqPlayerCallback(SLAndroidSimpleBufferQueueItf queue, void *pContext);


public:
    AudioChannel(int _channelId, JavaCallHelper *_helper, AVCodecContext *_avCodecContext,
                 AVRational base);
    virtual  ~AudioChannel();


public:
    virtual void play();

    virtual void stop();

    virtual void decode();


private:
    void _play();
    int _getData();


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
    SLAndroidBufferQueueItf bqPlayerBufferQueue = nullptr;
    SLPlayItf bqPlayerInterface = nullptr;
private:
    void _releaseOpenSL();


};


#endif //FFMPEGPLAYER_AUDIOCHANNEL_H
