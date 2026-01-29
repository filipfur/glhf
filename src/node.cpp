#include "glhf/node.h"

void glhf::Node::render(glhf::ShaderProgram &shaderProgram) {
    if (skin) {
        glhf::Skin::updateBoneMatrices(*skin, glm::inverse(transformation()));
    }
    for (const auto &child : children) {
        child->render(shaderProgram);
    }
    if (mesh && !mesh->hidden && !mesh->primitives.empty()) {
        shaderProgram.uniforms.at("u_model") << transformation();
        for (size_t i{0}; i < mesh->primitives.size(); ++i) {
            if (i < mesh->materials.size() && mesh->materials[i]) {
                mesh->materials[i]->bind(shaderProgram);
            }
            mesh->primitives[i]->render();
        }
    }
}
