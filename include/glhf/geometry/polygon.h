#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace glhf {

struct Polygon {

    Polygon() : points{} {}
    explicit Polygon(std::initializer_list<glm::vec3> points_) : points{points_} {}
    explicit Polygon(size_t count) : points{count} {}

    std::vector<glm::vec3> &operator*() { return points; }
    const std::vector<glm::vec3> &operator*() const { return points; }
    std::vector<glm::vec3> *operator->() { return &points; }
    const std::vector<glm::vec3> *operator->() const { return &points; }

    template <typename Callable> inline void forEachEdge(Callable callable) {
        for (size_t i = points.size() - 1, j = 0; j < 3; i = j++) {
            callable(points.at(i), points.at(j));
        }
    }

    glm::vec3 center() const;
    glm::vec3 normal() const;

    std::vector<glm::vec3> points;
};

} // namespace glhf