#pragma once

#include "uniform.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace glhf {

void createSharedSource(std::string_view key, std::string_view src);

struct Shader {
    uint32_t id;
    Shader(uint32_t type, const char *src, const std::unordered_set<std::string> &defines = {});
    ~Shader();

    Shader(const Shader &other) = delete;
    Shader &operator=(const Shader &other) = delete;

    Shader(Shader &&other) = delete;
    Shader &operator=(Shader &&other) = delete;
    bool compile(const char *src);
};

struct ShaderProgram {
    uint32_t id;
    std::unordered_map<std::string_view, Uniform> uniforms;

    ShaderProgram(std::unordered_map<std::string_view, Uniform> &&uniforms);
    ShaderProgram(std::unordered_map<std::string_view, Uniform> &&uniforms,
                  const std::unordered_map<std::string_view, uint32_t> &uniformBuffers,
                  const Shader &vertex, const Shader &fragment);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram &other) = delete;
    ShaderProgram &operator=(const ShaderProgram &other) = delete;
    ShaderProgram(ShaderProgram &&other) = delete;
    ShaderProgram &operator=(ShaderProgram &&other) = delete;

    Uniform *uniform(const char *key);
    void use();
    void link(const Shader &vertex, const Shader &fragment);
};

} // namespace glhf