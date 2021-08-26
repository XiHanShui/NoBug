//
// Created by WJ on 2021/5/9.
//

#include "AudioChannel.h"

extern "C" {
#include <libavutil/time.h>
}

void *audioDecode_t(void *args) {
    auto *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->decode();
    return nullptr;
}

void *audioPlay_t(void *args) {
    auto *audioChannel = static_cast<AudioChannel *>(args);
    audioChannel->_Play();
    return nullptr;
}

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf queue, void *pContext) {
    auto *audioChannel = static_cast<AudioChannel *>(pContext);
    int dataSize = audioChannel->_getData();
    if (dataSize > 0) {
        (*queue)->Enqueue(queue, audioChannel->buffer, dataSize);
    }
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
    /*1. 创建引擎*/
    SLresult lresult;
    lresult = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    if (SL_RESULT_SUCCESS != lresult) {
        return;
    }
    lresult = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != lresult) {
        return;
    }
    lresult = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);

    if (SL_RESULT_SUCCESS != lresult) {
        return;
    }
    /*创建混音器*/
    lresult = (*engineInterface)->CreateOutputMix(engineInterface, &outputMixObject, 0, nullptr,
                                                  nullptr);
    if (SL_RESULT_SUCCESS != lresult) {
        return;
    }
    SLDataLocator_AndroidSimpleBufferQueue androidQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                           2};

    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT,
                            SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource slDataSource = {&androidQueue, &pcm};
// 设置混音器
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, nullptr};
// 需要的接口
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    (*engineInterface)->CreateAudioPlayer(engineInterface, &bqPlayerObject, &slDataSource,
                                          &audioSnk, 1, ids, req);
    (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);

    /*开始播放*/
    ((*bqPlayerObject))->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPLayerBufferQueue);
//    设置回调(启动播放器后执行回调来获取数据并播放)
    (*bqPLayerBufferQueue)->RegisterCallback(bqPLayerBufferQueue, bqPlayerCallback, this);

//    获取播放状态接口
    (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerInterface);
//     设置播放状态
    (*bqPlayerInterface)->SetPlayState(bqPlayerInterface, SL_PLAYSTATE_PLAYING);
    bqPlayerCallback(bqPLayerBufferQueue, this);

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
        int nb = swr_convert(swrContext, &buffer, bufferCount, (const uint8_t **) (frame->data),
                             frame->nb_samples);
        dataSize = nb * out_channels * out_sampleSize;

        clock = frame->pts * av_q2d(time_Base);
        break;
    }
    releaseAvFrame(frame);
    return dataSize;
}

void AudioChannel::_releaseOpenSL() {
/*设置停止状态*/
    if (bqPlayerInterface) {
        (*bqPlayerInterface)->SetPlayState(bqPlayerInterface, SL_PLAYSTATE_STOPPED);
        bqPlayerInterface = nullptr;
    }
//    销毁播放器
    if (bqPlayerObject) {
        (*bqPlayerObject)->Destroy(bqPlayerObject);
        bqPlayerObject = nullptr;
        bqPLayerBufferQueue = nullptr;
    }
//    销毁混音器
    if (outputMixObject) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = nullptr;
    }
//    销毁引擎
    if (engineObject) {
        (*engineObject)->Destroy(engineObject);
        engineObject = nullptr;
        engineInterface = nullptr;
    }


}
