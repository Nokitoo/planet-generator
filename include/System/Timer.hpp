#pragma once

#include <cstdint> // uint64_t

namespace System {

class Timer
{
public:
    Timer();
    ~Timer() = default;

    Timer(const Timer& timer);
    Timer(Timer&& timer);

    Timer& operator=(const Timer& timer);
    Timer& operator=(Timer&& timer);

    void reset();
    float getElapsedTime() const;

private:
    // Time since last reset
    uint64_t _lastReset;
};

} // Namespace System
