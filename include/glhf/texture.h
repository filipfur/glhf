#pragma once

#include "glhf/bdf.h"
#include "glhf/opengl.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace glhf {

template <uint32_t W, uint32_t H, uint8_t C, typename T = uint8_t> struct Pattern {
    constexpr Pattern(glm::vec4 (*func)(uint32_t x, uint32_t y)) : data{} {
        size_t i{0};
        for (size_t y{0}; y < height; ++y) {
            for (size_t x{0}; x < width; ++x) {
                glm::vec4 pixel = func(x, y);
                for (size_t b{0}; b < C; ++b) {
                    data[i * channels + b] = static_cast<T>(pixel[b] * 255);
                }
                ++i;
            }
        }
    }
    const uint32_t width = W;
    const uint32_t height = H;
    const uint8_t channels = C;
    T data[W * H * C];
};

struct Texture {
    enum Channels {
        R = 1,
        RG = 2,
        RGB = 3,
        RGBA = 4,
        DS = 0x12,
    };
    uint32_t id;

    Texture(const uint8_t *data, uint32_t width, uint32_t height, Channels channels, uint32_t type);
    Texture(const char *filepath, bool flip, uint32_t type = GL_UNSIGNED_BYTE);
    Texture(const uint8_t *addr, uint32_t len, bool flip, uint32_t type = GL_UNSIGNED_BYTE);
    Texture(const bdf::Font &font, uint32_t type = GL_UNSIGNED_BYTE);
    ~Texture();

    Texture(const Texture &other) = delete;
    Texture &operator=(const Texture &other) = delete;

    Texture(Texture &&other);
    Texture &operator=(Texture &&other);

    void dispose();

    void bind();
    void unbind();

  private:
    void create();
    void load(const uint8_t *data, uint32_t width, uint32_t height, Channels channels,
              uint32_t type);
};
} // namespace glhf