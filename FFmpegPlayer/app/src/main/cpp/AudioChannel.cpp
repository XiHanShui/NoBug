//
// Created by WJ on 2021/4/17.
//


#include "AudioChannel.h"

extern "C"
{
#include <libavutil/time.h>
}

void *audioDecode_t(void *args) {

}


void *audioPlay_t(void *args) {
    auto *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->play();
    return 0;
}

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf queue, void *pContext) {
    auto *audioChannel = static_cast<AudioChannel *>(pContext);
    int dataSize = audioChannel->_getData();
    if (dataSize > 0) {
        (*queue)->Enqueue(queue, audioChannel->buffer, dataSize);
    }
}

AudioChannel::AudioChannel(int _channelId, JavaCallHelper *_helper, AVCodecContext *_avCodecContext,
                           AVRational base) : BaseChannel(_channelId, _helper, _avCodecContext,
                                                          base) {
    out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    out_sampleSize = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    out_sampleRate = 44100;
    bufferCount = out_sampleRate * out_sampleSize * out_channels;
    buffer = static_cast<uint8_t *>(malloc(bufferCount));

}

AudioChannel::~AudioChannel() {
    free(buffer);
    buffer = nullptr;
}

void AudioChannel::play() {
    swrContext = swr_alloc_set_opts(0,
                                    AV_CH_LAYOUT_STEREO,
                                    AV_SAMPLE_FMT_S16,
                                    44100,
                                    avCodecContext->channel_layout,
                                    avCodecContext->sample_fmt,
                                    avCodecContext->sample_rate,
                                    0, 0);
    swr_init(swrContext);
    isPlaying = true;
    setEnable(true);
    pthread_create(&audioDecodeTask, 0, audioDecode_t, this);
    pthread_create(&audioPlayTask, 0, audioPlay_t, this);
}


void AudioChannel::stop() {
    isPlaying = false;
    helper = nullptr;
    setEnable(false);
    pthread_join(audioDecodeTask, 0);
    pthread_join(audioPlayTask, 0);
    _releaseOpenSL();
    if (swrContext) {
        swr_free(&swrContext);
        swrContext = nullptr;
    }
}

void AudioChannel::decode() {
    AVPacket *avPacket = nullptr;
    while (isPlaying) {
        int ret = pkt_queue.deQueue(avPacket);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        ret = avcodec_send_packet(avCodecContext, avPacket);
        releaseAvPacket(avPacket);
        if (ret < 0) {
            break;
        }
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            break;
        }

        while (frame_queue.size() > 100 && isPlaying) {
            av_usleep(1000 * 10);
        }
        frame_queue.enQueue(frame);

    }


}

void AudioChannel::_play() {

    SLresult  lresult;
    lresult=slCreateEngine(&engineObject,0, nullptr,0, nullptr, nullptr);
    if(SL_RESULT_SUCCESS!=lresult){
        return;
    }
    lresult=(*engineObject)->Realize(engineObject,SL_BOOLEAN_FALSE);


}

int AudioChannel::_getData() {
    int dataSize = 0;
    AVFrame *frame = nullptr;
    while (isPlaying) {
        int ret = frame_queue.deQueue(frame);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        int nb = swr_convert(swrContext, &buffer, bufferCount, (const uint8_t **) frame->data,
                             frame->nb_samples);
        dataSize = nb * out_channels * out_sampleSize;
        clock = frame->pts * av_q2d(time_base);
        break;

    }
    releaseAvFrame(frame);
    return dataSize;
}

void AudioChannel::_releaseOpenSL() {

}




