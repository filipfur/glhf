#pragma once

#include "glhf/time.h"

namespace glhf {
class Timer {
  public:
    Timer();
    Timer(time::Raw duration);

    bool active() const;
    time::Raw timeLeft() const;
    float progress() const;
    bool elapsed() const;
    bool cancel();
    void reset();
    void set(time::Raw duration);

  private:
    time::Raw _duration;
    bool _active;
    time::Raw _expired;
};
} // namespace glhf
