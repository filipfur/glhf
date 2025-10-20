#pragma once

#include <cstdint>

namespace glhf {
struct UniformBuffer {

    static UniformBuffer *create(void *data, uint32_t byteLength);

    operator uint32_t() const { return bindingPoint; }

    UniformBuffer(uint32_t bindingPoint);
    ~UniformBuffer();

    UniformBuffer(const UniformBuffer &other) = delete;
    UniformBuffer &operator=(const UniformBuffer &other) = delete;

    UniformBuffer(UniformBuffer &&other) = delete;
    UniformBuffer &operator=(UniformBuffer &&other) = delete;

    void bind();
    void unbind();
    void bindBufferBase();
    void bufferData(void *data, uint32_t byteLength);
    void bufferSubData(void *data, uint32_t byteLength, uint32_t offset);

  protected:
    uint32_t id;
    uint32_t bindingPoint;
};
} // namespace glhf
