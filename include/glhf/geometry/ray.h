#pragma once

#include "glhf/geometry/geometry.h"
#include <glm/glm.hpp>
#include <optional>

namespace glhf {
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    std::optional<Intersection> intersect(const Plane &plane);
    std::optional<Intersection> intersect(const Sphere &sphere);
    std::optional<Intersection> intersect(const AABB &aabb);

    static Ray fromWindowCoordinate(const glm::mat4 &projection, const glm::mat4 &view,
                                    float windowX, float windowY, float windowWidth,
                                    float windowHeight);
};
} // namespace glhf