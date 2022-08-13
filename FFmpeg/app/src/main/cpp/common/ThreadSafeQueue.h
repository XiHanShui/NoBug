//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_THREADSAFEQUEUE_H
#define FFMPEGDEMO_THREADSAFEQUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    ThreadSafeQueue(ThreadSafeQueue const &other) {
        std::lock_guard<std::mutex> lk(other.m_mutex);
        m_dataQueue = other.m_dataQueue;
    }

    void push(T newValue) {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_dataQueue.push(newValue);
        m_condVar.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(m_mutex);
        if (empty()) return nullptr;
        T res = m_dataQueue.front();
        m_dataQueue.pop();
        return res;
    }


    bool empty() const {
        return m_dataQueue.empty();
    }

    int size(){
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_dataQueue.size();
    }


private:
    mutable std::mutex m_mutex;
    std::queue<T> m_dataQueue;
    std::condition_variable m_condVar;
};


#endif //FFMPEGDEMO_THREADSAFEQUEUE_H
