#include "glhf/trs.h"

#include <glm/glm.hpp>

#ifdef GLHF_USE_TRS_EULER
static glm::quat _fromEuler(const glm::vec3 &rotation) {
    return glm::angleAxis(glm::radians(rotation.y), glm::vec3{0.0f, 1.0f, 0.0f}) *
           glm::angleAxis(glm::radians(rotation.x), glm::vec3{1.0f, 0.0f, 0.0f}) *
           glm::angleAxis(glm::radians(rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
}
#endif

const glm::mat4 &glhf::TRS::transformation() {
    glm::mat4 local;
    if (_validity == VALID && _parent &&
        (_parent->_localVersion != _parentVersion || _parent->_validity != VALID)) {
        _validity = INVALID;
    }
    switch (_validity) {
#ifdef GLHF_USE_TRS_EULER
    case INVALID_EULER:
        _rotation = _fromEuler(euler.data());
        // intentional fall-through
#endif
    case INVALID_ROTATION:
        _rotationMatrix = glm::mat3_cast(_rotation);
        // intentional fall-through
    case INVALID:
        local = glm::scale(
            glm::translate(glm::mat4(1.0f), _translation) * glm::mat4(_rotationMatrix), _scale);
        if (_parent) {
            _transformation = _parent->transformation() * local;
            _parentVersion = _parent->_localVersion;
        } else {
            _transformation = local;
        }
        _validity = VALID;
        ++_localVersion;
        break;
    default:
        break;
    }
    return _transformation;
}