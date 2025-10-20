#include "glhf/timer.h"

glhf::Timer::Timer() : _duration{}, _active{false}, _expired{} {}

glhf::Timer::Timer(time::Raw duration)
    : _duration{duration}, _active{true}, _expired{time::raw() + duration} {}

bool glhf::Timer::active() const { return _active; }

glhf::time::Raw glhf::Timer::timeLeft() const { return _expired - time::raw(); }

float glhf::Timer::progress() const {
    float x = 1.0f - static_cast<float>(timeLeft()) / static_cast<float>(_duration);
    return x < 0.0f ? 0.0f : x;
}

bool glhf::Timer::elapsed() const { return _active && time::raw() >= _expired; }

bool glhf::Timer::cancel() {
    if (_active) {
        _active = false;
        return true;
    }
    return false;
}

void glhf::Timer::reset() {
    _active = true;
    _expired = time::millseconds() + _duration;
}

void glhf::Timer::set(time::Raw duration) {
    _duration = duration;
    _active = true;
    _expired = time::millseconds() + _duration;
}