#pragma once

#include "glhf/geometry/dcel.h"
#include "glhf/geometry/plane.h"
#include "glhf/geometry/polyhedron.h"
#include "glhf/geometry/sphere.h"
#include <list>
#include <memory>
#include <vector>

namespace glhf {

struct BSP {
    struct Node {
        std::unique_ptr<Plane> plane;
        glm::vec3 origin;
        Node *front;
        Node *back;
        bool solid;
        bool isLeaf() const { return !(front || back); }
        bool isSolid() const { return solid; }

        template <typename Callable>
        void forEachNode(Callable callable, int depth = 0, int breadth = 0) const {
            callable(*this, depth, breadth);
            if (front) {
                front->forEachNode(callable, depth + 1, breadth + 1);
            }
            if (back) {
                back->forEachNode(callable, depth + 1, breadth - 1);
            }
        }
    };

    const Node *root() const { return nodes.empty() ? nullptr : &nodes.front(); }

    bool pointInside(const glm::vec3 &point) const;
    Plane *intersectingPlane(const glm::vec3 &point) const;
    std::vector<glhf::Plane *> intersectingPlane(const glhf::Sphere &sphere) const;

    Node *build(glhf::Polyhedron polyhedron, size_t depth);
    std::list<Node> nodes;
};

} // namespace glhf
