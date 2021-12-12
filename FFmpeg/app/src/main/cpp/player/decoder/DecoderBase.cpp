#include <LogUtil.h>
#include "DecoderBase.h"

using namespace std;


void DecoderBase::startDecodingThread() {
    m_Thread = new thread(doAvDecoding, this);
}

void DecoderBase::decodingLoop() {

}

void DecoderBase::updateTimeStamp() {

}


long DecoderBase::AVSync() {
    return 0;
}

int DecoderBase::decodeOnPacket() {
    LOGCATE("DecoderBase::DecodeOnePacket m_MediaType=%d", m_MediaType);
    if (m_SeekPosition > 0) {
        auto seek_target = static_cast<int64_t>(m_SeekPosition * 1000000);//微妙
        int64_t seek_min = INT64_MIN;
        int64_t seek_max = INT64_MAX;
        int seek_ret = avformat_seek_file(m_AVFormatContext, -1, seek_min, seek_target, seek_max,
                                          0);
        if (seek_ret < 0) {
            m_SeekSuccess = false;
            LOGCATE("BaseDecoder::DecodeOneFrame error while seeking m_MediaType=%d", m_MediaType);
        } else {
            if (m_StreamIndex != -1) {
                avcodec_flush_buffers(m_AVCodecContext);
            }
            /*清理缓存*/
            clearCache();
            m_SeekSuccess = true;
            LOGCATE("BaseDecoder::DecodeOneFrame seekFrame pos=%f, m_MediaType=%d", m_SeekPosition,
                    m_MediaType);
        }
    }
    int result = av_read_frame(m_AVFormatContext, m_Packet);
    while (result==0){
        if (m_Packet->stream_index==m_StreamIndex){

        }


    }



    return 0;
}

void DecoderBase::doAvDecoding(DecoderBase *decoder) {
    LOGCATE("DecoderBase::DoAVDecoding");
    do {
        if (decoder->InitFFDecoder() != 0) {
            break;
        }
        decoder->onDecoderReady();
        decoder->decodingLoop();
    } while (false);
    decoder->unInitDecoder();
    decoder->onDecoderDone();
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
        if (avformat_open_input(&m_AVFormatContext, m_Url, nullptr, nullptr) != 0) {
            LOGCATE("DecoderBase::InitFFDecoder avformat_open_input fail.");
            break;
        }
        /*3.获取音视频流信息*/
        if (avformat_find_stream_info(m_AVFormatContext, nullptr) < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avformat_find_stream_info fail.");
            break;
        }
        /*4.获取音视频流索引*/
        for (int i = 0; i < m_AVFormatContext->nb_streams; ++i) {
            if (m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }
        if (m_StreamIndex == -1) {
            LOGCATE("DecoderBase::InitFFDecoder Fail to find stream index.");
            break;
        }

        /*5. 获取解码器参数*/
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;

        /*6. 获取解码器*/
        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (m_AVCodec == nullptr) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_find_decoder fail.");
            break;
        }
        /*7. 创建解码器上下文*/
        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_parameters_to_context fail.");
            break;
        }

        AVDictionary *avDictionary = nullptr;
        av_dict_set(&avDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&avDictionary, "stimeout", "20000000", 0);
        av_dict_set(&avDictionary, "max_delay", "30000000", 0);
        av_dict_set(&avDictionary, "rtsp_transport", "tcp", 0);

        /*8. 打开解码器*/
        result = avcodec_open2(m_AVCodecContext, m_AVCodec, &avDictionary);
        if (result < 0) {
            LOGCATE("DecoderBase::InitFFDecoder avcodec_open2 fail. result=%d", result);
            break;
        }
        result = 0;
        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;//us to ms
        /*创建 AVPacket*/
        m_Packet = av_packet_alloc();
        /*创建AVFrame*/
        m_Frame = av_frame_alloc();
    } while (false);

    if (result != 0 && m_MsgContext && m_MessageCallback) {
        m_MessageCallback(m_MsgContext, MSG_DECODER_INIT_ERROR, 0);
    }
    return result;
}


void DecoderBase::unInitDecoder() {
    LOGCATE("DecoderBase::UnInitDecoder");
    if (m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }

    if (m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }

    if (m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }

    if (m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
}


void DecoderBase::start() {
    if (m_Thread == nullptr) {
        startDecodingThread();
    } else {
        unique_lock<mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        m_Cond.notify_all();
    }
}

void DecoderBase::pause() {
    unique_lock<mutex> lock(m_Mutex);
    m_DecoderState = STATE_PAUSE;
}

void DecoderBase::stop() {
    unique_lock<mutex> lock(m_Mutex);
    m_DecoderState = STATE_STOP;
    m_Cond.notify_all();
}

void DecoderBase::seekToPosition(float position) {
    LOGCATE("DecoderBase::SeekToPosition position=%f", position);
    unique_lock<mutex> lock(m_Mutex);
    m_SeekPosition = position;
    m_DecoderState = STATE_DECODING;
    m_Cond.notify_all();
}

float DecoderBase::getCurrentPosition() {
    return m_CurTimeStamp;
}


