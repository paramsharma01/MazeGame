#include "../include/Timer.hpp"
#include <algorithm>

Timer::Timer(float seconds) : duration(seconds) {}

float Timer::getRemainingTime() const {
    float elapsed = clock.getElapsedTime().asSeconds();
    return std::max(0.0f, duration - elapsed);
}

bool Timer::isTimeUp() const {
    return getRemainingTime() <= 0.0f;
}

void Timer::reset() {
    clock.restart();
}
