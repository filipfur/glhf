#include "glhf/time.h"

static glhf::time::Raw _time{};

float glhf::time::seconds() { return _time * 1e-3f; }

glhf::time::Raw glhf::time::raw() { return _time; }

glhf::time::Raw glhf::time::millseconds() { return _time; }

void glhf::time::increment(glhf::time::Raw delta_ms) { _time += delta_ms; }