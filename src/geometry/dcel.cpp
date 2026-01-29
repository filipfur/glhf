#include "glhf/geometry/dcel.h"

#include <unordered_map>

glhf::DCEL::DCEL(const glhf::Polyhedron &polyhedron) {
    std::unordered_map<uint16_t, glhf::DCEL::Vertex *> vmap;
    std::unordered_map<uint16_t, std::unordered_map<uint16_t, glhf::DCEL::HalfEdge *>> emap;
    for (const auto &pface : polyhedron.faces) {
        auto &face = faces.emplace_back();
        HalfEdge *he = nullptr;
        HalfEdge *prev = nullptr;
        for (size_t i = pface.size() - 1, j = 0; j < pface.size(); i = j++) {
            uint16_t ii = pface[i];
            uint16_t ij = pface[j];
            const glm::vec3 &pi = polyhedron.vertices.at(ii);
            const glm::vec3 &pj = polyhedron.vertices.at(ij);

            prev = he;
            he = emap[ii][ij] ? emap[ii][ij] : &this->edges.emplace_back();
            if (!face.edge) {
                face.edge = he;
            }
            if (!he->face) {
                he->face = &face;
            }

            he->a = vmap[ii] ? vmap[ii] : &this->vertices.emplace_back(pi);
            he->b = vmap[ij] ? vmap[ij] : &this->vertices.emplace_back(pj);

            if (prev) {
                prev->next = he;
                he->prev = prev;
            }

            emap[ii][ij] = he;
            if (auto *opposite = emap[ij][ii]) {
                he->opposite = opposite;
                opposite->opposite = he;
            }
        }
        he->next = face.edge;
        face.edge->prev = he;
    }
}

glhf::DCEL::~DCEL() noexcept { faces.clear(); }

std::unordered_set<const glhf::DCEL::HalfEdge *> glhf::DCEL::uniqueEdges() const {
    std::unordered_set<const glhf::DCEL::HalfEdge *> unique_edges;
    for (const glhf::DCEL::HalfEdge &edge : edges) {
        auto it = unique_edges.find(&edge);
        auto it2 = unique_edges.find(edge.opposite);
        if (it == unique_edges.end() && it2 == unique_edges.end()) {
            unique_edges.emplace(&edge);
        }
    }
    return unique_edges;
}