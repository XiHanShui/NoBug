//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_AUDIODECODER_H
#define FFMPEGDEMO_AUDIODECODER_H
extern "C"{
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audio_fifo.h>
};
#include "render/audio/AudioRender.h"
#include "SingleAudioRecoder.h"
#include "Decoder.h"
#include "DecoderBase.h"

// 音频编码采样率
static  const int AUDIO_DST_SAMPLE_RATE=44100;
// 音频编码通道数
static  const int AUDIO_DST_CHANNEL_COUNTS=2;
// 音频编码声道格式
static  const  uint64_t  AUDIO_DST_CHANNEL_LAYOUT=AV_CH_LAYOUT_STEREO;
// 音频编码比特率
static  const int AUDIO_DST_BIT_RATE=64000;
// ACC音频一帧采样率
static  const int ACC_NB_SAMPLES=1024;


class AudioDecoder {

};


#endif //FFMPEGDEMO_AUDIODECODER_H