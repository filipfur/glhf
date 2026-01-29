#pragma once

// #include "face_iterator.h"
#include "glhf/geometry/plane.h"
#include "glhf/geometry/polygon.h"
#include "glhf/geometry/triangle.h"
#include "glhf/mesh.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <span>

namespace glhf {

using Face = std::vector<uint16_t>;

struct Polyhedron {

    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;

    static constexpr float EPSILON = 1e-6f;

    size_t indexOf(const glm::vec3 &p) {
        size_t i{0};
        for (; i < vertices.size(); ++i) {
            glm::vec3 d = p - vertices[i];
            if (glm::dot(d, d) < FLT_EPSILON) {
                return i;
            }
        }
        return SIZE_MAX;
    }

    Polyhedron &operator+=(const Polyhedron &other) {
        for (const auto &face : other.faces) {
            insert(other.to_polygon(face));
        }
        return *this;
    }

    Polyhedron operator+(const Polyhedron &other) const {
        Polyhedron rval{*this};
        rval += other;
        return rval;
    }

    size_t findOrInsert(const glm::vec3 &p) {
        size_t index = indexOf(p);
        if (index == SIZE_MAX) {
            vertices.push_back(p);
            return vertices.size() - 1;
        }
        return index;
    }

    void insert(const Polygon &polygon) {
        auto &face = faces.emplace_back();
        for (const glm::vec3 &p : *polygon) {
            face.push_back(findOrInsert(p));
        }
    }

    std::vector<Triangle> triangles() const {
        std::vector<Triangle> tris;
        for (const auto &face : faces) {
            if (face.size() == 3) {
                tris.emplace_back(face.at(0), face.at(1), face.at(2));
            } else if (face.size() == 4) {
                tris.emplace_back(face.at(0), face.at(1), face.at(2));
                tris.emplace_back(face.at(0), face.at(2), face.at(3));
            } else {
                assert(false); // TODO
            }
        }
        return tris;
    }

    void quadratize() {
        std::unordered_map<uint16_t, std::unordered_map<uint16_t, Face *>> fmap;
        for (auto &f : faces) {
            if (f.size() != 3) {
                continue;
            }
            for (size_t i = 2, j = 0; j < 3; i = j++) {
                fmap[f[i]][f[j]] = &f;
            }
        }
        for (auto &e1 : fmap) {
            for (auto &e2 : e1.second) {
                if (e1.first >= e2.first) {
                    continue;
                }
                auto *f1 = e2.second;
                auto *f2 = fmap[e2.first][e1.first];
                if (f2 == nullptr || f1->size() != 3 || f2->size() != 3) {
                    continue;
                }
                auto p1 = to_polygon(*f1);
                auto p2 = to_polygon(*f2);
                glm::vec3 n1 = glm::normalize(p1.normal());
                glm::vec3 n2 = glm::normalize(p2.normal());
                if (glm::dot(n1, n2) > 0.999f) {
                    for (uint16_t val : *f2) {
                        if (val != e1.first && val != e2.first) {
                            f1->push_back(val);
                            f2->clear();
                            break;
                        }
                    }
                }
            }
        }
        auto it = std::remove_if(faces.begin(), faces.end(),
                                 [](const glhf::Face &face) { return face.size() == 0; });
        faces.erase(it, faces.end());
    }

    Polygon to_polygon(const Face &face) const;

    std::pair<glhf::Polyhedron, glhf::Polyhedron> split(const glhf::Plane &plane) const;
};

Polyhedron createPolyhedron(std::span<glm::vec3> vs, std::span<uint16_t> is);

} // namespace glhf
