// ExpertQueue.h
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "ExpertRequest.h"

class ExpertQueue {
public:
    void enqueue(ExpertRequestPtr req) {
        std::unique_lock<std::mutex> lock(mtx_);
        queue_.push(req);
        cv_.notify_one();
    }
    ExpertRequestPtr dequeue() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{ return !queue_.empty(); });
        auto req = queue_.front();
        queue_.pop();
        return req;
    }
private:
    std::queue<ExpertRequestPtr> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
};
