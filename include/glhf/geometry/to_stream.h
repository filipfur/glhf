#include <ostream>

#include "glhf/geometry/types.h"

std::ostream &operator<<(std::ostream &os, const glhf::Plane &plane) {
    os << "Plane{ .normal=" << plane.normal << " .d=" << plane.d << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const glhf::Sphere sphere) {
    os << "Sphere{ .origin=" << sphere.origin << " .radii=" << sphere.radii << " }";
    return os;
}