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

extern const uint8_t _embed_object_vert[];
extern const uint8_t _embed_object_frag[];

extern const uint8_t _embed_cube_glb[];

static constexpr unsigned int WINDOW_WIDTH = 720u;
static constexpr unsigned int WINDOW_HEIGHT = 480u;
static constexpr glm::vec2 WINDOW_SIZE = {WINDOW_WIDTH, WINDOW_HEIGHT};

struct Application : public glhf::IApplication {
    void init(int width, int height) override {
        (void)width;
        (void)height;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glhf::Camera::createUBO();

        _collections.emplace_back((const char *)_embed_cube_glb, glhf::Collection::NO_OPTIONS);

        _cubeNode = _collections.back().scene.nodes.front();
        _cubeNode->mesh->primitives.front().material->color = rgb(112, 54, 144);

        _objectShader.reset(new glhf::ShaderProgram(
            {{"u_model", glm::mat4(1.0f)}, {"u_texture", 0}, {"u_color", glm::vec4(1.0f)}},
            {{"CameraBlock", *glhf::Camera::UBO}},
            {GL_VERTEX_SHADER, (const char *)_embed_object_vert},
            {GL_FRAGMENT_SHADER, (const char *)_embed_object_frag}));

        glhf::Camera::UBO->bindBufferBase();

        _camera.center = {0.0f, 0.0f, 0.0f};
        _camera.distance = 10.0f;
    }

    void fps(float frames) override { (void)frames; }

    bool update(float dt) override {
        _camera.pitch = glm::pi<float>() * 0.125f;
        _camera.yaw += dt * glm::pi<float>() * 0.125f;
        _camera.update(dt);

        float y1 = _cubeNode->t().y + _cubeVerticalSpeed * dt;
        if (y1 < 0) {
            _cubeVerticalSpeed = 0.0f;
            _cubeNode->translation = {0.0f, 0.0f, 0.0f};
        } else {
            _cubeNode->translation = {0.0f, y1, 0.0f};
            if (_cubeNode->t().y > 0.01f) {
                _cubeVerticalSpeed -= 9.82f * dt;
            }
        }
        return true;
    }

    void draw(int width, int height) override {
        (void)width;
        (void)height;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _objectShader->use();
        _cubeNode->render(_objectShader.get());
    }

    bool event(const SDL_Event &ev) override {
        switch (ev.type) {
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_SPACE:
                if (_cubeNode->t().y < 0.01f) {
                    _cubeVerticalSpeed += 4.0f + (rand() % 100) * 0.02;
                }
                break;
            default:
                break;
            }
        default:
            break;
        }
        return IApplication::event(ev);
    }

    glhf::Camera _camera{
        glm::perspective(glm::radians(45.0f), WINDOW_SIZE.x / WINDOW_SIZE.y, 0.01f, 100.0f)};
    std::shared_ptr<glhf::ShaderProgram> _objectShader;
    std::list<glhf::Collection> _collections;
    glhf::Node *_cubeNode;
    float _cubeVerticalSpeed{0.0f};
};

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    Application app;
    glhf::Window window{app};
    window.load("hello_cube", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    window.run();
    return 0;
}