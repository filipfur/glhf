#include "glhf/gltf.h"

#include "glhf/json.h"
#include <cassert>

namespace {
static constexpr size_t GLB_MAGIC = 0x46546C67;
static constexpr size_t GLB_CHUNK_TYPE_JSON = 0x4E4F534A;
static constexpr size_t GLB_CHUNK_TYPE_BIN = 0x004E4942;

struct glb_header {
    uint32_t magic;
    uint32_t version;
    uint32_t length;
};

struct glb_chunk {
    uint32_t length;
    uint32_t type;
};

void _parseJson(glhf::Gltf *gltf, const char *str, size_t len) {
    glhf::Json json;
    json.loadString(str);

    // std::cout << json << std::endl;

    const auto &gltfImages = json["images"];
    const auto &gltfTextureSamplers = json["textureSamplers"];
    const auto &gltfTextures = json["textures"];
    const auto &gltfMaterials = json["materials"];
    const auto &gltfBuffers = json["buffers"];
    const auto &gltfBufferViews = json["bufferViews"];
    const auto &gltfAccessors = json["accessors"];
    const auto &gltfSkins = json["skins"];
    const auto &gltfAnimations = json["animations"];
    const auto &gltfNodes = json["nodes"];
    const auto &gltfMeshes = json["meshes"];
    const auto &gltfScene = json["scenes"][json["scene"]];

    std::cout << "loading scene: " << gltfScene["name"].value << "\n";

    gltf->scene.name.assign(gltfScene["name"].value);

    gltf->images.reserve(gltfImages.size());
    gltf->textureSamplers.reserve(gltfTextureSamplers.size());
    gltf->textures.reserve(gltfTextures.size());
    gltf->materials.reserve(gltfMaterials.size());
    gltf->buffers.reserve(gltfBuffers.size());
    gltf->bufferViews.reserve(gltfBufferViews.size());
    gltf->accessors.reserve(gltfAccessors.size());
    gltf->meshes.reserve(gltfMeshes.size());
    gltf->nodes.reserve(gltfNodes.size());
    gltf->skins.reserve(gltfSkins.size());
    gltf->animations.reserve(gltfAnimations.size());

    for (const auto &gltfImage : gltfImages) {
        auto &image = gltf->images.emplace_back();
        image.bufferView = &gltf->bufferViews[gltfImage["bufferView"]];
        image.name = gltfImage["name"].value;
    }

    for (const auto &gltfTextureSampler : gltfTextureSamplers) {
        auto &textureSampler = gltf->textureSamplers.emplace_back();
        textureSampler.minFilter = gltfTextureSampler["minFilter"];
        textureSampler.magFilter = gltfTextureSampler["magFilter"];
    }

    for (const auto &gltfTexture : gltfTextures) {
        auto &texture = gltf->textures.emplace_back();
        texture.image = &gltf->images[gltfTexture["source"]];
        texture.sampler = &gltf->textureSamplers[gltfTexture["sampler"]];
    }

    for (const auto &gltfMaterial : gltfMaterials) {
        auto &material = gltf->materials.emplace_back();
        material.name = gltfMaterial["name"];
        if (const auto &pbr = gltfMaterial["pbrMetallicRoughness"]) {
            if (const auto &color = pbr["baseColorFactor"]) {
                material.baseColor = {static_cast<float>(color[0]), static_cast<float>(color[1]),
                                      static_cast<float>(color[2]), static_cast<float>(color[3])};
            } else {
                material.baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
            }
            if (const auto &tex = pbr["baseColorTexture"]) {
                material.textures.push_back(tex["index"]);
            }
            if (const auto &tex = pbr["metallicRoughnessTexture"]) {
                material.textures.push_back(tex["index"]);
            }
            material.metallic = pbr.get<float>("metallicFactor");
            material.roughness = pbr.get<float>("roughnessFactor");
        }
        if (const auto &tex = gltfMaterial["normalTexture"]) {
            material.textures.push_back(tex["index"]);
        }
    }

    for (const auto &gltfBuffer : gltfBuffers) {
        auto &buffer = gltf->buffers.emplace_back();
        buffer.length = gltfBuffer["byteLength"];
    }

    for (const auto &gltfBufferView : gltfBufferViews) {
        auto &bufferView = gltf->bufferViews.emplace_back();
        bufferView.buffer = &gltf->buffers[gltfBufferView["buffer"]];
        bufferView.length = gltfBufferView.get<uint32_t>("byteLength");
        bufferView.offset = gltfBufferView.get<uint32_t>("byteOffset");
        bufferView.target = gltfBufferView.get<uint16_t>("target");
    }

    for (const auto &gltfAccessor : gltfAccessors) {
        auto &accessor = gltf->accessors.emplace_back();
        accessor.bufferView = &gltf->bufferViews[gltfAccessor["bufferView"]];
        accessor.componentType = gltfAccessor.get<uint16_t>("componentType");
        auto &type = gltfAccessor["type"];
        if (type.value == "SCALAR") {
            accessor.type = glhf::Gltf::Accessor::SCALAR;
        } else if (type.value == "VEC2") {
            accessor.type = glhf::Gltf::Accessor::VEC2;
        } else if (type.value == "VEC3") {
            accessor.type = glhf::Gltf::Accessor::VEC3;
        } else if (type.value == "VEC4") {
            accessor.type = glhf::Gltf::Accessor::VEC4;
        } else if (type.value == "MAT4") {
            accessor.type = glhf::Gltf::Accessor::MAT4;
        } else {
            assert(false); // not found
        }
        accessor.count = gltfAccessor.get<uint32_t>("count");
    }

    for (const auto &gltfMesh : gltfMeshes) {
        auto &mesh = gltf->meshes.emplace_back();
        mesh.name.assign(gltfMesh["name"].value);
        mesh.primitives.reserve(gltfMesh["primitives"].size());
        for (const auto &gltfPrimitive : gltfMesh["primitives"]) {
            auto &primitive = mesh.primitives.emplace_back();
            for (const auto &gltfAttribute : gltfPrimitive["attributes"]) {
                if (gltfAttribute.key == "POSITION") {
                    primitive.attributes[glhf::Gltf::Primitive::POSITION] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "NORMAL") {
                    primitive.attributes[glhf::Gltf::Primitive::NORMAL] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "TEXCOORD_0") {
                    primitive.attributes[glhf::Gltf::Primitive::TEXCOORD_0] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "COLOR_0") {
                    primitive.attributes[glhf::Gltf::Primitive::COLOR_0] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "COLOR_1") {
                    primitive.attributes[glhf::Gltf::Primitive::COLOR_1] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "JOINTS_0") {
                    primitive.attributes[glhf::Gltf::Primitive::JOINTS_0] =
                        &gltf->accessors[(int)gltfAttribute];
                } else if (gltfAttribute.key == "WEIGHTS_0") {
                    primitive.attributes[glhf::Gltf::Primitive::WEIGHTS_0] =
                        &gltf->accessors[(int)gltfAttribute];
                } else {
                    assert(false); // not found
                }
            }
            primitive._indices = &gltf->accessors[gltfPrimitive["indices"]];
            primitive.material =
                gltfPrimitive.get<glhf::Gltf::Index_t>("material", glhf::Gltf::nullindex);
        }
    }

    for (const auto &gltfSkin : gltfSkins) {
        auto &skin = gltf->skins.emplace_back();
        skin.name = gltfSkin["name"];
        skin.inverseBindMatrices = &gltf->accessors.at(gltfSkin["inverseBindMatrices"]);
        for (glhf::Gltf::Index_t index : gltfSkin["joints"]) {
            skin.joints.emplace_back(index);
        }
    }

    for (const auto &gltfAnimation : gltfAnimations) {
        auto &animation = gltf->animations.emplace_back();
        animation.name = gltfAnimation["name"];
        const auto &gltfAnimSamplers = gltfAnimation["samplers"];
        const auto &gltfAnimChannels = gltfAnimation["channels"];
        animation.samplers.reserve(gltfAnimSamplers.size());
        for (const auto &gltfAnimSampler : gltfAnimSamplers) {
            auto &sampler = animation.samplers.emplace_back();
            sampler.input = &gltf->accessors.at(gltfAnimSampler["input"]);
            sampler.output = &gltf->accessors.at(gltfAnimSampler["output"]);
            const auto &gltfInterp = gltfAnimSampler["interpolation"];
            if (gltfInterp.value == "STEP") {
                sampler.interpolation = glhf::Gltf::Animation::Sampler::STEP;
            } else if (gltfInterp.value == "LINEAR") {
                sampler.interpolation = glhf::Gltf::Animation::Sampler::LINEAR;
            } else {
                assert(false); // not supported
            }
        }
        animation.channels.reserve(gltfAnimChannels.size());
        for (const auto &gltfAnimChannel : gltfAnimChannels) {
            auto &channel = animation.channels.emplace_back();
            channel.sampler = &animation.samplers.at(gltfAnimChannel["sampler"]);
            channel.targetNode = gltfAnimChannel["target"]["node"];
            const auto &targetPath = gltfAnimChannel["target"]["path"];
            if (targetPath.value == "translation") {
                channel.targetPath = glhf::Gltf::Animation::Channel::TRANSLATION;
            } else if (targetPath.value == "rotation") {
                channel.targetPath = glhf::Gltf::Animation::Channel::ROTATION;
            } else if (targetPath.value == "scale") {
                channel.targetPath = glhf::Gltf::Animation::Channel::SCALE;
            } else {
                assert(false); // not supported
            }
        }
    }

    for (const auto &gltfNode : gltfNodes) {
        auto &node = gltf->nodes.emplace_back();
        node.name.assign(gltfNode["name"].value);
        node.mesh = gltfNode.get<glhf::Gltf::Index_t>("mesh", glhf::Gltf::nullindex);
        node.skin = gltfNode.get<glhf::Gltf::Index_t>("skin", glhf::Gltf::nullindex);
        if (const auto &t = gltfNode["translation"]) {
            node.translation = {static_cast<float>(t[0]), static_cast<float>(t[1]),
                                static_cast<float>(t[2])};
        } else {
            node.translation = {0.0f, 0.0f, 0.0f};
        }
        if (const auto &r = gltfNode["rotation"]) {
            node.rotation = glm::quat{static_cast<float>(r[3]), static_cast<float>(r[0]),
                                      static_cast<float>(r[1]), static_cast<float>(r[2])};
        } else {
            node.rotation = {1.0f, 0.0f, 0.0f, 0.0f};
        }
        if (const auto &s = gltfNode["scale"]) {
            node.scale = {static_cast<float>(s[0]), static_cast<float>(s[1]),
                          static_cast<float>(s[2])};
        } else {
            node.scale = {1.0f, 1.0f, 1.0f};
        }
        if (const auto &children = gltfNode["children"]) {
            for (uint16_t child : children) {
                node.children.push_back(child);
            }
        }
    }

    for (int index : gltfScene["nodes"]) {
        gltf->scene.nodes.push_back(index);
    }
}
} // namespace

glhf::Gltf::Gltf(const char *glb) { loadGLB(glb); }

void glhf::Gltf::loadGLB(const char *glb) {
    glb_header *header = (glb_header *)glb;
    assert(header->magic == GLB_MAGIC);
    glb_chunk *chunk = (glb_chunk *)(header + 1);
    size_t i{header->length - sizeof(glb_header)};
    Buffer *buf;
    size_t buf_n = 0;
    while (i > 0) {
        switch (chunk->type) {
        case GLB_CHUNK_TYPE_JSON: {
            _parseJson(this, (char *)(chunk + 1), chunk->length);
            break;
        }
        case GLB_CHUNK_TYPE_BIN:
            // Assumption: JSON chunk comes first.
            buf = &buffers[buf_n++];
            buf->data = (unsigned char *)(chunk + 1);
            assert(buf->length == chunk->length);
            break;
        default:
            break;
        }
        i -= chunk->length + sizeof(glb_chunk);
        chunk = (glb_chunk *)((unsigned char *)chunk + chunk->length + sizeof(glb_chunk));
    }
}
