#include <gtest/gtest.h>

#include <glm/gtc/quaternion.hpp>

#include "glhf/geometry/geometry.h"
#include "glhf/geometry/polyhedron.h"
#include "glhf/geometry/types.h"

using namespace glhf;

TEST(GeometryTest, TestGeometryArray) {
    std::array<Geometry, 2> arr = {
        Ray{.origin = {0.0f, 2.0f, 0.0f}, .direction = {0.0f, -1.0f, 0.0f}},
        Plane{.normal = {0.0f, 1.0f, 0.0f}, .d = 0.0f}};

    auto &ray = std::get<Ray>(arr.at(0));
    auto &plane = std::get<Plane>(arr.at(1));

    auto hit = intersect(arr.at(0), arr.at(1));
    ASSERT_TRUE(hit);
    ASSERT_NEAR(glm::dot(hit->origin, hit->origin), 0.0f, FLT_EPSILON);
    ASSERT_NEAR(glm::distance(hit->normal, plane.normal), 0.0f, FLT_EPSILON);

    ASSERT_FALSE(intersect(arr.at(1), arr.at(0))); // unsupported
}

TEST(GeometryTest, TestRayPlane) {
    Plane plane{.normal = {0.0f, 1.0f, 0.0f}, .d = 0.0f};
    Plane plane_2{.normal = {-1.0f, 0.0f, 0.0f}, .d = 1.0f};
    Ray ray{.origin = {0.0f, 0.1f, 0.0f}, .direction = {0.0f, -1.0f, 0.0f}};
    ASSERT_TRUE(ray.intersect(plane));
    ray.direction = {1.0f, 0.0f, 0.0f};
    ASSERT_FALSE(ray.intersect(plane));
    ASSERT_TRUE(ray.intersect(plane_2));
}

TEST(GeometryTest, TestRotateNormal) {
    glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3{1.0f, 0.0f, 0.0f});
    glm::vec3 N{0.0f, 1.0f, 0.0f};
    ASSERT_NEAR(glm::distance(glm::vec3{0.0f, 0.0f, 1.0f}, q * N), 0.0f, FLT_EPSILON);
}

static glm::vec3 *start_vertices = nullptr;
static glhf::Face *start_faces = nullptr;
glhf::Polyhedron createPolyhedron() {
    glhf::Polyhedron polyhedron{
        .vertices = {{0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 3.0f}},
        .faces = {{0, 1, 1}},
    };
    start_vertices = polyhedron.vertices.data();
    start_faces = polyhedron.faces.data();
    return polyhedron;
}

TEST(GeometryTest, PolyhedronCopyMove) {
    glhf::Polyhedron p0 = createPolyhedron();
    glhf::Polyhedron p1 = p0;
    ASSERT_EQ(p0.vertices.data(), start_vertices);
    ASSERT_EQ(p0.faces.data(), start_faces);
    ASSERT_NE(p0.vertices.data(), p1.vertices.data());
    ASSERT_NE(p0.faces.data(), p1.faces.data());
}