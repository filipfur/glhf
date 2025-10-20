#pragma once

#include <cstdint>

namespace glhf {
namespace bdf {
static const uint16_t GLYPHS_MAX{128};
struct Font {
    uint16_t pointsize;
    uint16_t dpix;
    uint16_t dpiy;
    // bounding box from lower left corner
    uint16_t pw;
    uint16_t ph;
    int16_t px;
    int16_t py;
    // Glyph glyphs[GLYPHS_MAX];
    uint8_t bitmap[GLYPHS_MAX][16];
    void *gpuInstance;
};
} // namespace bdf
} // namespace glhf
