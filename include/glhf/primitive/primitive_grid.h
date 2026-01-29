#pragma once

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>

namespace glhf {
template <std::size_t N, std::size_t M> struct PrimitiveGrid {
    static constexpr std::size_t CELLS_N = N + 1;
    static constexpr std::size_t CELLS_M = M + 1;
    static constexpr std::size_t CELLS = CELLS_N * CELLS_M;
    constexpr PrimitiveGrid(float uvScale = 1.0f) : positions{}, normals{}, uvs{}, indices{} {
        for (size_t m{0}; m < CELLS_M; ++m) {
            for (size_t n{0}; n < CELLS_N; ++n) {
                int i = m * CELLS_N + n;
                float s = (float)n / N;
                float t = (float)m / M;
                positions[i] = {
                    s * 2.0f - 1.0f,
                    t * 2.0f - 1.0f,
                    0.0f,
                };
                normals[i] = {0.0f, 0.0f, 1.0f};
                uvs[i] = {s * uvScale, t * uvScale};
            }
        }
        size_t i = 0;
        size_t j = 0;
        for (size_t m{0}; m < M; ++m) {
            for (size_t n{0}; n < N; ++n) {
                indices[i * 6 + 0] = j + 0;
                indices[i * 6 + 1] = j + 1;
                indices[i * 6 + 2] = j + CELLS_N;
                indices[i * 6 + 3] = j + 1;
                indices[i * 6 + 4] = j + CELLS_N + 1;
                indices[i * 6 + 5] = j + CELLS_N;
                ++i;
                ++j;
            }
            ++j;
        }
    }
    glm::vec3 positions[CELLS];
    glm::vec3 normals[CELLS];
    glm::vec2 uvs[CELLS];
    uint16_t indices[CELLS * 6];
};
} // namespace glhf
