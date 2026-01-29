#pragma once

#include <cstddef>
#include <glm/glm.hpp>

namespace glhf {
template <std::size_t segments, std::size_t discs> struct PrimitiveSphere {
    static constexpr std::size_t CELLS_N = segments + 1;
    static constexpr std::size_t CELLS_M = discs + 1;
    static constexpr std::size_t CELLS = CELLS_N * CELLS_M;

    PrimitiveSphere() : positions{}, normals{}, uvs{}, indices{} {
        float s = 1.0f / static_cast<float>(segments);
        uint16_t vs_i = 0;
        float theta = M_PI / static_cast<float>(discs);

        for (uint16_t j{0}; j <= discs; ++j) {
            float y = -glm::cos(theta * j);
            float f = 1.0 - y * y;
            float r = f < FLT_EPSILON ? 0.0f : glm::sqrt(f) + FLT_EPSILON;
            for (uint16_t i{0}; i <= segments; ++i) {
                float a = 2.0f * M_PI * i * s;
                glm::vec3 v{glm::sin(a) * r, y, glm::cos(a) * r};
                positions[vs_i] = v;
                normals[vs_i] = v;
                uvs[vs_i] = {i * s, y * 0.5f + 0.5f};
                ++vs_i;
            }
        }

        size_t i{0};
        size_t j{0};
        for (uint16_t d{0}; d < discs; ++d) {
            for (uint16_t seg{0}; seg < segments; ++seg) {
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
    uint16_t indices[segments * discs * 6];
};
} // namespace glhf
