// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
template<typename T>
class HITUP_API ThreadSafeQueue
{
private:
    TQueue<T> queue;
    mutable FCriticalSection mutex;

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    void Push(T value) {
        FScopeLock lock(&mutex);
        queue.Enqueue(std::move(value));
    }

    bool TryPop(T& value) {
        FScopeLock lock(&mutex);
        if (Empty()) {
            return false;
        }
        queue.Dequeue(value);
        return true;
    }

    void DrainQueue(TArray<T>& result)
    {
        FScopeLock lock(&mutex);
        while (!Empty()) {
            T value;
            queue.Dequeue(value);
            result.Add(value);
        }
    }

    bool Empty() const {
        FScopeLock lock(&mutex);
        return queue.IsEmpty();
    }
};
