#pragma once

#include "glhf/color.h"
#include "glhf/opengl.h"
#include "glhf/primitive/primitive.h"
#include "glhf/shader.h"
#include <array>
#include <assert.h>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace glhf {
struct Primitive {
    uint32_t vertexArray;
    std::vector<uint32_t> buffers;
    uint32_t elementBuffer;
    uint32_t count;

    Primitive() : vertexArray{}, buffers{}, elementBuffer{}, count{} {}
    ~Primitive() { dispose(); }
    Primitive(const Primitive &other) = delete;
    Primitive &operator=(const Primitive &other) = delete;

    Primitive(Primitive &&other)
        : vertexArray{other.vertexArray}, buffers{std::move(other.buffers)},
          elementBuffer{other.elementBuffer}, count{other.count} {
        other.vertexArray = 0;
        other.elementBuffer = 0;
        other.count = 0;
    }
    Primitive &operator=(Primitive &&other) {
        if (this != &other) {
            dispose();
            vertexArray = other.vertexArray;
            buffers = std::move(other.buffers);
            elementBuffer = other.elementBuffer;
            count = other.count;

            other.vertexArray = 0;
            other.elementBuffer = 0;
            other.count = 0;
        }
        return *this;
    }

    void dispose() {
        if (vertexArray) {
            glDeleteVertexArrays(1, &vertexArray);
        }
        if (!buffers.empty()) {
            glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
        }
        if (elementBuffer) {
            glDeleteBuffers(1, &elementBuffer);
        }
    }

    void render() {
        glBindVertexArray(vertexArray);
        if (elementBuffer) {
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, count);
        }
        glBindVertexArray(0);
    }

  private:
    template <std::size_t N> friend struct PrimitiveDescriptor;
};
} // namespace glhf
