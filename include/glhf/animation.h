#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <span>
#include <string_view>
#include <unordered_map>

namespace glhf {
struct Output {
    std::span<glm::vec3> translations;
    std::span<glm::quat> rotations;
    std::span<glm::vec3> scales;
};

struct Sampler {
    // Interpolation
    size_t index;
    size_t endIndex;
    std::span<float> input;
    std::unordered_map<struct Node *, Output> outputs;
};

struct Animation {
    std::string_view name;
    std::vector<Sampler> samplers;
    float time;
    void update(float dt);
};
} // namespace glhf
