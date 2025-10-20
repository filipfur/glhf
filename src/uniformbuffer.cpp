#include "glhf/uniformbuffer.h"
#include "glhf/log.h"
#include "glhf/opengl.h"
#include <cassert>
#include <list>

static std::list<glhf::UniformBuffer> _uniformBuffers;

glhf::UniformBuffer *glhf::UniformBuffer::create(void *data, uint32_t byteLength) {
    auto &ubo = _uniformBuffers.emplace_back(_uniformBuffers.size());
    ubo.bind();
    ubo.bufferData(data, byteLength);
    ubo.unbind();
    return &ubo;
}

glhf::UniformBuffer::UniformBuffer(uint32_t bindingPoint_) : bindingPoint{bindingPoint_} {
    glGenBuffers(1, &id);
}

glhf::UniformBuffer::~UniformBuffer() { glDeleteBuffers(1, &id); }

void glhf::UniformBuffer::bindBufferBase() {
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);
}

void glhf::UniformBuffer::bind() { glBindBuffer(GL_UNIFORM_BUFFER, id); }

void glhf::UniformBuffer::unbind() { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

void glhf::UniformBuffer::bufferData(void *data, uint32_t byteLength) {
    assert(byteLength % 16 == 0);
    glBufferData(GL_UNIFORM_BUFFER, byteLength, data, GL_DYNAMIC_DRAW);
}

void glhf::UniformBuffer::bufferSubData(void *data, uint32_t byteLength, uint32_t offset) {
    glBufferSubData(GL_UNIFORM_BUFFER, offset, byteLength, data);
}