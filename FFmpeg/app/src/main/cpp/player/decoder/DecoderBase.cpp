#include "DecoderBase.h"


int DecoderBase::InitFFDecoder() {
    return 0;
}

void DecoderBase::unInitDecoder() {

}

void DecoderBase::startDecodingThread() {

}

void DecoderBase::decodingLoop() {

}

void DecoderBase::updateTimeStamp() {

}

long DecoderBase::AVSync() {
    return 0;
}

int DecoderBase::decodeOnPacket() {
    return 0;
}

void DecoderBase::doAvDecoding(DecoderBase *decoder) {

}

int DecoderBase::init(const char *url, AVMediaType mediaType) {
    strcpy(m_Url, url);
    m_MediaType = mediaType;
    return 0;
}

void DecoderBase::unInit() {
    if (m_Thread) {
        stop();
        m_Thread->join();
        delete m_Thread;
        m_Thread = nullptr;
    }
}

int DecoderBase::InitFFDecoder() {
    int result = -1;
    do {
        /*1. 创建封装格式上下文*/
        m_AVFormatContext = avformat_alloc_context();
        /*2. 打开文件*/
        if (avformat_open_input(&m_AVFormatContext,m_Url, nullptr, nullptr)!=0){
            LOGCATE("DecoderBase::InitFFDecoder avformat_open_input fail.");
            break;
        }
        /*3.获取音视频流信息*/
        if (avformat_find_stream_info(m_AVFormatContext, nullptr)<0){

        }


    } while (false)


}


