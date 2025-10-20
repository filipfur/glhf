#include "glhf/trs.h"

#include <glm/glm.hpp>

static glm::quat fromEuler(const glm::vec3 &rotation) {
    return glm::angleAxis(glm::radians(rotation.y), glm::vec3{0.0f, 1.0f, 0.0f}) *
           glm::angleAxis(glm::radians(rotation.x), glm::vec3{1.0f, 0.0f, 0.0f}) *
           glm::angleAxis(glm::radians(rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
}

glm::mat4 &glhf::TRS::model() {
    glm::mat4 local;
    switch (_validity) {
    case INVALID_EULER:
        rotation = fromEuler(euler.data());
        // intentional fall-through
    case INVALID:
        local = glm::scale(glm::translate(glm::mat4(1.0f), translation.data()) *
                               glm::mat4(glm::mat3_cast(rotation.data())),
                           scale.data());
        if (_parent) {
            _model = _parent->model() * local;
        } else {
            _model = local;
        }
        _validity = VALID;
        break;
    default:
        break;
    }
    return _model;
}

glm::vec3 glhf::TRS::position() {
    auto &m = model();
    return {m[3][0], m[3][1], m[3][2]};
}

glhf::TRS *glhf::TRS::parent() { return _parent; }

void glhf::TRS::setParent(TRS *parent) { _parent = parent; }