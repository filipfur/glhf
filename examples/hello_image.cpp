#include "glhf/window.h"

#include "glhf/camera.h"
#include "glhf/collection.h"
#include "glhf/gltf.h"
#include "glhf/log.h"
#include "glhf/primitive_descriptor.h"
#include "glhf/shader.h"
#include "glhf/time.h"
#include <glm/glm.hpp>
#include <list>
#include <memory>

extern const uint8_t _glhf_shaders_screen_vert[];
extern const uint8_t _glhf_shaders_texture_frag[];

extern const uint8_t _glhf_images_sheep_png[];
extern const size_t _glhf_images_sheep_png_length;

static constexpr unsigned int WINDOW_WIDTH = 480u;
static constexpr unsigned int WINDOW_HEIGHT = 480u;

struct Application : public glhf::IApplication {
    void init(int width, int height) override {
        (void)width;
        (void)height;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        _shaderProgram.reset(new glhf::ShaderProgram(
            {{"u_texture", 0}}, {}, {GL_VERTEX_SHADER, (const char *)_glhf_shaders_screen_vert},
            {GL_FRAGMENT_SHADER, (const char *)_glhf_shaders_texture_frag}));
    }

    void fps(float frames) override { (void)frames; }

    bool update(float dt) override {
        (void)dt;
        return true;
    }

    void draw(int width, int height) override {
        (void)width;
        (void)height;
        uint8_t r = rand() % 256;
        uint8_t g = rand() % 256;
        uint8_t b = rand() % 256;
        glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _shaderProgram->use();
        static glm::vec4 screen_vertices[] = {
            {-1.0f, -1.0f, 0.0f, 0.0f},
            {1.0f, -1.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
        };
        static uint16_t screen_indices[] = {0, 1, 2, 2, 1, 3};
        static glhf::Primitive screen_primitive = glhf::XYUV.createPrimitive(
            {std::span<uint8_t>((uint8_t *)screen_vertices, sizeof(screen_vertices))},
            std::span<uint16_t>(screen_indices));
        static glhf::Texture texture{_glhf_images_sheep_png,
                                     (uint32_t)_glhf_images_sheep_png_length, true};
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        screen_primitive.render();
    }

    std::shared_ptr<glhf::ShaderProgram> _shaderProgram;
};

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Application app;
    glhf::Window window{app};
    window.load("hello_image", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    window.run();
    return 0;
}