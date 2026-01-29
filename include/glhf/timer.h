#pragma once

#include "glhf/time.h"

namespace glhf {
class Timer {
  public:
    Timer();
    Timer(Time::Raw duration);

    operator bool() const { return active(); }

    bool active() const;
    Time::Raw timeLeft() const;
    float progress() const;
    bool elapsed() const;
    bool cancel();
    void reset();
    void set(Time::Raw duration);

  private:
    Time::Raw _duration;
    Time::Raw _expired;
};
} // namespace glhf
