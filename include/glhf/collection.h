#pragma once

#include "glhf/animation.h"
#include "glhf/gltf.h"
#include "glhf/node.h"
#include "glhf/skin.h"

namespace glhf {

struct Scene {
    std::string_view name;
    std::vector<Node *> nodes;
};

struct Collection {
    enum Options {
        NO_OPTIONS = 0,
        TANGENTS = 0b00000001,
        SKINNING = 0b00000010,
        EVERYTHING = 0b11111111,
    };

    Collection(const char *glb, Options options = NO_OPTIONS) { fromGLB(glb, options); }
    ~Collection() = default;

    Collection(const Collection &other) = delete;
    Collection &operator=(const Collection &other) = delete;

    Collection(Collection &&other) = delete;
    Collection &operator=(Collection &&other) = delete;

    void fromGLB(const char *glb, Options options = NO_OPTIONS);

    Gltf gltf;
    std::vector<Texture> textures;
    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    std::vector<Node> nodes;
    std::vector<Skin> skins;
    std::vector<Animation> animations;
    Scene scene;
};

} // namespace glhf