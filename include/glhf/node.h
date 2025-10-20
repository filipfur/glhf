#pragma once

#include "glhf/mesh.h"
#include "glhf/shader.h"
#include "glhf/skin.h"
#include "glhf/trs.h"

namespace glhf {
struct Node : public TRS {
    std::string_view name;
    std::vector<Node *> children;
    Mesh *mesh;
    Skin *skin;
    void render(glhf::ShaderProgram *shaderProgram);
    void recursive(const std::function<void(glhf::Node *, int)> &func, int depth = 0);
    Node *find(const std::function<bool(const glhf::Node &)> &func);
};
} // namespace glhf
