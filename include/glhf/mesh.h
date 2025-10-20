#pragma once

#include "glhf/color.h"
#include "glhf/opengl.h"
#include "glhf/primitive.h"
#include "glhf/texture.h"
#include <string_view>
#include <unordered_map>
#include <vector>

namespace glhf {

struct Mesh {
    std::string_view name;
    std::vector<Primitive> primitives;
    bool hidden{false};
};

}; // namespace glhf
