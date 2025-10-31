#pragma once

#include "glhf/animation.h"
#include "glhf/gltf.h"
#include "glhf/node.h"
#include "glhf/predicate.h"
#include "glhf/skin.h"
#include <algorithm>

namespace glhf {

struct Scene {
    std::string_view name;
    std::vector<Node *> nodes;
    template <class UnaryPred> glhf::Node *findNode(UnaryPred p) {
        auto it = std::find_if(nodes.begin(), nodes.end(), p);
        return it == nodes.end() ? nullptr : *it;
    }
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

    template <class UnaryPred> glhf::Node *findNode(UnaryPred p) {
        auto it = std::find_if(nodes.begin(), nodes.end(), p);
        return it == nodes.end() ? nullptr : &(*it);
    }

    template <class UnaryPred> glhf::Mesh *findMesh(UnaryPred p) {
        auto it = std::find_if(meshes.begin(), meshes.end(), p);
        return it == meshes.end() ? nullptr : &(*it);
    }

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