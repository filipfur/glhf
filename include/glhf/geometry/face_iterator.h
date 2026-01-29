#pragma once

#include "glhf/geometry/face.h"
#include <glm/glm.hpp>
#include <iterator>
#include <vector>

namespace glhf {
struct const_face_iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Face;
    using pointer = value_type *;
    using reference = value_type &;

    const_face_iterator(const std::vector<glm::vec3> &vertices,
                        std::vector<Face>::const_iterator face)
        : _vertices{vertices}, _face{face} {}

    const glm::vec3 &A() const { return _vertices.at(_face->a); }
    const glm::vec3 &B() const { return _vertices.at(_face->b); }
    const glm::vec3 &C() const { return _vertices.at(_face->c); }

    const glm::vec3 &operator[](int index) const { return _vertices.at((&_face->a)[index]); }

    glm::vec3 AB() const { return B() - A(); }
    glm::vec3 AC() const { return C() - A(); }

    glm::vec3 normal() const { return glm::cross(AB(), AC()); }

    const Face &operator*() const { return *_face; }
    const Face *operator->() const { return &(*_face); }

    const_face_iterator &operator++() {
        ++_face;
        return *this;
    }

    const_face_iterator operator++(int) {
        const_face_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const const_face_iterator &a, const const_face_iterator &b) {
        return a._face == b._face;
    };
    friend bool operator!=(const const_face_iterator &a, const const_face_iterator &b) {
        return a._face != b._face;
    };

  private:
    const std::vector<glm::vec3> &_vertices;
    std::vector<Face>::const_iterator _face;
};

struct face_iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Face;
    using pointer = value_type *;
    using reference = value_type &;

    face_iterator(std::vector<glm::vec3> &vertices, std::vector<Face>::iterator face)
        : _vertices{vertices}, _face{face} {}

    glm::vec3 &A() { return _vertices.at(_face->a); }
    glm::vec3 &B() { return _vertices.at(_face->b); }
    glm::vec3 &C() { return _vertices.at(_face->c); }

    const glm::vec3 &A() const { return _vertices.at(_face->a); }
    const glm::vec3 &B() const { return _vertices.at(_face->b); }
    const glm::vec3 &C() const { return _vertices.at(_face->c); }

    glm::vec3 &operator[](int index) { return _vertices.at((&_face->a)[index]); }
    const glm::vec3 &operator[](int index) const { return _vertices.at((&_face->a)[index]); }

    glm::vec3 AB() const { return B() - A(); }
    glm::vec3 AC() const { return C() - A(); }

    glm::vec3 normal() const { return glm::cross(AB(), AC()); }

    Face &operator*() const { return *_face; }
    Face *operator->() { return &(*_face); }

    face_iterator &operator++() {
        ++_face;
        return *this;
    }

    face_iterator operator++(int) {
        face_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const face_iterator &a, const face_iterator &b) {
        return a._face == b._face;
    };
    friend bool operator!=(const face_iterator &a, const face_iterator &b) {
        return a._face != b._face;
    };

  private:
    std::vector<glm::vec3> &_vertices;
    std::vector<Face>::iterator _face;
};

template <typename T> face_iterator face_iterator_begin(T &t) {
    return face_iterator{t.vertices, t.faces.begin()};
}

template <typename T> face_iterator face_iterator_end(T &t) {
    return face_iterator{t.vertices, t.faces.end()};
}

template <typename T> const_face_iterator const_face_iterator_begin(const T &t) {
    return const_face_iterator{t.vertices, t.faces.begin()};
}

template <typename T> const_face_iterator const_face_iterator_end(const T &t) {
    return const_face_iterator{t.vertices, t.faces.end()};
}

} // namespace glhf
