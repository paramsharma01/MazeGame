#pragma once
#include <SFML/System/Clock.hpp>

class Timer {
private:
    sf::Clock clock;
    float duration;

public:
    Timer(float seconds);
    float getRemainingTime() const;
    bool isTimeUp() const;
    void reset();
};
