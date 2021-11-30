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
    void updateTimeStamp()

public:
    DecoderBase() {};

    virtual  ~DecoderBase() {};

    virtual void start() {};

    virtual void pause() {};

    virtual void stop() {};

    virtual float getDuration() {

    }


};


#endif //LEARNFFMPEG_DECODERBASE_H