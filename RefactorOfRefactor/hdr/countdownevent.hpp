#pragma once
#include "debug.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

class CountDownEvent
{
private:
    std::atomic<int32_t> numberOfRemainingItems;
    std::condition_variable observer;
    std::mutex m;

public:
    void reset(const int32_t size)
    {
        ASSERT(size > 0, "CountDownEvent size must be larger than 0");
        numberOfRemainingItems = size;
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(m);
        observer.wait(lock, [this]() { return numberOfRemainingItems == 0; });
    }

    void signal()
    {
        --numberOfRemainingItems;

        if (numberOfRemainingItems == 0)
        {
            observer.notify_one();
        }
    }
};
