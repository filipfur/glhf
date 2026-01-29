#pragma once

#include <cstddef>
#include <glm/glm.hpp>

namespace glhf {
struct PrimitiveCube {

    static constexpr std::size_t NUM_VERTICES = 6 * 4;
    static constexpr std::size_t NUM_INDICES = 6 * 6;

    constexpr PrimitiveCube() : positions{}, normals{}, uvs{}, indices{0, 1, 2, 0, 3, 2} {
        size_t i{0};
        size_t j{0};
        for (glm::vec3 normal : {
                 glm::vec3{1.0f, 0.0f, 0.0f},
                 glm::vec3{0.0f, 1.0f, 0.0f},
                 glm::vec3{0.0f, 0.0f, 1.0f},
                 glm::vec3{-1.0f, 0.0f, 0.0f},
                 glm::vec3{0.0f, -1.0f, 0.0f},
                 glm::vec3{0.0f, 0.0f, -1.0f},
             }) {
            glm::vec3 right =
                glm::cross(normal.y * normal.y < FLT_EPSILON ? glm::vec3{0.0f, 1.0f, 0.0f}
                                                             : glm::vec3{1.0f, 0.0f, 0.0f},
                           normal);
            glm::vec3 up = glm::cross(normal, right);
            positions[i + 0] = normal - right - up;
            normals[i + 0] = normal;
            uvs[i + 0] = {0.0f, 0.0f};

            positions[i + 1] = normal + right - up;
            normals[i + 1] = normal;
            uvs[i + 1] = {1.0f, 0.0f};

            positions[i + 2] = normal - right + up;
            normals[i + 2] = normal;
            uvs[i + 2] = {0.0f, 1.0f};

            positions[i + 3] = normal + right + up;
            normals[i + 3] = normal;
            uvs[i + 3] = {1.0f, 1.0f};

            indices[j + 0] = i + 0;
            indices[j + 1] = i + 1;
            indices[j + 2] = i + 2;
            indices[j + 3] = i + 1;
            indices[j + 4] = i + 3;
            indices[j + 5] = i + 2;

            i += 4;
            j += 6;
        }
    }

    glm::vec3 positions[NUM_VERTICES];
    glm::vec3 normals[NUM_VERTICES];
    glm::vec2 uvs[NUM_VERTICES];
    uint16_t indices[NUM_INDICES];
};
} // namespace glhf
