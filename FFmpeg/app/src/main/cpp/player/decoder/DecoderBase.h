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

#define  MAX_PATH 2048
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
        return m_Duration * 1.0F / 1000;
    }

    virtual void seekToPosition(float position);

    virtual float getCurrentPosition();

    virtual void cleanCache() {};

    void setMessageCallback(void *context, MessageCallback callback) override {
        m_MsgContext = context;
        m_MsgCallback = callback;
    }

    virtual void setAVSyncCallback(void *context, AVSyncCallback callback) {
        m_AVDecoderContext = context;
        m_AVSyncCallback = callback;
    }


protected:
    void *m_MsgContext = nullptr;
    MessageCallback m_MsgCallback = nullptr;

    virtual int init(const char *url, AVMediaType mediaType);

    virtual void unInit();

    virtual void onDecoderReady() = 0;

    virtual void onDecoderDone() = 0;

    // 解码数据回调
    virtual void onFrameAvailable(AVFrame *frame) = 0;

    AVCodecContext *getCodecContext() {
        return m_AVCodecContext;
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

    // 封装格式上下文
    AVFormatContext *m_AVFormatContext = nullptr;
    // 解码器上下文
    AVCodecContext *m_AVCodecContext = nullptr;
    // 解码器
    AVCodec *m_AVCodec = nullptr;
    //解码数据包
    AVPacket *m_AVPacket = nullptr;
    //解码帧
    AVFrame *m_AVFrame = nullptr;
    //数据流类型
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    //文件地址
    char m_Url[MAX_PATH] = {0};
    // 当前播放时间
    long m_CurTimeStamp = 0;
    // 播放起始时间
    long m_StartTimeStamp = 0;
    //总时长 ms
    long m_Duration = 0;
    // 数据流索引
    int m_StreamIndex = -1;
    //锁和条件
    mutex m_Mutex;
    condition_variable m_Cond;
    thread *m_thread = nullptr;
    // seek position
    volatile float m_SeekPosition = 0;
    volatile bool m_SeekSuccess = false;
    // 解码器状态
    volatile int m_DecoderState = STATE_UNKNOWN;
    void *m_AVDecoderContext = nullptr;
    // 音视频同步
    AVSyncCallback m_AVSyncCallback = nullptr;

};


#endif //FFMPEGDEMO_DECODERBASE_H
