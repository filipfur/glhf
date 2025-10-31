#pragma once

#include "glhf/uniformbuffer.h"
#include <glm/glm.hpp>

namespace glhf {

struct Camera {
    struct Block {
        glm::mat4 u_projection;
        glm::mat4 u_view;
        glm::vec3 u_eye;
        float padding;
    };

    static constexpr glm::vec3 UP{0.0f, 1.0f, 0.0f};

    Camera(const glm::mat4 &projection)
        : block{projection, glm::mat4{1.0f}, glm::vec3{0.0f}, 0.0f}, center{0.0f, 0.0f, 0.0f},
          distance{10.0f}, yaw{0.0f}, pitch{0.0f} {}

    Block block;
    glm::vec3 center;
    float distance;
    float yaw;
    float pitch;

    static glhf::UniformBuffer *UBO;
    static void createUBO();

    void update();
};

} // namespace glhf
