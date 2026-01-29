#pragma once

#include "glhf/geometry/geometry.h"
#include "glhf/geometry/ray.h"
#include <glm/glm.hpp>
#include <optional>

namespace glhf {
struct Line : public Ray {
    float magnitude;
};
} // namespace glhf