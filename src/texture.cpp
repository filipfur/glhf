#include "glhf/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

#include "glhf/opengl.h"
#include <cstdio>

glhf::Texture::Texture(const uint8_t *data, uint32_t width, uint32_t height, Channels channels,
                       uint32_t type) {
    create();
    load(data, width, height, channels, type);
}

glhf::Texture::~Texture() { dispose(); }

glhf::Texture::Texture(Texture &&other) : id{other.id} { other.id = 0; }
glhf::Texture &glhf::Texture::operator=(Texture &&other) {
    if (this != &other) {
        dispose();
        id = other.id;
        other.id = 0;
    }
    return *this;
}

void glhf::Texture::dispose() {
    if (id) {
        glDeleteTextures(1, &id);
    }
    id = 0;
}

void glhf::Texture::bind() { glBindTexture(GL_TEXTURE_2D, id); }

void glhf::Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

glhf::Texture::Texture(const char *filepath, bool flip, uint32_t type) {
    int iw, ih, ic;
    stbi_set_flip_vertically_on_load(flip);
    const uint8_t *idata = stbi_load(filepath, &iw, &ih, &ic, 0);
    create();
    load(idata, iw, ih, static_cast<Channels>(ic), type);
    stbi_image_free((void *)idata);
}

glhf::Texture::Texture(const uint8_t *addr, uint32_t len, bool flip, uint32_t type) {
    int iw, ih, ic;
    stbi_set_flip_vertically_on_load(flip);
    const uint8_t *idata = stbi_load_from_memory((uint8_t *)addr, len, &iw, &ih, &ic, 0);
    create();
    load(idata, iw, ih, static_cast<Channels>(ic), type);
    stbi_image_free((void *)idata);
}

glhf::Texture::Texture(const bdf::Font &font, uint32_t type) {
    int iw = font.pw * 16;
    int ih = font.ph * 8;
    int ic = 1;
    uint8_t _buf[128 * 128];
    uint8_t c{0};
    for (uint8_t y{0}; y < 8; ++y) {
        for (uint8_t x{0}; x < 16; ++x) {
            for (int16_t i{0}; i < font.ph; ++i) {
                for (int16_t j{0}; j < font.pw; ++j) {
                    /*_buf[i + (int)(c / 16) * font.ph][j + (c % 16) * font.pw] =
                    (font.bitmap[c][i] & (0b10000000 >> j)) ? 0xFF : 0x00;*/
                    _buf[(ih - (y * font.ph + i)) * iw + x * font.pw + j] =
                        (font.bitmap[c][i] & (0b10000000 >> j)) ? 0xFF : 0x00;
                }
            }
            ++c;
        }
    }
    create();
    load((uint8_t *)_buf, iw, ih, static_cast<Channels>(ic), type);
}

void glhf::Texture::create() {
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unbind();
}

void glhf::Texture::load(const uint8_t *data, uint32_t width, uint32_t height, Channels channels,
                         uint32_t type) {
    bind();
    GLenum format;
    GLenum internalFormat;
    switch (channels) {
    case R:
        format = internalFormat = GL_R8;
        format = GL_RED;
        break;
    case RGB:
        format = internalFormat = GL_RGB;
        break;
    case RGBA:
        format = internalFormat = GL_RGBA;
        break;
    case DS:
        internalFormat = GL_DEPTH24_STENCIL8;
        format = GL_DEPTH_STENCIL;
        break;
    default:
        printf("ERROR: Unknown texture channel format\n");
        format = internalFormat = GL_RED;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    unbind();
}