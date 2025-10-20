#include "glhf/node.h"

static void _invalidateRecursive(glhf::Node *node) {
    node->invalidate();
    for (glhf::Node *child : node->children) {
        _invalidateRecursive(child);
    }
}

void glhf::Node::render(glhf::ShaderProgram *shaderProgram) {
    if (parent() == nullptr && !isValid()) {
        _invalidateRecursive(this);
    }
    if (skin) {
        glhf::Skin::updateBoneMatrices(skin, glm::inverse(model()));
    }
    for (Node *child : children) {
        child->render(shaderProgram);
    }
    if (mesh && !mesh->hidden && !mesh->primitives.empty()) {
        shaderProgram->uniforms.at("u_model") << model();
        for (auto &primitive : mesh->primitives) {
            primitive.bindMaterial(shaderProgram);
            primitive.render();
        }
    }
}

void glhf::Node::recursive(const std::function<void(glhf::Node *, int)> &func, int depth) {
    func(this, depth);
    for (glhf::Node *child : children) {
        child->recursive(func, depth + 1);
    }
}

glhf::Node *glhf::Node::find(const std::function<bool(const glhf::Node &)> &func) {
    if (func(*this)) {
        return this;
    }
    glhf::Node *node = nullptr;
    for (glhf::Node *child : children) {
        node = node ? node : child->find(func);
    }
    return node;
}