#include "glhf/window.h"

#include "glhf/framebuffer.h"
#include "glhf/log.h"
#include "glhf/primitive_descriptor.h"
#include "glhf/shader.h"
#include "glhf/texture.h"
#include "glhf/time.h"
#include "glhf/uniformbuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static constexpr unsigned int WINDOW_WIDTH = 720u;
static constexpr unsigned int WINDOW_HEIGHT = 480u;
static constexpr glm::vec2 WINDOW_SIZE = {WINDOW_WIDTH, WINDOW_HEIGHT};

const float t_h = glm::sin(glm::pi<float>() / 3);
const glm::vec3 triangle_positions[] = {
    {0.0f, 0.5f * t_h, 0.0f},
    {-0.5f, -0.5f * t_h, 0.0f},
    {0.5f, -0.5f * t_h, 0.0f},
};
const glm::vec3 triangle_normals[] = {
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
};
const glm::vec2 triangle_uvs[] = {
    {0.5f, 1.0f},
    {0.0f, 0.0f},
    {1.0f, 0.0f},
};

const glm::vec4 screen_vertices[] = {
    {-1.0f, -1.0f, 0.0f, 0.0f},
    {+1.0f, -1.0f, 1.0f, 0.0f},
    {-1.0f, +1.0f, 0.0f, 1.0f},
    {+1.0f, +1.0f, 1.0f, 1.0f},
};
const uint16_t screen_indices[] = {
    0, 1, 2, 1, 3, 2,
};

const char *triVertSrc = GLHF_GLSL_VERSION R"(
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNor;
layout(location=2) in vec2 aUV;
out vec2 UV;

layout (std140) uniform CameraBlock
{
    mat4 u_projection;
    mat4 u_view;
    vec3 u_eye;
};

uniform mat4 u_model;

void main() {
    UV = aUV;
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}
)";

const char *triFragSrc = GLHF_GLSL_VERSION R"(
precision mediump float;
in vec2 UV;

uniform sampler2D u_texture;

layout (std140) uniform MaterialBlock
{
    vec3 u_color;
};

layout(location=0) out vec4 FragColor;
void main() {
    vec4 diffuse = texture(u_texture, UV);
    FragColor = vec4(vec3(UV, 1.0) * u_color * diffuse.rgb, 1);
}
)";

const char *screenVertSrc = GLHF_GLSL_VERSION R"(
layout(location=0) in vec4 aXYUV;
out vec2 UV;

void main() {
    UV = aXYUV.zw;
    gl_Position = vec4(aXYUV.xy, 0.0, 1.0);
}
)";

const char *screenFragSrc = GLHF_GLSL_VERSION R"(
precision mediump float;
in vec2 UV;
uniform sampler2D u_texture;
layout(location=0) out vec4 FragColor;
void main() {
    vec4 diffuse = texture(u_texture, UV);
    FragColor = diffuse;
}
)";

struct CameraBlock {
    glm::mat4 u_projection;
    glm::mat4 u_view;
    glm::vec3 u_eye;
    float padding;
};

struct MaterialBlock {
    glm::vec3 u_color;
    float padding;
};

constexpr glhf::Pattern<8, 8, glhf::Texture::RGB> checkersPattern{[](uint32_t x, uint32_t y) {
    return glm::vec4(glm::vec3(0.75f + ((y % 2 + x) % 2) * 0.25f), 1.0f);
}};

struct Application : public glhf::IApplication {
    void init(int width, int height) override {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        primitives.push_back(glhf::PositionNormalUV.createPrimitive(
            {
                std::span<uint8_t>((uint8_t *)triangle_positions, sizeof(triangle_positions)),
                std::span<uint8_t>((uint8_t *)triangle_normals, sizeof(triangle_normals)),
                std::span<uint8_t>((uint8_t *)triangle_uvs, sizeof(triangle_uvs)),
            },
            sizeof(triangle_positions) / sizeof(glm::vec3)));

        primitives.push_back(glhf::XYUV.createPrimitive(
            {std::span<uint8_t>((uint8_t *)screen_vertices, sizeof(screen_vertices))},
            std::span<uint16_t>((uint16_t *)screen_indices,
                                sizeof(screen_indices) / sizeof(uint16_t))));

        auto *cameraUBO = glhf::UniformBuffer::create(&_cameraBlock, sizeof(CameraBlock));
        auto *materialUBO = glhf::UniformBuffer::create(&_materialBlock, sizeof(glm::vec4));
        _triShader.reset(new glhf::ShaderProgram(
            {{"u_model", glm::mat4(1.0f)}, {"u_texture", 0}},
            {{"CameraBlock", *cameraUBO}, {"MaterialBlock", *materialUBO}},
            {GL_VERTEX_SHADER, triVertSrc}, {GL_FRAGMENT_SHADER, triFragSrc}));
        _screenShader.reset(new glhf::ShaderProgram({{"u_texture", 0}}, {},
                                                    {GL_VERTEX_SHADER, screenVertSrc},
                                                    {GL_FRAGMENT_SHADER, screenFragSrc}));
        cameraUBO->bindBufferBase();
        materialUBO->bindBufferBase();
        _checkersTexture.reset(new glhf::Texture(
            checkersPattern.data, checkersPattern.width, checkersPattern.height,
            static_cast<glhf::Texture::Channels>(checkersPattern.channels), GL_UNSIGNED_BYTE));
        _fbo.reset(new glhf::Framebuffer());
        _fbo->bind();
        _fbo->createTexture(GL_COLOR_ATTACHMENT0, width * 2, height * 2, glhf::Texture::RGB,
                            GL_UNSIGNED_BYTE);
        _fbo->textures.at(GL_COLOR_ATTACHMENT0)->bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        _fbo->createRenderBufferDS(width * 2, height * 2);
        _fbo->unbind();
    }
    void fps(float frames) override { (void)frames; }
    bool update(float dt) override {
        (void)dt;
        return true;
    }
    void draw(int width, int height) override {
        const size_t PRIMITIVE_TRIANGLE = 0;
        const size_t PRIMITIVE_SCREEN = 1;

        if (_antialiasing) {
            glViewport(0, 0, width * 2, height * 2);
            _fbo->bind();
        }
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        _checkersTexture->bind();
        _triShader->use();
        _triShader->uniforms.at("u_model")
            << glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.33f)), glhf::Time::seconds(),
                           glm::vec3{0.0f, 0.0f, 1.0f});
        primitives.at(PRIMITIVE_TRIANGLE).render();
        if (_antialiasing) {
            _fbo->unbind();
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _fbo->textures.at(GL_COLOR_ATTACHMENT0)->bind();
            _screenShader->use();
            primitives.at(PRIMITIVE_SCREEN).render();
        }
    }
    bool event(const SDL_Event &ev) override {
        switch (ev.type) {
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_a:
                _antialiasing = !_antialiasing;
                LOG_INFO("toggle antialiasing: %d", _antialiasing);
                return true;
            }
            break;
        }
        return IApplication::event(ev);
    }

    std::vector<glhf::Primitive> primitives;
    CameraBlock _cameraBlock{
        .u_projection =
            glm::perspective(glm::radians(45.0f), WINDOW_SIZE.x / WINDOW_SIZE.y, 0.01f, 100.0f),
        .u_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f)),
        .u_eye = glm::vec3{0.0f, 0.0f, 1.0f},
        .padding = 0.0f,
    };
    MaterialBlock _materialBlock{
        .u_color = {1.0f, 0.0f, 1.0f},
        .padding = 0.0f,
    };
    std::shared_ptr<glhf::ShaderProgram> _triShader;
    std::shared_ptr<glhf::ShaderProgram> _screenShader;
    std::shared_ptr<glhf::Texture> _checkersTexture;
    std::shared_ptr<glhf::Framebuffer> _fbo;
    bool _antialiasing{true};
};

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Application app;
    glhf::Window window{app};
    window.load("hello_triangle", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    window.run();
    return 0;
}