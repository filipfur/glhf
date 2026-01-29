#include "glhf/geometry/polygon.h"

#include <numeric>

glm::vec3 glhf::Polygon::center() const {
    return std::accumulate(points.begin(), points.end(), glm::vec3{0.0f, 0.0f, 0.0f}) /
           (float)points.size();
}

glm::vec3 glhf::Polygon::normal() const {
    assert(points.size() > 2);
    const glm::vec3 &A = points.at(0);
    const glm::vec3 &B = points.at(1);
    const glm::vec3 &C = points.at(2);
    return glm::cross(B - A, C - A);
}