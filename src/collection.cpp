#include "glhf/collection.h"
#include "glhf/log.h"
#include "glhf/primitive_descriptor.h"

#include <iostream>

void glhf::Collection::fromGLB(const char *glb, Options options) {
    gltf.loadGLB(glb);
    textures.reserve(gltf.textures.size());
    for (const auto &gltfTexture : gltf.textures) {
        textures.emplace_back((uint8_t *)gltfTexture.image->bufferView->data(),
                              gltfTexture.image->bufferView->length, false);
        // TODO:
        // texture.setFilter(gltfTexture.sampler->magFilter, gltfTexture.sampler->minFilter);
    }
    materials.reserve(gltf.materials.size());
    constexpr uint8_t pixel[] = {0xFF, 0xFF, 0xFF};
    static Texture defaultTexture{pixel, 1, 1, Texture::RGB, GL_UNSIGNED_BYTE};
    static Material defaultMaterial{.name = "Default",
                                    .color = rgb(255, 255, 255),
                                    .metallic = 0.0f,
                                    .roughness = 0.5f,
                                    .textures = {{GL_TEXTURE0, &defaultTexture}}};
    for (const auto &gltfMaterial : gltf.materials) {
        auto &material = materials.emplace_back();
        material.name = gltfMaterial.name;
        material.color = gltfMaterial.baseColor;
        material.metallic = gltfMaterial.metallic;
        material.roughness = gltfMaterial.roughness;
        if (gltfMaterial.textures.empty()) {
            material.textures.emplace(GL_TEXTURE0, &defaultTexture);
        } else {
            for (auto texIndex : gltfMaterial.textures) {
                material.textures.emplace(GL_TEXTURE0 + material.textures.size(),
                                          &textures.at(texIndex));
            }
        }
    }
    meshes.reserve(gltf.meshes.size());
    for (const auto &gltfMesh : gltf.meshes) {
        auto &mesh = meshes.emplace_back();
        mesh.name = gltfMesh.name;
        for (const auto &gltfPrimitive : gltfMesh.primitives) {
            std::vector<glm::vec3> tangents;
            if (options & TANGENTS) {
                auto positions = gltfPrimitive.positions();
                auto uvs = gltfPrimitive.positions();
                auto indices = gltfPrimitive.indices16();
                tangents.resize(positions.size());
                for (size_t i{0}; i < indices.size(); i += 3) {
                    auto i0 = indices[i];
                    auto i1 = indices[i + 1];
                    auto i2 = indices[i + 2];
                    const glm::vec3 &p0 = positions[i0];
                    const glm::vec3 &p1 = positions[i1];
                    const glm::vec3 &p2 = positions[i2];
                    const glm::vec2 &u0 = uvs[i0];
                    const glm::vec2 &u1 = uvs[i1];
                    const glm::vec2 &u2 = uvs[i2];
                    glm::vec3 dp0 = p1 - p0;
                    glm::vec3 dp1 = p2 - p0;
                    glm::vec2 du0 = u1 - u0;
                    glm::vec2 du1 = u2 - u0;

                    float f = 1.0f / (du0.x * du1.y - du1.x * du0.y);
                    glm::vec3 t = {f * (du1.y * dp0.x - du0.y * dp1.x),
                                   f * (du1.y * dp0.y - du0.y * dp1.y),
                                   f * (du1.y * dp0.z - du0.y * dp1.z)};
                    tangents[i0] = t;
                    tangents[i1] = t;
                    tangents[i2] = t;
                }
            }

            if (options & SKINNING && gltfPrimitive.attributes[glhf::Gltf::Primitive::JOINTS_0]) {
                if (options & TANGENTS) {
                    mesh.primitives.push_back(
                        glhf::PositionNormalUVTangentJointWeight.createPrimitive(
                            {
                                gltfPrimitive.attributes[glhf::Gltf::Primitive::POSITION]->bytes(),
                                gltfPrimitive.attributes[glhf::Gltf::Primitive::NORMAL]->bytes(),
                                gltfPrimitive.attributes[glhf::Gltf::Primitive::TEXCOORD_0]
                                    ->bytes(),
                                std::span<uint8_t>((uint8_t *)tangents.data(),
                                                   tangents.size() * sizeof(glm::vec3)),
                                gltfPrimitive.attributes[glhf::Gltf::Primitive::JOINTS_0]->bytes(),
                                gltfPrimitive.attributes[glhf::Gltf::Primitive::WEIGHTS_0]->bytes(),
                            },
                            gltfPrimitive.indices16()));
                } else {
                    mesh.primitives.push_back(glhf::PositionNormalUVJointWeight.createPrimitive(
                        {
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::POSITION]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::NORMAL]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::TEXCOORD_0]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::JOINTS_0]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::WEIGHTS_0]->bytes(),
                        },
                        gltfPrimitive.indices16()));
                }
            } else {
                if (options & TANGENTS) {
                    mesh.primitives.push_back(glhf::PositionNormalUVTangent.createPrimitive(
                        {
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::POSITION]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::NORMAL]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::TEXCOORD_0]->bytes(),
                            std::span<uint8_t>((uint8_t *)tangents.data(),
                                               tangents.size() * sizeof(glm::vec3)),
                        },
                        gltfPrimitive.indices16()));

                } else {
                    mesh.primitives.push_back(glhf::PositionNormalUV.createPrimitive(
                        {
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::POSITION]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::NORMAL]->bytes(),
                            gltfPrimitive.attributes[glhf::Gltf::Primitive::TEXCOORD_0]->bytes(),
                        },
                        gltfPrimitive.indices16()));
                }
            }
            mesh.primitives.back().material = (gltfPrimitive.material == glhf::Gltf::nullindex)
                                                  ? &defaultMaterial
                                                  : &materials.at(gltfPrimitive.material);
        }
    }
    nodes.resize(gltf.nodes.size());
    skins.reserve(gltf.skins.size());
    for (const auto &gltfSkin : gltf.skins) {
        auto &skin = skins.emplace_back();
        skin.name = gltfSkin.name;
        for (glhf::Gltf::Index_t j : gltfSkin.joints) {
            skin.joints.push_back(&nodes.at(j));
        }
        assert(skin.joints.size() <= glhf::Skin::MAX_BONES);
        skin.inverseBindMatrices = gltfSkin.inverseBindMatrices->span<glm::mat4>();
    }
    animations.reserve(gltf.animations.size());
    for (const auto &gltfAnimation : gltf.animations) {
        auto &animation = animations.emplace_back();
        animation.name = gltfAnimation.name;
        animation.time = 0.0f;
        std::unordered_map<glhf::Gltf::Accessor *, glhf::Gltf::Index_t> smap;
        for (const auto &gltfChannel : gltfAnimation.channels) {
            const auto &gltfSampler = *gltfChannel.sampler;
            auto it = smap.find(gltfSampler.input);
            if (it == smap.end()) {
                smap[gltfSampler.input] = animation.samplers.size();
                animation.samplers.emplace_back();
            }
            auto &sampler = animation.samplers.at(smap[gltfSampler.input]);
            sampler.input = gltfSampler.input->span<float>();
            sampler.index = 0;
            sampler.endIndex = gltfSampler.input->count;
            Node *node = &nodes.at(gltfChannel.targetNode);
            auto outIt = sampler.outputs.find(node);
            if (outIt == sampler.outputs.end()) {
                sampler.outputs[node] = {};
                outIt = sampler.outputs.find(node);
            }
            switch (gltfChannel.targetPath) {
            case glhf::Gltf::Animation::Channel::TRANSLATION:
                outIt->second.translations = gltfSampler.output->span<glm::vec3>();
                break;
            case glhf::Gltf::Animation::Channel::ROTATION:
                outIt->second.rotations = gltfSampler.output->span<glm::quat>();
                break;
            case glhf::Gltf::Animation::Channel::SCALE:
                outIt->second.scales = gltfSampler.output->span<glm::vec3>();
                break;
            }
        }
    }
    auto nodeIt = nodes.begin();
    for (const auto &gltfNode : gltf.nodes) {
        auto &node = *nodeIt;
        node.name = gltfNode.name;
        node.mesh = gltfNode.mesh == glhf::Gltf::nullindex ? nullptr : &meshes.at(gltfNode.mesh);
        node.skin = gltfNode.skin == glhf::Gltf::nullindex ? nullptr : &skins.at(gltfNode.skin);
        node.translation = gltfNode.translation;
        node.rotation = gltfNode.rotation;
        node.scale = gltfNode.scale;
        for (glhf::Gltf::Index_t child : gltfNode.children) {
            node.children.push_back(&nodes.at(child));
            nodes.at(child).setParent(&node);
        }
        ++nodeIt;
    }
    scene.name = gltf.scene.name;
    for (glhf::Gltf::Index_t node : gltf.scene.nodes) {
        scene.nodes.push_back(&nodes[node]);
        scene.nodes.back()->recursive(
            []([[maybe_unused]] glhf::Node *node, [[maybe_unused]] int depth) {
                LOG_TRACE("%*s%.*s%s%s", depth * 2, "", (int)node->name.size(), node->name.data(),
                          node->mesh ? " (mesh)" : "", node->skin ? " (skin)" : "");
            });
    }
}