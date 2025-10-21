#include "glhf/window.h"

#include "glhf/camera.h"
#include "glhf/collection.h"
#include "glhf/gltf.h"
#include "glhf/instance_factory.h"
#include "glhf/log.h"
#include "glhf/primitive_descriptor.h"
#include "glhf/shader.h"
#include "glhf/time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include <memory>

extern const uint8_t _embed_inst_vert[];
extern const uint8_t _embed_anim_tbn_vert[];
extern const uint8_t _embed_object_frag[];
extern const uint8_t _embed_object_normal_frag[];

extern const uint8_t _embed_cube_glb[];
extern const uint8_t _embed_joyful_vampire_glb[];

static constexpr unsigned int WINDOW_WIDTH = 720u;
static constexpr unsigned int WINDOW_HEIGHT = 480u;
static constexpr glm::vec2 WINDOW_SIZE = {WINDOW_WIDTH, WINDOW_HEIGHT};

struct Application : public glhf::IApplication {
    void init(int width, int height) override {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glhf::Camera::createUBO();
        glhf::Skin::createUBO();

        glhf::Collection::Options opts = static_cast<glhf::Collection::Options>(
            glhf::Collection::SKINNING | glhf::Collection::TANGENTS);
        _collections.emplace_back((const char *)_embed_joyful_vampire_glb, opts);
        _collections.emplace_back((const char *)_embed_cube_glb, glhf::Collection::NO_OPTIONS);
        setActiveCollection(_collections.begin());

        glm::mat4 instances[101 * 101];
        size_t i{0};
        for (int y{-50}; y <= 50; ++y) {
            for (int x{-50}; x <= 50; ++x) {
                float dy = glm::sqrt(x * x + y * y) < 4 ? 0 : (rand() % 100) * 1e-2f;
                instances[i++] = glm::translate(glm::scale(glm::mat4{1.0f}, glm::vec3(0.1f)),
                                                glm::vec3(x * 2.0f, -1.0f + dy, y * 2.0f));
            }
        }
        assert(i == 101 * 101);

        glhf::Node *cubeNode = _collections.back().scene.nodes.front();
        cubeNode->mesh->primitives.front().material->color = rgb(212, 255, 119);
        assert(cubeNode->mesh->name == "Cube");
        _instanceFactory.reset(new glhf::InstanceFactory<glm::mat4>(
            &cubeNode->mesh->primitives.front(), glhf::PositionNormalUV.attributes.size(),
            std::span<glm::mat4>(instances)));

        _instShader.reset(new glhf::ShaderProgram(
            {{"u_model", glm::mat4(1.0f)}, {"u_texture", 0}, {"u_color", glm::vec4(1.0f)}},
            {{"CameraBlock", *glhf::Camera::UBO}},
            {GL_VERTEX_SHADER, (const char *)_embed_inst_vert},
            {GL_FRAGMENT_SHADER, (const char *)_embed_object_frag}));
        _animShader.reset(new glhf::ShaderProgram(
            {{"u_model", glm::mat4(1.0f)},
             {"u_texture", 0},
             {"u_normal", 1},
             {"u_color", glm::vec4(1.0f)}},
            {{"CameraBlock", *glhf::Camera::UBO}, {"SkinBlock", *glhf::Skin::UBO}},
            {GL_VERTEX_SHADER, (const char *)_embed_anim_tbn_vert},
            {GL_FRAGMENT_SHADER, (const char *)_embed_object_normal_frag}));

        glhf::Camera::UBO->bindBufferBase();
        glhf::Skin::UBO->bindBufferBase();
    }

    void fps(float frames) override {}

    bool update(float dt) override {

        _camera.pitch = glm::pi<float>() * 0.125f;
        _camera.yaw += dt * glm::pi<float>() * 0.125f;
        _camera.update(dt);

        if (!_activeCollection->animations.empty()) {
            _activeCollection->animations.front().update(dt);
        }

        return true;
    }

    void draw(int width, int height) override {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _instShader->use();
        _instanceFactory->render(_instShader.get());
        _animShader->use();
        for (auto *node : _activeCollection->scene.nodes) {
            if (node->find([](const glhf::Node &node) { return node.skin; })) {
                node->render(_animShader.get());
            }
        }
    }

    bool event(const SDL_Event &ev) override {
        switch (ev.type) {
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            default:
                break;
            }
        default:
            break;
        }
        return IApplication::event(ev);
    }

    void setActiveCollection(std::list<glhf::Collection>::iterator collection) {
        _activeCollection = collection;

        // measure scene
        glm::vec3 min{FLT_MAX};
        glm::vec3 max{-FLT_MAX};
        for (const auto &mesh : collection->gltf.meshes) {
            for (const auto &primitive : mesh.primitives) {
                for (const glm::vec3 &p : primitive.positions()) {
                    for (size_t j{0}; j < 3; ++j) {
                        min[j] = std::min(min[j], p[j]);
                        max[j] = std::max(max[j], p[j]);
                    }
                }
            }
        }
        glm::vec3 extents = (max - min);
        LOG_INFO("Collection '%s' size = %dx%dx%d mm", collection->gltf.scene.name.data(),
                 (int)(extents.x * 1000), (int)(extents.y * 1000), (int)(extents.z * 1000));
        _camera.center = (min + max) * 0.5f;
        _camera.distance = glm::max(glm::max(extents.x, extents.y), extents.z) * 1.5f;
    }

    glhf::Camera _camera{
        glm::perspective(glm::radians(45.0f), WINDOW_SIZE.x / WINDOW_SIZE.y, 0.01f, 100.0f)};
    std::shared_ptr<glhf::ShaderProgram> _instShader;
    std::shared_ptr<glhf::ShaderProgram> _animShader;
    std::list<glhf::Collection> _collections;
    std::list<glhf::Collection>::iterator _activeCollection;
    std::shared_ptr<glhf::InstanceFactory<glm::mat4>> _instanceFactory;
};

int main() {
    Application app;
    glhf::Window window{app};
    window.load("hello_dancer", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    window.run();
    return 0;
}