#pragma once

#include "glhf/mesh.h"
#include "glhf/shader.h"
#include "glhf/skin.h"
#include "glhf/trs.h"
#include <memory>

namespace glhf {
struct Node : public TRS {
    std::string_view name;
    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Skin> skin;
    void render(glhf::ShaderProgram &shaderProgram);

    template <typename RecurPred> void recursive(RecurPred p, int depth = 0) {
        p(this, depth);
        for (const auto &child : children) {
            child->recursive(p, depth + 1);
        }
    }

    template <typename UnaryPred> Node *find(UnaryPred p) {
        if (p(*this)) {
            return this;
        }
        glhf::Node *node = nullptr;
        for (const auto &child : children) {
            node = child->find(p);
            if (node) {
                break;
            }
        }
        return node;
    }

    template <typename UnaryPred> std::shared_ptr<Node> findChild(UnaryPred p) {
        for (const auto &child : children) {
            if (p(*child)) {
                return child;
            } else {
                child->findChild(p);
            }
        }
        return nullptr;
    }
};
} // namespace glhf
