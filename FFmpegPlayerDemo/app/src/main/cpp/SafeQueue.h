//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_SAFEQUEUE_H
#define FFMPEGDEMO_SAFEQUEUE_H

#include "queue"
#include <pthread.h>

using namespace std;

template<typename T>
class SafeQueue {
    typedef void  (*ReleaseHandle)(T &);

    typedef void  (*SyncHandle)(queue<T> &);


public:
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    queue<T> q;
    bool enable;
    ReleaseHandle releaseHandle;
    SyncHandle syncHandle;

public:
    SafeQueue() {
        pthread_cond_init(&cond, nullptr);
        pthread_mutex_init(&mutex, nullptr);
    }

    ~SafeQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void enqueue(T newValue) {
        pthread_mutex_lock(&mutex);
        if (enable) {
            q.push(newValue);
            pthread_cond_signal(&cond);
        } else {
            releaseHandle(newValue);
        }
        pthread_mutex_unlock(&mutex);
    }

    int deQueue(T &value) {
        int ret = 0;
        pthread_mutex_lock(&mutex);
        while (enable && q.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (!q.empty()) {
            value = q.front();
            q.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        return ret;
    }

    void setEnable(bool _enable) {
        pthread_mutex_lock(&mutex);
        this->enable = _enable;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int empty() {
        return q.empty();
    }

    int size() {
        return q.size();
    }

    void clear() {
        pthread_mutex_lock(&mutex);
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            T value = q.front();
            releaseHandle(value);
            q.pop();
        }
        pthread_mutex_unlock(&mutex);
    }


    void sync() {
        pthread_mutex_lock(&mutex);
        syncHandle(q);
        pthread_mutex_unlock(&mutex);
    }

    void setReleaseHandle(ReleaseHandle r) {
        releaseHandle = r;
    }

    void setSyncHandle(SyncHandle s) {
        syncHandle = s;
    }


};


#endif //FFMPEGDEMO_SAFEQUEUE_H
