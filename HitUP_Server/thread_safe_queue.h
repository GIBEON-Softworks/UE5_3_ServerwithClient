#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable condVar;

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    void Push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
        condVar.notify_one();
    }

    bool TryPop(T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    void WaitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.wait(lock, [this] { return !queue.empty(); });
        value = std::move(queue.front());
        queue.pop();
    }

    void DrainQueue(std::vector<T>& result) 
    {
        std::lock_guard<std::mutex> lock(mutex);
        while (!queue.empty()) {
            result.push_back(std::move(queue.front()));
            queue.pop();
        }
    }

    bool Empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};
