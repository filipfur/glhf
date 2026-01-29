#pragma once

#include "glhf/material.h"
#include "glhf/primitive/primitive.h"
#include <glm/glm.hpp>
#include <span>

namespace glhf {

template <typename T> struct InstanceFactory {
    InstanceFactory(std::shared_ptr<Primitive> primitive_, std::shared_ptr<Material> material_,
                    size_t N, std::span<T> instances_)
        : primitive{primitive_}, material{material_}, instances{instances_} {
        glBindVertexArray(primitive->vertexArray);
        glGenBuffers(1, &instanceVBO);
        bufferData();
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        setupAttributePointers(N);
        glBindVertexArray(0);
    }

    void bufferData() {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instances.size_bytes(), instances.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~InstanceFactory() { glDeleteBuffers(GL_ARRAY_BUFFER, &instanceVBO); }

    InstanceFactory(const InstanceFactory &other) = delete;
    InstanceFactory &operator=(const InstanceFactory &other) = delete;

    InstanceFactory(InstanceFactory &&other) = delete;
    InstanceFactory &operator=(InstanceFactory &&other) = delete;

    void render(glhf::ShaderProgram &shaderProgram) {
        shaderProgram.use();
        if (material) {
            material->bind(shaderProgram);
        }
        glBindVertexArray(primitive->vertexArray);
        glDrawElementsInstanced(GL_TRIANGLES, primitive->count, GL_UNSIGNED_SHORT, 0,
                                instances.size());
    }

  private:
    void setupAttributePointers(size_t indexOffset);

    std::shared_ptr<Primitive> primitive;
    std::shared_ptr<Material> material;
    std::span<T> instances;
    unsigned int instanceVBO;
};

template <typename T> void InstanceFactory<T>::setupAttributePointers(size_t indexOffset) {
    glEnableVertexAttribArray(indexOffset);
    glVertexAttribPointer(indexOffset, 4, GL_FLOAT, GL_FALSE, sizeof(T), (void *)0);
    glVertexAttribDivisor(indexOffset, 1);
}

template <> void InstanceFactory<glm::mat4>::setupAttributePointers(size_t indexOffset) {
    glEnableVertexAttribArray(indexOffset);
    glVertexAttribPointer(indexOffset, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *)0);
    glEnableVertexAttribArray(indexOffset + 1);
    glVertexAttribPointer(indexOffset + 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(indexOffset + 2);
    glVertexAttribPointer(indexOffset + 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(indexOffset + 3);
    glVertexAttribPointer(indexOffset + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(indexOffset, 1);
    glVertexAttribDivisor(indexOffset + 1, 1);
    glVertexAttribDivisor(indexOffset + 2, 1);
    glVertexAttribDivisor(indexOffset + 3, 1);
}

} // namespace glhf
