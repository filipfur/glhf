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

    Node clone() const {
        Node node;
        node.name = name;
        node.mesh = mesh;
        node.skin = skin;
        return node;
    }

    template <typename RecurPred> void recursive(RecurPred p, int depth = 0) {
        p(this, depth);
        for (glhf::Node *child : children) {
            child->recursive(p, depth + 1);
        }
    }

    template <typename UnaryPred> Node *find(UnaryPred p) {
        if (p(*this)) {
            return this;
        }
        glhf::Node *node = nullptr;
        for (glhf::Node *child : children) {
            node = node ? node : child->find(p);
        }
        return node;
    }
};
} // namespace glhf
