#include "glhf/skin.h"
#include "glhf/node.h"

glhf::UniformBuffer *glhf::Skin::UBO{nullptr};

glhf::Skin::Block glhf::Skin::block{};

void glhf::Skin::updateBoneMatrices(Skin *skin, const glm::mat4 &globalWorldInverse) {
    for (size_t j{0}; j < skin->joints.size() && j < glhf::Skin::MAX_BONES; ++j) {
        block.bones[j] =
            globalWorldInverse * skin->joints.at(j)->model() * skin->inverseBindMatrices[j];
    }
    UBO->bind();
    UBO->bufferData(&block, sizeof(glhf::Skin::Block));
}

void glhf::Skin::createUBO() { UBO = glhf::UniformBuffer::create(&block, sizeof(Block)); }