#include "glhf/geometry/geometry.h"

#include "glhf/geometry/types.h"
#include "glhf/log.h"
#include <array>
#include <cassert>
#include <optional>

namespace glhf {
struct Resolver {
    std::optional<Intersection> operator()(auto &&, auto &&) {
        LOG_WARN("unsupported intersection");
        return std::nullopt;
    }
    std::optional<Intersection> operator()(Ray &ray, Plane &plane) { return ray.intersect(plane); }
};

std::optional<Intersection> intersect(Geometry &geometryA, Geometry &geometryB) {
    return std::visit(Resolver{}, geometryA, geometryB);
}
} // namespace glhf