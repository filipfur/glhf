#include "glhf/camera.h"

#include <glm/gtc/matrix_transform.hpp>

glhf::UniformBuffer *glhf::Camera::UBO{nullptr};

void glhf::Camera::createUBO() {
    Block block;
    UBO = glhf::UniformBuffer::create(&block, sizeof(Block));
}

void glhf::Camera::update(float dt) {
    (void)dt;
    const glm::vec3 eye =
        center + glm::vec3{glm::sin(yaw) * glm::cos(pitch) * distance, glm::sin(pitch) * distance,
                           glm::cos(yaw) * glm::cos(pitch) * distance};

    block.u_view = glm::lookAt(eye, center, UP);
    block.u_eye = eye;
    UBO->bind();
    // UBO->bufferSubData(&block.u_view,
    //                    sizeof(block.u_view) + sizeof(block.u_eye) + sizeof(block.padding),
    //                    sizeof(block.u_projection));
    UBO->bufferData(&block, sizeof(glhf::Camera::Block));
    UBO->unbind();
}