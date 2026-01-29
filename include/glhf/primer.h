#pragma once

#include <glm/gtc/quaternion.hpp>
namespace glhf {

inline constexpr glm::vec3 RIGHT = {1.0f, 0.0f, 1.0f};
inline constexpr glm::vec3 UP = {0.0f, 1.0f, 0.0f};
inline constexpr glm::vec3 FORWARD = {0.0f, 0.0f, 1.0f};

inline glm::quat rotationBetween(const glm::vec3 &v0, const glm::vec3 &v1) {
    float dotV0V1 = glm::dot(v0, v1);
    if (dotV0V1 > 0.9999f) {
        return {1.0f, 0.0f, 0.0f, 0.0f};
    } else if (dotV0V1 < -0.9999f) {
        // this orthognalized vector can be null vector and then we goofed
        return glm::angleAxis(glm::pi<float>(), glm::normalize(glm::vec3(
                                                    v0[1] + v0[2], v0[2] - v0[0], -v0[0] - v0[1])));
    }
    glm::vec3 rotationAxis = glm::cross(v0, v1);
    return glm::normalize(
        glm::quat{1.0f + dotV0V1, rotationAxis.x, rotationAxis.y, rotationAxis.z});
}

} // namespace glhf
