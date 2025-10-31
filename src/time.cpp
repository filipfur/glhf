#include "glhf/time.h"

static glhf::Time::Raw _time{};

float glhf::Time::seconds() { return _time * 1e-3f; }

glhf::Time::Raw glhf::Time::raw() { return _time; }

glhf::Time::Raw glhf::Time::millseconds() { return _time; }

void glhf::Time::increment(glhf::Time::Raw delta_ms) { _time += delta_ms; }