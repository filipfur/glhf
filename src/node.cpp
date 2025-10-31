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
