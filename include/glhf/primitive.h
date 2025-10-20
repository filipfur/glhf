#pragma once

#include "glhf/color.h"
#include "glhf/opengl.h"
#include "glhf/primitive.h"
#include "glhf/shader.h"
#include "glhf/texture.h"
#include <array>
#include <assert.h>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace glhf {

struct Material {
    std::string_view name;
    Color color;
    float metallic;
    float roughness;
    std::unordered_map<uint32_t, Texture *> textures;
};

struct Primitive {
    uint32_t vertexArray;
    std::vector<uint32_t> buffers;
    uint32_t elementBuffer;
    uint32_t count;
    Material *material;

    ~Primitive() { dispose(); }
    Primitive(const Primitive &other) = delete;
    Primitive &operator=(const Primitive &other) = delete;

    Primitive(Primitive &&other)
        : vertexArray{other.vertexArray}, buffers{std::move(other.buffers)},
          elementBuffer{other.elementBuffer}, count{other.count}, material{other.material} {
        other.vertexArray = 0;
        other.elementBuffer = 0;
        other.count = 0;
        other.material = nullptr;
    }
    Primitive &operator=(Primitive &&other) {
        if (this != &other) {
            dispose();
            vertexArray = other.vertexArray;
            buffers = std::move(other.buffers);
            elementBuffer = other.elementBuffer;
            count = other.count;
            material = other.material;

            other.vertexArray = 0;
            other.elementBuffer = 0;
            other.count = 0;
            other.material = nullptr;
        }
        return *this;
    }

    void dispose() {
        if (vertexArray) {
            glDeleteVertexArrays(1, &vertexArray);
        }
        if (!buffers.empty()) {
            glDeleteBuffers(buffers.size(), buffers.data());
        }
        if (elementBuffer) {
            glDeleteBuffers(1, &elementBuffer);
        }
    }

    void bindMaterial(glhf::ShaderProgram *shaderProgram) {
        if (material) {
            if (auto color = shaderProgram->uniform("u_color")) {
                *color << material->color.vec4();
            }
            if (auto metallic = shaderProgram->uniform("u_metallic")) {
                *metallic << material->metallic;
            }
            if (auto metallic = shaderProgram->uniform("u_roughness")) {
                *metallic << material->metallic;
            }
            for (const auto &it : material->textures) {
                glActiveTexture(it.first);
                glBindTexture(GL_TEXTURE_2D, it.second->id);
            }
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
    Primitive() : vertexArray{0}, buffers{}, elementBuffer{0}, count{0}, material{nullptr} {}
    template <std::size_t N> friend struct PrimitiveDescriptor;
};

} // namespace glhf
