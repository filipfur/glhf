#include "glhf/timer.h"

glhf::Timer::Timer() : _duration{}, _expired{} {}

glhf::Timer::Timer(Time::Raw duration) : _duration{duration}, _expired{Time::raw() + duration} {}

bool glhf::Timer::active() const { return _expired; }

glhf::Time::Raw glhf::Timer::timeLeft() const { return _expired - Time::raw(); }

float glhf::Timer::progress() const {
    float x = 1.0f - static_cast<float>(timeLeft()) / static_cast<float>(_duration);
    return x < 0.0f ? 0.0f : x;
}

bool glhf::Timer::elapsed() const { return active() && Time::raw() >= _expired; }

bool glhf::Timer::cancel() {
    if (active()) {
        _expired = 0;
        return true;
    }
    return false;
}

void glhf::Timer::reset() { _expired = Time::millseconds() + _duration; }

void glhf::Timer::set(Time::Raw duration) {
    _duration = duration;
    reset();
}