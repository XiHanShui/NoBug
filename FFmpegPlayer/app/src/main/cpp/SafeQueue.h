//
// Created by WJ on 2021/4/17.
//

#ifndef FFMPEGPLAYER_SAFEQUEUE_H
#define FFMPEGPLAYER_SAFEQUEUE_H

#include "queue"
#include "pthread.h"

using namespace std;

template<typename T>
class SafeQueue {

    typedef void (*ReleaseHandle)(T &);

    typedef void (*SyncHandle)(queue <T> &);


public:
    SafeQueue() {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    ~SafeQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }


    void enQueue(T newValue) {
        pthread_mutex_lock(&mutex);
        if (mEnable) {
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
        while (mEnable && q.empty()) {
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

    void setEnable(bool enable) {
        pthread_mutex_lock(&mutex);
        mEnable = enable;
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
        int i = 0;
        for (i; i < size; ++i) {
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

    void setSyncHandle(SyncHandle sync) {
        syncHandle = sync;
    }


public:
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    queue <T> q;
    bool mEnable;
    ReleaseHandle releaseHandle;
    SyncHandle syncHandle;


};


#endif //FFMPEGPLAYER_SAFEQUEUE_H
