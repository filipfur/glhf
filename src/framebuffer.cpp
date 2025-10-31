#include "glhf/framebuffer.h"

#include "glhf/log.h"
#include "glhf/opengl.h"
#include "glhf/texture.h"

glhf::Framebuffer::Framebuffer() { glGenFramebuffers(1, &id); }

glhf::Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &id); }

void glhf::Framebuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, id); }
void glhf::Framebuffer::attach(uint32_t attachment, std::shared_ptr<Texture> texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->id, 0);
    textures.emplace(attachment, texture);
}
void glhf::Framebuffer::createTexture(uint32_t attachment, uint32_t width, uint32_t height,
                                      Texture::Channels channels, uint32_t type) {
    attach(attachment, std::make_shared<glhf::Texture>(nullptr, width, height, channels, type));
}
void glhf::Framebuffer::createDepthStencil(uint32_t width, uint32_t height) {
    this->createTexture(GL_DEPTH_STENCIL_ATTACHMENT, width, height, Texture::DS,
                        GL_UNSIGNED_INT_24_8);
}
void glhf::Framebuffer::createRenderBufferDS(uint32_t width, uint32_t height) {
    // TODO: Dispose the render buffer at some point
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}
void glhf::Framebuffer::checkStatus([[maybe_unused]] const char *label) {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR("Framebuffer '%s' is not complete: %d", label, id);
    }
}
void glhf::Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
