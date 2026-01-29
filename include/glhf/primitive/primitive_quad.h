#pragma once

#include <glm/glm.hpp>

namespace glhf {
struct PrimitiveQuad {
    constexpr PrimitiveQuad(float L=-1.0f, float R=1.0f, float B=-1.0f, float T=1.0f)
            : positions{
                  {L, B, 0.0f},
                  {R, B, 0.0f},
                  {L, T, 0.0f},
                  {R, T, 0.0f},
              },
              normals{
                  {0.0f, 0.0f, 1.0f},
                  {0.0f, 0.0f, 1.0f},
                  {0.0f, 0.0f, 1.0f},
                  {0.0f, 0.0f, 1.0f},
              },
              uvs{
                  {0.0f, 0.0f},
                  {1.0f, 0.0f},
                  {0.0f, 1.0f},
                  {1.0f, 1.0f},
              },
              indices{0, 1, 2, 1, 3, 2} {}
    glm::vec3 positions[4];
    glm::vec3 normals[4];
    glm::vec2 uvs[4];
    uint16_t indices[6];
};
struct PrimitiveQuadXYUV {
    constexpr PrimitiveQuadXYUV(float L=-1.0f, float R=1.0f, float B=-1.0f, float T=1.0f) : vertices{
            {L, B, 0.0f, 0.0f},
            {R, B, 1.0f, 0.0f},
            {L, T, 0.0f, 1.0f},
            {R, T, 1.0f, 1.0f},
        }, indices{0, 1, 2, 1, 3, 2,} {
    }
    glm::vec4 vertices[4];
    uint16_t indices[6];
};
constexpr PrimitiveQuad PrimitiveScreen;
constexpr PrimitiveQuadXYUV PrimitiveScreenXYUV;
} // namespace glhf
