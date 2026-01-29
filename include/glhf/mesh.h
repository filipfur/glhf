#pragma once

#include "glhf/color.h"
#include "glhf/material.h"
#include "glhf/opengl.h"
#include "glhf/primitive/primitive.h"
#include "glhf/texture.h"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace glhf {

struct Mesh {
    std::string_view name;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::vector<std::shared_ptr<Material>> materials;
    bool hidden{false};
};

template <typename... Primitives>
inline std::shared_ptr<Mesh> createMesh(const std::string_view &name, Primitives &&...primitives) {
    std::shared_ptr<Mesh> mesh{new Mesh()};
    mesh->name = name;
    (mesh->primitives.push_back(std::forward<Primitives>(primitives)), ...);
    return mesh;
}

}; // namespace glhf
