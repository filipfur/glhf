#pragma once

#include <cstdint>

namespace glhf {
class Time {
  public:
    using Raw = uint32_t;

    static Raw raw();
    static Raw millseconds();
    static float seconds();

    template <Raw hz> static constexpr Raw fromHz() {
        static_assert(1000 % hz == 0);
        return 1000 / hz;
    }

    static constexpr Raw fromSeconds(float s) { return static_cast<Raw>(s * 1e3f); }

    static constexpr Raw fromMilliseconds(Raw ms) { return ms; }

    static constexpr float toSeconds(Raw raw) { return raw * 1e-3f; }

    static void increment(Raw delta_ms);
}; // namespace time

} // namespace glhf