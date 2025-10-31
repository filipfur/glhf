#include <gtest/gtest.h>

#include "glhf/gltf.h"
#include "glhf/json.h"

extern const unsigned char _glhf_objects_cube_glb[];

TEST(GltfTest, TestCube) {
    glhf::Json json;

    glhf::Gltf gltf;
    gltf.loadGLB((const char *)_glhf_objects_cube_glb);

    const auto &node = gltf.nodes.at(gltf.scene.nodes.at(0));
    const auto &mesh = gltf.meshes.at(node.mesh);
    ASSERT_STREQ(mesh.name.c_str(), "Cube");

    const auto &prim = mesh.primitives.front();
    auto positions = prim.positions();
    auto normals = prim.normals();
    auto uvs = prim.texCoords();
    ASSERT_EQ(positions.size(), 24u);
    ASSERT_EQ(normals.size(), 24u);
    ASSERT_EQ(uvs.size(), 24u);
    glm::vec3 total{0};
    for (const glm::vec3 &p : positions) {
        ASSERT_GE(glm::dot(p, p), 1.0f);
        total += p;
    }
    ASSERT_NEAR(glm::dot(total, total), 0.0f, 1e-6f);

    auto indices = prim.indices16();
    ASSERT_EQ(indices.size(), 36);
    for (auto i : indices) {
        ASSERT_LT(i, 25u);
    }
}