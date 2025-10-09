#ifndef EXPERT_QUEUE_H
#define EXPERT_QUEUE_H

#include "Common.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

// Request structure for expert engineer
struct ExpertRequest {
    RobotInfo robot;
    std::promise<RobotInfo> completion_promise;
    
    ExpertRequest(const RobotInfo& r) : robot(r) {}
    
    // Move constructor and assignment for promise
    ExpertRequest(ExpertRequest&& other) noexcept
        : robot(other.robot),
          completion_promise(std::move(other.completion_promise)) {}
    
    ExpertRequest& operator=(ExpertRequest&& other) noexcept {
        if (this != &other) {
            robot = other.robot;
            completion_promise = std::move(other.completion_promise);
        }
        return *this;
    }
    
    // Delete copy constructor and assignment
    ExpertRequest(const ExpertRequest&) = delete;
    ExpertRequest& operator=(const ExpertRequest&) = delete;
};

class ExpertQueue {
private:
    std::queue<ExpertRequest> requests;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool shutdown;
    
public:
    ExpertQueue() : shutdown(false) {}
    
    // Enqueue a request and return a future for the result
    std::future<RobotInfo> EnqueueRequest(const RobotInfo& robot) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        
        ExpertRequest request(robot);
        std::future<RobotInfo> future = request.completion_promise.get_future();
        
        requests.push(std::move(request));
        cv.notify_one(); // Wake up one expert engineer
        
        return future;
    }
    
    // Dequeue a request (blocks if queue is empty)
    bool DequeueRequest(ExpertRequest& request) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        
        // Wait until there's a request or shutdown signal
        cv.wait(lock, [this] { return !requests.empty() || shutdown; });
        
        if (shutdown && requests.empty()) {
            return false;
        }
        
        request = std::move(requests.front());
        requests.pop();
        
        return true;
    }
    
    // Signal shutdown to all waiting threads
    void Shutdown() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            shutdown = true;
        }
        cv.notify_all();
    }
    
    // Check if queue is empty (for statistics)
    bool IsEmpty() {
        std::lock_guard<std::mutex> lock(queue_mutex);
        return requests.empty();
    }
    
    // Get queue size (for statistics)
    size_t Size() {
        std::lock_guard<std::mutex> lock(queue_mutex);
        return requests.size();
    }
};

#endif // EXPERT_QUEUE_H