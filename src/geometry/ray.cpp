#include "glhf/geometry/ray.h"

#include "glhf/geometry/aabb.h"
#include "glhf/geometry/plane.h"
#include "glhf/geometry/sphere.h"
#include <glm/glm.hpp>

namespace glhf {

// Ray: P = P0 + Vt
// Plane: dot(P, N) - d = 0
// d = dot(P, N)
// dot(P0 + Vt, N) - d = 0 <=> dot(P0, N) + t * dot(V, N) - d = 0
// t = (d - dot(P0, N)) / dot(V, N)

std::optional<Intersection> Ray::intersect(const Plane &plane) {
    float denom = glm::dot(this->direction, plane.normal);
    // don't count backface hits
    if (denom > -FLT_EPSILON) { // && denom < FLT_EPSILON) {
        return std::nullopt;
    }
    float t = (plane.d - glm::dot(this->origin, plane.normal)) / denom;
    return Intersection{
        .origin = this->origin + t * this->direction,
        .normal = plane.normal,
        .depth = t,
    };
}

// ro + rd * t = n * r
std::optional<Intersection> Ray::intersect(const Sphere &sphere) {
    glm::vec3 OC = sphere.origin - origin;
    float t1 = glm::dot(OC, direction);
    if (t1 < 0) {
        return std::nullopt;
    }
    glm::vec3 P = origin + direction * t1;
    glm::vec3 CP = P - sphere.origin;
    float a = glm::sqrt(sphere.radii * sphere.radii - glm::dot(CP, CP));
    glm::vec3 contact = P - direction * a;
    return Intersection{
        .origin = contact,
        .normal = glm::normalize(contact - sphere.origin),
        .depth = a,
    };
}

std::optional<Intersection> Ray::intersect(const AABB &aabb) {

    const glm::vec3 ro = origin - aabb.origin;
    glm::vec3 m = 1.0f / direction;
    glm::vec3 n = m * ro;
    glm::vec3 k = glm::abs(m) * aabb.halfExtents;

    glm::vec3 t1 = -n - k; //(-aabb.halfExtents - ro) / direction;
    glm::vec3 t2 = -n + k; //(+aabb.halfExtents - ro) / direction;
    float tN = glm::max(glm::max(t1.x, t1.y), t1.z);
    size_t maxI = 0;
    float maxF = -FLT_MAX;
    for (size_t i{0}; i < 3; ++i) {
        if (t1[i] > maxF) {
            maxF = t1[i];
            maxI = i;
        }
    }
    float tF = glm::min(glm::min(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0) {
        return std::nullopt;
    }
    t1[0] = t1[1] = t1[2] = 0.0f;
    t1[maxI] = 1.0f;
    return Intersection{
        .origin = origin + direction * tN,
        .normal = t1,
        .depth = tN,
    };
}

Ray Ray::fromWindowCoordinate(const glm::mat4 &projection, const glm::mat4 &view, float windowX,
                              float windowY, float windowWidth, float windowHeight) {
    float x = (2.0f * windowX) / windowWidth - 1.0f;
    float y = (2.0f * windowY) / windowHeight - 1.0f;
    // float z = 1.0f;

    glm::vec4 ray_clip_near(x, y, -1.0, 1.0);
    glm::vec4 ray_clip_far(x, y, 1.0, 1.0);

    glm::mat4 invProj = glm::inverse(projection);
    glm::vec4 ray_eye_near = invProj * ray_clip_near;
    glm::vec4 ray_eye_far = invProj * ray_clip_far;

    ray_eye_near /= ray_eye_near.w;
    ray_eye_far /= ray_eye_far.w;

    glm::mat4 invView = glm::inverse(view);
    glm::vec4 ray_world_near = invView * ray_eye_near;
    glm::vec4 ray_world_far = invView * ray_eye_far;

    return {glm::vec3(ray_world_near), glm::normalize(glm::vec3(ray_world_far - ray_world_near))};
}

} // namespace glhf
