#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace glhf {
template <std::size_t segments> struct PrimitiveCylinder {
    static constexpr size_t num_vertices = segments * 4 + 2;

    PrimitiveCylinder(float radii = 1.0f, float height = 1.0f)
        : positions{}, normals{}, uvs{}, indices{} {
        size_t i{0};
        size_t idx_ring_b = segments + 2;
        size_t idx_ring_c = segments + 2 + segments;
        size_t idx_ring_d = segments + 2 + segments + segments;
        for (; i < segments; ++i) {
            float theta = (float)i / segments * 2.0f * glm::pi<float>();

            positions[i] = {glm::cos(theta) * radii, glm::sin(theta) * radii, height};
            normals[i] = {0.0f, 0.0f, 1.0f};
            uvs[i] = {glm::cos(theta) * 0.5f + 0.5f, glm::sin(theta) * 0.5f + 0.5f};

            positions[i + idx_ring_b] = {positions[i][0], positions[i][1], -positions[i][2]};
            normals[i + idx_ring_b] = {normals[i][0], normals[i][1], -normals[i][2]};
            uvs[i + idx_ring_b] = {1.0f - uvs[i][0], 1.0f - uvs[i][1]};

            positions[i + idx_ring_c] = positions[i];
            uvs[i + idx_ring_c] = uvs[i];

            positions[i + idx_ring_d] = positions[i + idx_ring_b];
            uvs[i + idx_ring_d] = uvs[i + idx_ring_b];
        }

        positions[i] = {0.0f, 0.0f, height};
        normals[i] = {0.0f, 0.0f, 1.0f};
        uvs[i] = {0.0f, 0.0f};
        ++i;
        positions[i] = {0.0f, 0.0f, -height};
        normals[i] = {0.0f, 0.0f, -1.0f};
        uvs[i] = {0.0f, 0.0f};
        i = 0;
        for (size_t j{0}; j < segments; ++j) {
            indices[i++] = j;
            indices[i++] = (j + 1) % segments;
            indices[i++] = segments;
        }
        for (size_t j{0}; j < segments; ++j) {
            indices[i++] = ((j + 1) % segments) + idx_ring_b;
            indices[i++] = j + idx_ring_b;
            indices[i++] = segments + 1;
        }
        uint16_t i0, i1, i2;
        for (size_t j{0}; j < segments; ++j) {
            i0 = idx_ring_c + j;
            i1 = idx_ring_d + j;
            i2 = idx_ring_c + (j + 1) % segments;
            indices[i++] = i0;
            indices[i++] = i1;
            indices[i++] = i2;
            const glm::vec3 &a = positions[i0];
            const glm::vec3 &b = positions[i1];
            const glm::vec3 &c = positions[i2];
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 n = glm::normalize(glm::cross(ab, ac));
            normals[i0] = n;
            normals[i1] = n;
            normals[i2] = n;
        }
        for (size_t j{0}; j < segments; ++j) {
            i0 = idx_ring_d + (j + 1) % segments;
            i1 = idx_ring_c + (j + 1) % segments;
            i2 = idx_ring_d + j;
            indices[i++] = i0;
            indices[i++] = i1;
            indices[i++] = i2;
            const glm::vec3 &a = positions[i0];
            const glm::vec3 &b = positions[i1];
            const glm::vec3 &c = positions[i2];
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 n = glm::normalize(glm::cross(ab, ac));
            normals[i0] = n;
            normals[i1] = n;
            normals[i2] = n;
        }
    }

    glm::vec3 positions[num_vertices];
    glm::vec3 normals[num_vertices];
    glm::vec2 uvs[num_vertices];
    uint16_t indices[segments * 3 * 2 * 2];
};
} // namespace glhf
