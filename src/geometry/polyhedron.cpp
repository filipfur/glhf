#include "glhf/geometry/polyhedron.h"

#include "glhf/geometry/dcel.h"
#include "glhf/geometry/ray.h"
#include "glhf/log.h"
#include <numeric>

glhf::Polygon glhf::Polyhedron::to_polygon(const glhf::Face &face) const {
    glhf::Polygon polygon(face.size());
    std::transform(face.begin(), face.end(), polygon->begin(),
                   [this](uint16_t face) { return this->vertices.at(face); });
    return polygon;
}

std::pair<glhf::Polyhedron, glhf::Polyhedron>
glhf::Polyhedron::split(const glhf::Plane &plane) const {
    glhf::Polyhedron frontPart;
    glhf::Polyhedron backPart;
    DCEL dcel(*this);

    glhf::Ray ray;

    for (const auto &face : faces) {
        auto polygon = to_polygon(face);
        Polygon frontPoly;
        Polygon backPoly;
        polygon.forEachEdge([&](const glm::vec3 &a, const glm::vec3 &b) {
            bool aFront = plane.sideOf(a) >= 0;
            bool bFront = plane.sideOf(b) >= 0;
            if (aFront && bFront) {
                frontPoly->push_back(b);
            } else if (!aFront && !bFront) {
                backPoly->push_back(b);
            } else {
                ray.origin = a;
                ray.direction = glm::normalize(b - a);
                auto hit = ray.intersect(plane);
                assert(hit.has_value());
                if (aFront) {
                    frontPoly->push_back(hit->origin);
                    backPoly->push_back(hit->origin);
                    backPoly->push_back(b);
                } else {
                    frontPoly->push_back(hit->origin);
                    frontPoly->push_back(b);
                    backPoly->push_back(hit->origin);
                }
            }
        });
        if (!frontPoly->empty()) {
            frontPart.insert(frontPoly);
        }
        if (!backPoly->empty()) {
            backPart.insert(backPoly);
        }
    }
    return {frontPart, backPart};
}

glhf::Polyhedron glhf::createPolyhedron(std::span<glm::vec3> vs, std::span<uint16_t> is) {
    Polyhedron polyhedron{
        .vertices{vs.begin(), vs.end()},
        .faces{static_cast<size_t>(is.size() / 3)},
    };
    for (size_t i{0}; i < is.size(); i += 3) {
        auto &face = polyhedron.faces.at(i / 3);
        face.push_back(is[i + 0]);
        face.push_back(is[i + 1]);
        face.push_back(is[i + 2]);
    }
    return polyhedron;
}