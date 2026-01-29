#pragma once

#include "glhf/geometry/polyhedron.h"
#include "glhf/log.h"
#include <list>

namespace glhf {

// doubly connected edge list
struct DCEL {
    struct Vertex;
    struct Face;
    struct HalfEdge {
        Vertex *a;
        Vertex *b;
        Face *face;
        HalfEdge *next;
        HalfEdge *prev;
        HalfEdge *opposite;
    };

    struct Vertex {
        explicit Vertex(const glm::vec3 &p_) : p{p_} {}
        glm::vec3 p;
        HalfEdge *edge;
        operator glm::vec3() const { return p; }
    };

    struct Face {
        HalfEdge *edge;
        glm::vec3 center() const {
            glm::vec3 P{0.0f, 0.0f, 0.0f};
            size_t count{0};
            for_each_edge([&](const HalfEdge &he) {
                P += *he.a;
                ++count;
            });
            return P / static_cast<float>(count);
        }
        glm::vec3 normal() const {
            const glm::vec3 &A = *edge->a;
            const glm::vec3 &B = *edge->b;
            const glm::vec3 &C = *edge->prev->a;
            glm::vec3 AB = B - A;
            glm::vec3 AC = C - A;
            return glm::cross(AB, AC);
        }
        template <typename Callable> void for_each_edge(Callable callable) {
            HalfEdge *he = edge;
            do {
                callable(*he);
                he = he->next;
            } while (he != edge);
        }
        template <typename Callable> void for_each_edge(Callable callable) const {
            const HalfEdge *he = edge;
            do {
                callable(*he);
                he = he->next;
            } while (he != edge);
        }
    };

    DCEL(const Polyhedron &polyhedron);
    ~DCEL() noexcept;

    size_t indexOf(const HalfEdge *edge) {
        size_t i{0};
        for (const auto &e : edges) {
            if (&e == edge) {
                return i;
            }
            ++i;
        }
        return SIZE_MAX;
    }

    std::unordered_set<const HalfEdge *> uniqueEdges() const;

    void quadratize() {
        auto unique_edges = uniqueEdges();
        for (auto *ue : unique_edges) {
            if (glm::dot(glm::normalize(ue->face->normal()),
                         glm::normalize(ue->opposite->face->normal())) > 0.999f) {
                size_t removed_faces = faces.remove_if([ue](const DCEL::Face &f) {
                    return &f == ue->face || &f == ue->opposite->face;
                });
                LOGV_TRACE(removed_faces);

                ue->opposite->prev->next = ue->next;
                ue->next->prev = ue->opposite->prev;

                ue->prev->next = ue->opposite->next;
                ue->opposite->next->prev = ue->prev;

                auto &quad = faces.emplace_back();

                auto *he = ue->prev;
                quad.edge = he;

                do {
                    he->face = &quad;
                    he = he->next;
                    LOGS_TRACE(he->a->p);
                } while (he != ue->prev);

                auto *ue_o = ue->opposite;
                if (ue->a->edge == ue) {
                    ue->a->edge = ue->prev;
                }
                if (ue->b->edge == ue) {
                    ue->b->edge = ue->prev->prev;
                }

                if (ue_o->a->edge == ue_o) {
                    ue_o->a->edge = ue_o->prev;
                }
                if (ue_o->b->edge == ue_o) {
                    ue_o->b->edge = ue_o->prev->prev;
                }

                size_t removed_edges = edges.remove_if(
                    [ue, ue_o](const DCEL::HalfEdge &e) { return &e == ue || &e == ue_o; });
                LOGV_TRACE(removed_edges);
            }
        }
    }

    std::list<Face> faces;
    std::list<HalfEdge> edges;
    std::list<Vertex> vertices;
};

} // namespace glhf