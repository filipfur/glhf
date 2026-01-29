#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glhf {
struct TRS {
    const glm::vec3 &translation() const { return _translation; }
    const glm::quat &rotation() const { return _rotation; }
    const glm::vec3 &scale() const { return _scale; }
#ifdef GLHF_USE_TRS_EULER
    const glm::vec3 &euler() const { return _euler; }
#endif

    glm::vec3 &translation() {
        if (_validity == VALID) {
            _validity = INVALID;
        }
        return _translation;
    }

    glm::quat &rotation() {
        _validity = INVALID_ROTATION;
        return _rotation;
    }

    glm::vec3 &scale() {
        if (_validity == VALID) {
            _validity = INVALID;
        }
        return _scale;
    }

#ifdef GLHF_USE_TRS_EULER
    glm::quat &euler() {
        _validity = INVALID_EULER;
        return _euler;
    }
#endif

    glm::vec3 globalTranslation() const {
        auto &m = transformation();
        return {m[3][0], m[3][1], m[3][2]};
    }

    const glm::mat4 &transformation() const { return _transformation; }
    const glm::mat4 &transformation();
    void setTransformation(const glm::mat4 &transformation) {
        // TODO: oversee
        _validity = VALID;
        _transformation = transformation;
    }

    const TRS *parent() const { return this; }
    void setParent(TRS *trs) { _parent = trs; }

    bool valid() const { return _validity == VALID; }
    void invalidate() { _validity = INVALID; }

  private:
    enum Validity {
        INVALID,
        INVALID_ROTATION,
#ifdef GLHF_USE_TRS_EULER
        INVALID_EULER,
#endif
        VALID,
    };
    glm::vec3 _translation{0.0f, 0.0f, 0.0f};
    glm::quat _rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 _scale{0.0f, 0.0f, 0.0f};
#ifdef GLHF_USE_TRS_EULER
    glm::vec3 _euler{0.0f, 0.0f, 0.0f};
#endif
    TRS *_parent{nullptr};
    glm::mat4 _rotationMatrix{1.0f};
    glm::mat4 _transformation{1.0f};
    Validity _validity{VALID};
    uint8_t _localVersion{0};
    uint8_t _parentVersion{0};
};
} // namespace glhf
