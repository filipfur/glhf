#pragma once

#include "glhf/color.h"
#include "glhf/shader.h"
#include "glhf/texture.h"
#include <memory>
#include <string_view>
#include <unordered_map>

namespace glhf {
struct Material {
    std::string_view name;
    Color color;
    float metallic;
    float roughness;
    std::unordered_map<uint32_t, std::shared_ptr<Texture>> textures;

    void bind(glhf::ShaderProgram &shaderProgram) {
        if (auto u_color = shaderProgram.uniform("u_color")) {
            *u_color << color.vec4();
        }
        if (auto u_metallic = shaderProgram.uniform("u_metallic")) {
            *u_metallic << metallic;
        }
        if (auto u_roughness = shaderProgram.uniform("u_roughness")) {
            *u_roughness << roughness;
        }
        for (const auto &it : textures) {
            glActiveTexture(it.first);
            glBindTexture(GL_TEXTURE_2D, it.second->id);
        }
    }
};

inline std::shared_ptr<glhf::Material> createMaterial(std::string_view name, glhf::Color color) {
    return std::shared_ptr<glhf::Material>(new glhf::Material{
        .name = name,
        .color = color,
        .metallic = 0.0f,
        .roughness = 0.5f,
        .textures = {},
    });
}
} // namespace glhf
