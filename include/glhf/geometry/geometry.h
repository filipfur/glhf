#pragma once

#include <glm/glm.hpp>
#include <optional>

namespace glhf {
struct Plane;
struct Ray;
struct Sphere;
struct AABB;
struct Cone;
struct Cylinder;

struct Intersection {
    glm::vec3 origin;
    glm::vec3 normal;
    float depth;
};

using Geometry = std::variant<Plane, Ray, Sphere, AABB, Cone, Cylinder>;

std::optional<Intersection> intersect(Geometry &a, Geometry &b);
} // namespace glhf
