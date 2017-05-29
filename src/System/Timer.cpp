#include <SDL.h> // SDL_GetPerformanceCounter

#include <System/Timer.hpp> // System::Timer

namespace System {

Timer::Timer() {
    reset();
}

Timer::Timer(const Timer& timer) {
    _lastReset = timer._lastReset;
}

Timer::Timer(Timer&& timer) {
    _lastReset = timer._lastReset;
}

Timer& Timer::operator=(const Timer& timer) {
    _lastReset = timer._lastReset;

    return *this;
}

Timer& Timer::operator=(Timer&& timer) {
    _lastReset = timer._lastReset;

    return *this;
}

void    Timer::reset() {
    _lastReset = SDL_GetPerformanceCounter();
}

float   Timer::getElapsedTime() const {
    uint64_t freq = SDL_GetPerformanceFrequency();
    return static_cast<float>(SDL_GetPerformanceCounter() - _lastReset) / static_cast<float>(freq);
}

} // Namespace System
