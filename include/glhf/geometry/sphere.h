#pragma once

#include "glhf/geometry/geometry.h"
#include <glm/glm.hpp>

namespace glhf {
struct Sphere {
    glm::vec3 origin;
    float radii;

    float distanceTo(const Sphere &sphere) { return glm::distance(origin, sphere.origin); }

    std::optional<Intersection> intersect(const Sphere &sphere) {
        glm::vec3 PX = sphere.origin - origin;
        float dist = glm::sqrt(glm::dot(PX, PX));
        float delta = dist - (radii + sphere.radii);
        if (delta <= 0) {
            glm::vec3 N = PX / dist;
            return Intersection{
                .origin = origin + N * (radii + delta), .normal = -N, .depth = -delta};
        }
        return std::nullopt;
    }
};
} // namespace glhf
