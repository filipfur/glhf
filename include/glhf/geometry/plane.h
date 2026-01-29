#pragma once

#include "glhf/geometry/geometry.h"
#include "glhf/geometry/polygon.h"
#include "glhf/log.h"
#include <glm/glm.hpp>

namespace glhf {
struct Plane {
    glm::vec3 normal;
    float d;
    static constexpr Plane createFromPoint(const glm::vec3 &p, const glm::vec3 &n) {
        return Plane{.normal = n, .d = glm::dot(n, p)};
    }
    constexpr glm::vec3 origin() const { return normal * d; }

    enum PlaneSide {
        BEHIND = -1,
        COPLANAR = 0,
        IN_FRONT = 1,
        STRADDLING = 2,
    };

    float signedDistance(const glm::vec3 &point) const { return glm::dot(normal, point) - d; }

    PlaneSide sideOf(const glm::vec3 &point, float thickness = 1e-6f) const {
        float dist = signedDistance(point);
        // LOGS_TRACE("normal=" << normal << " d=" << d << " point" << point << " dist=" << dist);
        if (dist > thickness) {
            return IN_FRONT;
        } else if (dist < -thickness) {
            return BEHIND;
        }
        return COPLANAR;
    }

    PlaneSide sideOf(const std::vector<glm::vec3> &polygon, float thickness = 1e-6f) {
        PlaneSide rval = COPLANAR;
        assert(!polygon.empty());
        for (const glm::vec3 &p : polygon) {
            auto res = sideOf(p, thickness);
            if (res == IN_FRONT) {
                rval = (rval == BEHIND) ? STRADDLING : IN_FRONT;
            } else if (res == BEHIND) {
                rval = (rval == IN_FRONT) ? STRADDLING : BEHIND;
            }
            if (rval == STRADDLING) {
                break;
            }
        }
        return rval;
    }
};

static_assert(Plane::createFromPoint({4, 0, 0}, {1, 0, 0}).origin() == glm::vec3{4, 0, 0});
} // namespace glhf
