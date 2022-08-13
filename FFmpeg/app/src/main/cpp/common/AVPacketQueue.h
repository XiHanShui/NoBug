//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_AVPACKETQUEUE_H
#define FFMPEGDEMO_AVPACKETQUEUE_H
#include <thread>
extern "C" {
#include <libavcodec/avcodec.h>
};

using namespace std;

typedef struct AVPacketNode {
    AVPacket pkt;
    struct AVPacketNode *next;
} AVPacketNode;

class AVPacketQueue {

public:
    AVPacketQueue();

    virtual  ~AVPacketQueue();

    // 入队数据包
    int pushPacket(AVPacket *pkt);

    // 入队空数据包
    int pushNUllPacket(int stream_index);

    // 刷新
    void flush();

    // 终止
    void abort();

    // 开始
    void start();

    // 获取数据包
    void getPacket(AVPacket *pkt);

    int  getPacket(AVPacket*pkt,int block);

    int getPacketSize();

    int getSize();

    int64_t getDuration();

    int isAbort();


private:

    int put(AVPacket *pkt);

private:
    mutex m_mutex;
    condition_variable m_CondVar;
    AVPacketNode *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int64_t  duration;
    volatile int abort_request;


};


#endif //FFMPEGDEMO_AVPACKETQUEUE_H
