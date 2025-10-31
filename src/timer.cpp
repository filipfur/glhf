#include "glhf/timer.h"

glhf::Timer::Timer() : _duration{}, _active{false}, _expired{} {}

glhf::Timer::Timer(Time::Raw duration)
    : _duration{duration}, _active{true}, _expired{Time::raw() + duration} {}

bool glhf::Timer::active() const { return _active; }

glhf::Time::Raw glhf::Timer::timeLeft() const { return _expired - Time::raw(); }

float glhf::Timer::progress() const {
    float x = 1.0f - static_cast<float>(timeLeft()) / static_cast<float>(_duration);
    return x < 0.0f ? 0.0f : x;
}

bool glhf::Timer::elapsed() const { return _active && Time::raw() >= _expired; }

bool glhf::Timer::cancel() {
    if (_active) {
        _active = false;
        return true;
    }
    return false;
}

void glhf::Timer::reset() {
    _active = true;
    _expired = Time::millseconds() + _duration;
}

void glhf::Timer::set(Time::Raw duration) {
    _duration = duration;
    _active = true;
    _expired = Time::millseconds() + _duration;
}