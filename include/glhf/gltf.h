#pragma once

#include "glhf/opengl.h"
#include <cassert>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <span>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>

namespace glhf {
struct Gltf {

    using Index_t = uint16_t;
    static constexpr Index_t nullindex = 0xFFFF;

    Gltf() = default;
    explicit Gltf(const char *str);
    void loadGLB(const char *str);

    ~Gltf() = default;

    Gltf(const Gltf &other) = delete;
    Gltf &operator=(const Gltf &other) = delete;

    Gltf(Gltf &&other) = delete;
    Gltf &operator=(Gltf &&other) = delete;

    struct Buffer {
        unsigned char *data;
        size_t length;
    };

    struct BufferView {
        Buffer *buffer;
        uint32_t offset;
        uint32_t length;
        uint16_t target;
        void *data() { return (void *)(buffer->data + offset); }
    };

    struct Accessor {
        BufferView *bufferView;
        uint16_t componentType;
        uint32_t count;
        enum Type : uint8_t { SCALAR, VEC2, VEC3, VEC4, MAT4 } type;
        std::span<uint8_t> bytes() { return {(uint8_t *)bufferView->data(), bufferView->length}; }
        template <typename T> std::span<T> span() {
            return {(T *)bufferView->data(), bufferView->length / sizeof(T)};
        }
    };

    struct Image {
        std::string_view name;
        BufferView *bufferView;
    };

    struct Texture {
        struct Sampler {
            uint32_t magFilter;
            uint32_t minFilter;
        };
        Image *image;
        Sampler *sampler;
    };

    struct Material {
        std::string name;
        glm::vec4 baseColor;
        float metallic;
        float roughness;
        std::vector<Index_t> textures;
    };

    struct Primitive {
        enum Attribute : uint8_t {
            POSITION,
            NORMAL,
            TEXCOORD_0,
            COLOR_0,
            COLOR_1,
            JOINTS_0,
            WEIGHTS_0,
            COUNT,
        };
        std::span<glm::vec3> positions() const {
            assert(attributes[POSITION]->componentType == GL_FLOAT);
            assert(attributes[POSITION]->type == Gltf::Accessor::VEC3);
            return attributes[POSITION]->span<glm::vec3>();
        }
        std::span<glm::vec3> normals() const {
            assert(attributes[NORMAL]->componentType == GL_FLOAT);
            assert(attributes[NORMAL]->type == Gltf::Accessor::VEC3);
            return attributes[NORMAL]->span<glm::vec3>();
        }
        std::span<glm::vec2> texCoords() const {
            assert(attributes[TEXCOORD_0]->componentType == GL_FLOAT);
            assert(attributes[TEXCOORD_0]->type == Gltf::Accessor::VEC2);
            return attributes[TEXCOORD_0]->span<glm::vec2>();
        }
        std::span<glhf::Gltf::Index_t> indices16() const {
            assert(_indices->componentType == GL_UNSIGNED_SHORT);
            return _indices->span<glhf::Gltf::Index_t>();
        }
        std::span<uint32_t> indices32() const {
            assert(_indices->componentType == GL_UNSIGNED_INT);
            return _indices->span<uint32_t>();
        }
        Accessor *attributes[COUNT];
        Accessor *_indices;
        Index_t material;
    };

    struct Mesh {
        std::string name;
        std::vector<Primitive> primitives;
    };

    struct Skin {
        std::string_view name;
        std::vector<Index_t> joints;
        Accessor *inverseBindMatrices;
    };

    struct Animation {
        struct Sampler {
            enum Interpolation { STEP, LINEAR };
            Accessor *input;
            Accessor *output;
            Interpolation interpolation;
        };
        struct Channel {
            enum Path { TRANSLATION, ROTATION, SCALE };
            Sampler *sampler;
            Index_t targetNode;
            Path targetPath;
        };
        std::string_view name;
        std::vector<Sampler> samplers;
        std::vector<Channel> channels;
    };

    struct Node {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
        std::string name;
        Index_t mesh;
        Index_t skin;
        std::vector<Index_t> children;
    };

    struct Scene {
        std::string name;
        std::vector<Index_t> nodes;
    };

    std::vector<Image> images;
    std::vector<Texture::Sampler> textureSamplers;
    std::vector<Texture> textures;
    std::vector<Material> materials;
    std::vector<Buffer> buffers;
    std::vector<BufferView> bufferViews;
    std::vector<Accessor> accessors;
    std::vector<Mesh> meshes;
    std::vector<Skin> skins;
    std::vector<Animation> animations;
    std::vector<Node> nodes;
    Scene scene;
};
} // namespace glhf
