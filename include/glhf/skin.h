#pragma once

#include "glhf/uniformbuffer.h"
#include <glm/glm.hpp>
#include <span>
#include <string_view>

namespace glhf {
struct Skin {
    static constexpr size_t MAX_BONES{100};
    struct Block {
        glm::mat4 bones[MAX_BONES];
    };
    static Block block;
    static UniformBuffer *UBO;
    static void createUBO();
    static void updateBoneMatrices(Skin *skin, const glm::mat4 &globalWorldInverse);
    std::string_view name;
    std::vector<struct Node *> joints;
    std::span<glm::mat4> inverseBindMatrices;
};
} // namespace glhf
