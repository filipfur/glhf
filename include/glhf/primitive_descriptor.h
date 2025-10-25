#pragma once

#include "glhf/primitive.h"

namespace glhf {

struct AttributerPointer {
    enum Size : uint8_t { SCALAR = 1, VEC2 = 2, VEC3 = 3, VEC4 = 4 };
    uint8_t size;
    uint8_t bitdepth;
    uint16_t type;
};

template <std::size_t N> struct PrimitiveDescriptor {
    std::array<AttributerPointer, N> attributes;
    uint16_t usage;

    Primitive createPrimitive(const std::array<std::span<uint8_t>, N> &data, size_t count) const {
        Primitive primitive;
        _createVertexArray(data, primitive);
        primitive.count = static_cast<uint32_t>(count);
        glBindVertexArray(0);
        return primitive;
    }

    template <typename T = uint16_t>
    Primitive createPrimitive(const std::array<std::span<uint8_t>, N> &data,
                              std::span<T> indices) const {
        Primitive primitive;
        _createVertexArray(data, primitive);
        glGenBuffers(1, &primitive.elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive.elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), usage);
        primitive.count = static_cast<uint32_t>(indices.size());
        glBindVertexArray(0);
        return primitive;
    }

    constexpr operator size_t() const { return N; }

  private:
    void _createVertexArray(const std::array<std::span<uint8_t>, N> &data,
                            Primitive &primitive) const {
        glGenVertexArrays(1, &primitive.vertexArray);
        glBindVertexArray(primitive.vertexArray);
        primitive.buffers.resize(N);
        glGenBuffers(N, primitive.buffers.data());
        for (uint32_t i{0}; i < N; ++i) {
            glBindBuffer(GL_ARRAY_BUFFER, primitive.buffers.at(i));
            glBufferData(GL_ARRAY_BUFFER, data.at(i).size(), data.at(i).data(), usage);
            glVertexAttribPointer(i, attributes.at(i).size, attributes.at(i).type, GL_FALSE,
                                  attributes.at(i).size * attributes.at(i).bitdepth, nullptr);
            glEnableVertexAttribArray(i);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

constexpr PrimitiveDescriptor<1> XYUV{
    {
        AttributerPointer{AttributerPointer::VEC4, sizeof(float), GL_FLOAT}, // xyuv
    },
    GL_STATIC_DRAW};

constexpr PrimitiveDescriptor<3> PositionNormalUV{
    {
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT}, // position
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT}, // normal
        AttributerPointer{AttributerPointer::VEC2, sizeof(float), GL_FLOAT}, // uv
    },
    GL_STATIC_DRAW};

constexpr PrimitiveDescriptor<4> PositionNormalUVTangent{
    {
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT}, // position
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT}, // normal
        AttributerPointer{AttributerPointer::VEC2, sizeof(float), GL_FLOAT}, // uv
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT}, // tangent
    },
    GL_STATIC_DRAW};

constexpr PrimitiveDescriptor<5> PositionNormalUVJointWeight{
    {
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT},           // position
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT},           // normal
        AttributerPointer{AttributerPointer::VEC2, sizeof(float), GL_FLOAT},           // uv
        AttributerPointer{AttributerPointer::VEC4, sizeof(uint8_t), GL_UNSIGNED_BYTE}, // joint
        AttributerPointer{AttributerPointer::VEC4, sizeof(float), GL_FLOAT},           // weight
    },
    GL_STATIC_DRAW};

constexpr PrimitiveDescriptor<6> PositionNormalUVTangentJointWeight{
    {
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT},           // position
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT},           // normal
        AttributerPointer{AttributerPointer::VEC2, sizeof(float), GL_FLOAT},           // uv
        AttributerPointer{AttributerPointer::VEC3, sizeof(float), GL_FLOAT},           // tangent
        AttributerPointer{AttributerPointer::VEC4, sizeof(uint8_t), GL_UNSIGNED_BYTE}, // joint
        AttributerPointer{AttributerPointer::VEC4, sizeof(float), GL_FLOAT},           // weight
    },
    GL_STATIC_DRAW};

} // namespace glhf
