//
// Created by WJ on 2021/5/9.
//

#include "AudioChannel.h"

extern "C" {
#include <libavutil/time.h>
}

void *audioDecode_t(void *args) {

}

void *audioPlay_t(void *args) {

}

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf queue,void  *pContext){
    AudioChannel *audioChannel= static_cast<AudioChannel *>(pContext);




}


AudioChannel::AudioChannel(int channelId, JavaCallHelper *_helper, AVCodecContext *_avCodecContext,
                           const AVRational &base) : BaseChannel(channelId, _helper,
                                                                 _avCodecContext, base) {

//    双声道
    out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
//    每个样本的大小
    out_sampleSize = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
//    采样率
    out_sampleRate = 44100;
    bufferCount = out_sampleRate * out_sampleSize * out_channels;
    buffer = static_cast<uint8_t *>(malloc(bufferCount));
}

AudioChannel::~AudioChannel() {
    free(buffer);
    buffer = nullptr;
}

void AudioChannel::play() {
    swrContext = swr_alloc_set_opts(nullptr,
                                    AV_CH_LAYOUT_STEREO,
                                    AV_SAMPLE_FMT_S16,
                                    44100,
                                    avCodecContext->channel_layout,
                                    avCodecContext->sample_fmt,
                                    avCodecContext->sample_rate,
                                    0, nullptr);
    swr_init(swrContext);
    isPlaying = true;
    setEnable(true);
//    解码
    pthread_create(&audioDecodeTask, nullptr, audioDecode_t, this);
//    播放
    pthread_create(&audioPlayTask, nullptr, audioPlay_t, this);


}

void AudioChannel::stop() {
    isPlaying = false;
    helper = nullptr;
    setEnable(false);
    pthread_join(audioDecodeTask, nullptr);
    pthread_join(audioPlayTask, nullptr);
    _releaseOpenSL();
    if (swrContext) {
        swr_free(&swrContext);
        swrContext = nullptr;
    }
}

void AudioChannel::decode() {
    AVPacket *packet = nullptr;
    while (isPlaying) {
        int ret = pkt_queue.deQueue(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        ret = avcodec_send_packet(avCodecContext, packet);
        releaseAvPacker(packet);
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
        frame_queue.enqueue(frame);
    }


}

void AudioChannel::_Play() {

}

int AudioChannel::_getData() {
    return 0;
}

void AudioChannel::_releaseOpenSL() {

}
