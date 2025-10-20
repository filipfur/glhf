#pragma once

#include "glhf/texture.h"
#include <memory>
#include <unordered_map>

namespace glhf {
struct Framebuffer {
    uint32_t id;
    std::unordered_map<uint32_t, std::shared_ptr<Texture>> textures;
    uint32_t rbo;

    Framebuffer();
    ~Framebuffer();

    Framebuffer(const Framebuffer &other) = delete;
    Framebuffer &operator=(const Framebuffer &other) = delete;

    Framebuffer(Framebuffer &&other) = delete;
    Framebuffer &operator=(Framebuffer &&other) = delete;

    void bind();
    void attach(uint32_t attachment, std::shared_ptr<Texture> texture);
    void createTexture(uint32_t attachment, uint32_t width, uint32_t height,
                       Texture::Channels channels, uint32_t type);
    void createDepthStencil(uint32_t width, uint32_t height);
    void createRenderBufferDS(uint32_t width, uint32_t height);
    void checkStatus(const char *label);
    void unbind();
};
} // namespace glhf
