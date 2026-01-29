#pragma once

#include <cstdint>

namespace dex {
template <uint8_t bit, typename T> inline bool is_bit(const T &t) { return t & (1 << bit); }
template <uint8_t bit, typename T> inline void set_bit(T &t) { t |= (1 << bit); }
template <uint8_t bit, typename T> inline void clear_bit(T &t) { t &= ~(1 << bit); }
template <uint8_t bit, typename T> inline void toggle_bit(T &t) { t ^= (1 << bit); }
} // namespace dex
