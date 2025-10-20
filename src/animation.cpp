#include "glhf/animation.h"
#include "glhf/log.h"
#include "glhf/node.h"
#include <algorithm>

void glhf::Animation::update(float dt) {
    bool done = true;
    for (auto &sampler : samplers) {
        if (sampler.input[sampler.index] < time) {
            ++sampler.index;
        }
        if (sampler.index < (sampler.endIndex - 1)) {
            done = false;
        } else {
            assert(sampler.index == (sampler.endIndex - 1));
        }
        for (auto [node, channel] : sampler.outputs) {
            if (sampler.index < channel.translations.size()) {
                node->translation = glm::mix(node->t(), channel.translations[sampler.index], 0.5f);
                // node->translation = channel.translations[sampler.index];
            }
            if (sampler.index < channel.rotations.size()) {
                node->rotation = glm::slerp(node->r(), channel.rotations[sampler.index], 0.5f);
                // node->rotation = channel.rotations[sampler.index];
            }
            if (sampler.index < channel.scales.size()) {
                node->scale = glm::mix(node->s(), channel.scales[sampler.index], 0.5f);
                // node->scale = channel.scales[sampler.index];
            }
        }
    }
    if (done) {
        std::for_each(samplers.begin(), samplers.end(), [](auto &sampler) { sampler.index = 0; });
        time = 0.0f;
    } else {
        time += dt;
    }
}