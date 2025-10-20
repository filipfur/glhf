#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glhf {
struct TRS {
  protected:
    enum Validity {
        VALID,
        INVALID,
        INVALID_EULER,
    };

  public:
    template <typename T> struct Attribute {
        Attribute(TRS::Validity &validity, TRS::Validity invalidation, const T &t)
            : _validity{validity}, _invalidation{invalidation}, _t{t}, x{_t.x}, y{_t.y}, z{_t.z} {}
        Attribute &operator=(const T &t) {
            _t = t;
            _validity = _invalidation;
            return *this;
        }
        Attribute &operator+=(const T &t) {
            _t += t;
            _validity = _invalidation;
            return *this;
        }
        Attribute &operator-=(const T &t) {
            _t -= t;
            _validity = _invalidation;
            return *this;
        }
        Attribute &operator*=(const T &t) {
            _t *= t;
            _validity = _invalidation;
            return *this;
        }
        Attribute &operator/=(const T &t) {
            _t /= t;
            _validity = _invalidation;
            return *this;
        }

        const T &data() const { return _t; }
        T &data() { return _t; }

        Validity &_validity;
        Validity _invalidation;
        T _t;
        const float &x;
        const float &y;
        const float &z;
    };

    TRS()
        : translation{_validity, INVALID, {0.0f, 0.0f, 0.0f}},
          scale{_validity, INVALID, {1.0f, 1.0f, 1.0f}},
          rotation{_validity, INVALID, {1.0f, 0.0f, 0.0f, 0.0f}},
          euler{_validity, INVALID_EULER, {0.0f, 0.0f, 0.0f}} {}

    Attribute<glm::vec3> translation;
    Attribute<glm::vec3> scale;
    Attribute<glm::quat> rotation;
    Attribute<glm::vec3> euler;

    glm::mat4 &model();
    glm::vec3 position();

    TRS *parent();
    void setParent(TRS *parent);

    void setTransform(TRS &other) {
        _model = other.model();
        _validity = VALID;
    }

    const glm::vec3 &t() { return translation.data(); }
    glm::vec3 xz() { return glm::vec3{translation.x, 0.0f, translation.z}; }

    const glm::quat &r() { return rotation.data(); }

    const glm::vec3 &s() { return scale.data(); }

    void invalidate() { _validity = INVALID; }
    bool isValid() const { return _validity == VALID; }

  private:
    Validity _validity{INVALID};
    glm::mat4 _model{1.0f};
    TRS *_parent{nullptr};
};
} // namespace glhf
