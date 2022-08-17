//
// Created by WJ on 2022/8/10.
//

#include "DecoderBase.h"
#include "LogUtil.h"

void DecoderBase::start() {

}

void DecoderBase::pause() {

}

void DecoderBase::stop() {

}

void DecoderBase::seekToPosition(float position) {

}

float DecoderBase::getCurrentPosition() {
    return 0;
}

int DecoderBase::init(const char *url, AVMediaType mediaType) {
    strcpy(m_Url, url);
    m_MediaType = mediaType;
    return 0;
}

void DecoderBase::unInit() {
    if (m_thread) {
        stop();
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
}

int DecoderBase::initFFDecoder() {
    int result = -1;
    do {
        // 1. 创建封装格式上下文
        m_AVFormatContext = avformat_alloc_context();
        // 2. 打开上下文
        if (avformat_open_input(&m_AVFormatContext, m_Url, nullptr, nullptr) != 0) {
            break;
        }
        // 3. 获取音视频流信息
        if (avformat_find_stream_info(m_AVFormatContext, nullptr) < 0) {
            break;
        }

        // 4. 获取音视频流索引
        for (int i = 0; i < m_AVFormatContext->nb_streams; i++) {
            if (m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }
        if (m_StreamIndex == -1) {
            break;
        }
        // 5. 获取解码器参数
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;

        // 6. 获取解码器
        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (m_AVCodec == nullptr) {
            break;
        }
        // 7.创建解码器上下文
        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            break;
        }
        AVDictionary *pAVDictionary = nullptr;
        av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
        av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
        av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);

        // 8. 打开解码器
        result = avcodec_open2(m_AVCodecContext, m_AVCodec, &pAVDictionary);
        if (result < 0) {
            break;
        }
        result = 0;
        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;// us to ms
        // 创建AVPacket 存放编码数据
        m_AVPacket = av_packet_alloc();
        // 创建AVFrame 存放解码后的数据
        m_AVFrame = av_frame_alloc();
    } while (false);
    if (result != 0 && m_MsgCallback && m_MsgCallback) {
        m_MsgCallback(m_MsgContext, MSG_DECODER_INIT_ERROR, 0);
    }
    return result;
}

void DecoderBase::unInitDecoder() {
    if (m_AVFrame != nullptr) {
        av_frame_free(&m_AVFrame);
        m_AVFrame = nullptr;
    }

    if (m_AVPacket != nullptr) {
        av_packet_free(&m_AVPacket);
        m_AVPacket = nullptr;
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

void DecoderBase::startDecodingThread() {
    m_thread = new thread(doAVDecoding, this);
}

void DecoderBase::decodingLoop() {
    {
        std::unique_lock<mutex> lk(m_Mutex);
        m_DecoderState = STATE_DECODING;
        lk.unlock();
    }
    for (;;) {
        while (m_DecoderState == STATE_PAUSE) {
            std::unique_lock<mutex> lk(m_Mutex);
            m_Cond.wait_for(lk, std::chrono::milliseconds(10));
            m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        }
        if (m_DecoderState == STATE_STOP) {
            break;
        }
        if (m_StartTimeStamp == -1) {
            m_StartTimeStamp = GetSysCurrentTime();
        }
        if (decodeOnePacket() != 0) {
            std::unique_lock<mutex> lk(m_Mutex);
            m_DecoderState = STATE_PAUSE;
        }
    }
}

void DecoderBase::updateTimeStamp() {

}

long DecoderBase::avSync() {
    return 0;
}

int DecoderBase::decodeOnePacket() {
    if (m_SeekPosition > 0) {
        int64_t seek_target = static_cast<int64_t>(m_SeekPosition * 1000000);
        int64_t seek_min = INT64_MIN;
        int64_t seek_max = INT64_MAX;
        int seek_ret = avformat_seek_file(m_AVFormatContext, -1, seek_min, seek_target, seek_max,
                                          0);
        if (seek_ret < 0) {
            m_SeekSuccess = false;
        } else {
            if (m_StreamIndex != -1) {
                avcodec_flush_buffers(m_AVCodecContext);
            }
            cleanCache();
            m_SeekSuccess = true;
        }
    }
    int result = av_read_frame(m_AVFormatContext, m_AVPacket);
    while (result == 0) {
        if (m_AVPacket->stream_index == m_StreamIndex) {
            if (avcodec_send_packet(m_AVCodecContext, m_AVPacket) == AVERROR_EOF) {
                result = -1;
                goto __EXIT;
            }
            int frameCount=0;
            while (avcodec_receive_frame(m_AVCodecContext,m_AVFrame)==0){
                updateTimeStamp();
                avSync();
                onFrameAvailable(m_AVFrame);
                frameCount++;
            }
            if (frameCount>0){
                result=0;
                goto __EXIT;
            }
        }
        av_packet_unref(m_AVPacket);
        result= av_read_frame(m_AVFormatContext,m_AVPacket);
    }


    __EXIT:


    return result;
}

void DecoderBase::doAVDecoding(DecoderBase *decoder) {
    do {
        if (decoder->initFFDecoder() != 0) {
            break;
        }
        decoder->onDecoderReady();
        decoder->decodingLoop();
    } while (false);
    decoder->unInitDecoder();
    decoder->onDecoderDone();
}
