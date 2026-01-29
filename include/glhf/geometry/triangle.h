#pragma once

#include <cstdint>

namespace glhf {

struct Triangle {
    Triangle(uint16_t a_, uint16_t b_, uint16_t c_) : a{a_}, b{b_}, c{c_} {}
    uint16_t &operator[](int i) { return *(&a + i); }
    const uint16_t &operator[](int i) const { return *(&a + i); }
    uint16_t a;
    uint16_t b;
    uint16_t c;
};

} // namespace glhf
