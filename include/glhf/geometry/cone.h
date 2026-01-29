#pragma once

#include <glm/glm.hpp>

namespace glhf {
struct Cone {
    glm::vec3 origin;
    glm::vec3 normal;
    float radii;
    float height;
};
} // namespace glhf
