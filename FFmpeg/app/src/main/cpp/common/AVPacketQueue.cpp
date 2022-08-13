//
// Created by WJ on 2022/8/10.
//

#include "AVPacketQueue.h"
#include "LogUtil.h"


AVPacketQueue::AVPacketQueue() {
    abort_request = 0;
    first_pkt = nullptr;
    last_pkt = nullptr;
    nb_packets = 0;
    size = 0;
    duration = 0;
}

AVPacketQueue::~AVPacketQueue() {
    AVPacketQueue::abort();
    flush();
}

int AVPacketQueue::pushPacket(AVPacket *pkt) {
    int ret;
    unique_lock<mutex> lk(m_mutex);
    ret = put(pkt);
    m_CondVar.notify_all();
    lk.unlock();
    if (ret < 0) {
        av_packet_unref(pkt);
    }
    return ret;
}

int AVPacketQueue::pushNUllPacket(int stream_index) {
    AVPacket pkt1, *pkt = &pkt1;
    av_init_packet(pkt);
    pkt->data = nullptr;
    pkt->size = 0;
    pkt->stream_index = stream_index;
    return pushPacket(pkt);
}

void AVPacketQueue::flush() {
    AVPacketNode *pkt, *pkt1;
    unique_lock<mutex> lk(m_mutex);
    for (pkt = first_pkt; pkt; pkt = pkt1) {
        pkt1 = pkt->next;
        av_packet_unref(&pkt->pkt);
        av_freep(&pkt);
    }
    last_pkt = nullptr;
    first_pkt = nullptr;
    nb_packets = 0;
    size = 0;
    duration = 0;
    m_CondVar.notify_all();
}

void AVPacketQueue::abort() {
    unique_lock<mutex> lk(m_mutex);
    abort_request = 1;
    m_CondVar.notify_all();
}

void AVPacketQueue::start() {
    unique_lock<mutex> lk(m_mutex);
    abort_request = 0;
    m_CondVar.notify_all();
}

void AVPacketQueue::getPacket(AVPacket *pkt) {
    return getPacket(pkt, 1);
}

int AVPacketQueue::getPacket(AVPacket *pkt, int block) {
    AVPacketNode *pkt1;
    int ret;
    unique_lock<mutex> lk(m_mutex);
    for (;;) {
        if (abort_request) {
            ret = -1;
            break;
        }
        pkt1 = first_pkt;
        if (pkt1) {
            first_pkt = pkt1->next;
            if (!first_pkt) {
                last_pkt = nullptr;
            }
            nb_packets--;
            size -= pkt1->pkt.size + sizeof(*pkt1);
            duration -= pkt1->pkt.duration;
            *pkt = pkt1->pkt;
            av_freep(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            m_CondVar.wait(lk);
        }
    }
    return  ret;
}


int AVPacketQueue::getPacketSize() {
    unique_lock<mutex> lk(m_mutex);
    return  nb_packets;
}

int AVPacketQueue::getSize() {
    unique_lock<mutex>(m_mutex);
    return size;
}

int64_t AVPacketQueue::getDuration() {
    unique_lock<mutex>(m_mutex);
    return duration;
}

int AVPacketQueue::isAbort() {
    unique_lock<mutex>(m_mutex);
    return abort_request;
}

int AVPacketQueue::put(AVPacket *pkt) {
    AVPacketNode *pkt1;
    if (abort_request) {
        return -1;
    }
    pkt1 = (AVPacketNode *) av_malloc(sizeof(AVPacketNode));
    if (!pkt1) {
        return -1;
    }
    pkt1->pkt = *pkt;
    pkt1->next = nullptr;
    if (!last_pkt) {
        first_pkt = pkt1;
    } else {
        last_pkt->next = pkt1;
    }
    last_pkt = pkt1;
    nb_packets++;
    size += pkt1->pkt.size + sizeof(*pkt1);
    duration += pkt1->pkt.duration;
    return 0;
}

