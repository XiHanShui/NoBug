#ifndef LEARNFFMPEG_DECODERBASE_H
#define LEARNFFMPEG_DECODERBASE_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
};

#include <thread>
#include "Decoder.h"
#include "LogUtil.h"


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
    MSG_REQUEST_RENDER,
    MSG_DECODING_TIME
};

class DecoderBase : public Decoder {
private:
    int InitFFDecoder();

    void unInitDecoder();

    /*启动解码线程*/
    void startDecodingThread();

    /*音视频解码循环*/
    void decodingLoop();

    /*更新显示时间戳*/
    void updateTimeStamp();

    /*音视频同步*/
    long AVSync();

    /*解码一个packet编码数据*/
    int decodeOnPacket();

    /*线程函数*/
    static void doAvDecoding(DecoderBase *decoder);

    /*封装格式上下文*/
    AVFormatContext *m_AVFormatContext = nullptr;
    /*解码器上下文*/
    AVCodecContext *m_AVCodecContext = nullptr;
    /*解码器*/
    AVCodec *m_AVCodec = nullptr;
    /*解码数据包*/
    AVPacket *m_Packet = nullptr;
    /*解码帧*/
    AVFrame *m_Frame = nullptr;
    /*数据流类型*/
    AVMediaType m_MediaType = AVMEDIA_TYPE_UNKNOWN;
    /*文件地址*/
    char m_Url[MAX_PATH] = {0};
    /*当前播放时间*/
    long m_CurTimeStamp = 0;
    /*播放起始时间*/
    long m_StartTimeStamp = -1;
    /*总时长*/
    long m_Duration = 0;
    /*数据流索引*/
    int m_StreamIndex = -1;
    /*锁和条件变量*/
    mutex m_Mutex;
    condition_variable  m_Cond;
    thread *m_Thread = nullptr;
    /*seek position*/
    volatile float m_SeekPosition = 0;
    volatile bool m_SeekSuccess = false;
    /*解码器状态*/
    volatile int m_DecoderState = STATE_UNKNOWN;
    void *m_AVDecoderContext = nullptr;
    /*音视频同步*/
    AVSyncCallback m_AvSyncCallback = nullptr;


protected:
    void *m_MsgContext = nullptr;
    MessageCallback m_MessageCallback = nullptr;

    virtual int init(const char *url, AVMediaType mediaType);

    virtual void unInit();

    virtual void onDecoderReady() = 0;

    virtual void onDecoderDone() = 0;

    virtual void onFrameAvailable(AVFrame *frame) = 0;

    AVCodecContext *getCodecContext() {
        return m_AVCodecContext;
    }


public:
    DecoderBase() {};

    virtual  ~DecoderBase() {};

    /*开始播放*/
    virtual void start() ;

    /*暂停播放*/
    virtual void pause() ;

    /*停止播放*/
    virtual void stop() ;

    /*获取时长*/
    virtual float getDuration() {
        return m_Duration * 1.0f / 1000;
    }

    /*seek 到某个时间点播放*/
    virtual void seekToPosition(float position);

    /*当前播放位置,用于更新进度条和音视频同步*/
    virtual float getCurrentPosition();

    /*清理解码缓存*/
    virtual void clearCache() {};

    /*设置消息回调*/
    virtual void setMessageCallback(void *context, AVSyncCallback callback) {
        this->m_MsgContext = context;
        this->m_AvSyncCallback = callback;
    }

};


#endif //LEARNFFMPEG_DECODERBASE_H