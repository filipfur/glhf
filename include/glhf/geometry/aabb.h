#pragma once

#include <glm/glm.hpp>

namespace glhf {
struct AABB {
    glm::vec3 origin;
    glm::vec3 halfExtents;
};
} // namespace glhf
