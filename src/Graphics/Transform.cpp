#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::perspective

#include <Graphics/Transform.hpp>

namespace Graphics {

Transform::Transform(const Transform& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    isDirty(true);
}

Transform::Transform(Transform&& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    isDirty(true);

    camera._pos = {};
    camera._orientation = {};
}

Transform& Transform::operator=(const Transform& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    isDirty(true);

    return *this;
}

Transform& Transform::operator=(Transform&& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    isDirty(true);

    camera._pos = {};
    camera._orientation = {};

    return *this;
}

const glm::vec3& Transform::getPos() const {
    return _pos;
}

const glm::quat& Transform::getOrientation() const {
    return _orientation;
}

bool Transform::isDirty() const {
    return _dirty;
}

void Transform::setPos(const glm::vec3& pos) {
    _pos = pos;
    isDirty(true);
}

void Transform::isDirty(bool dirty) {
    _dirty = dirty;
}

// Calculations from http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
void Transform::lookAt(const glm::vec3& pos) {
    glm::vec3 u = glm::normalize(pos - _pos);
    glm::vec3 v = _forward;

    float normUNormV = sqrt(glm::dot(u, u) * glm::dot(v, v));
    float realPart = normUNormV + glm::dot(u, v);
    glm::vec3 w;

    if (realPart < 1.e-6f * normUNormV)
    {
        /* If u and v are exactly opposite, rotate 180 degrees
         * around an arbitrary orthogonal axis. Axis normalisation
         * can happen later, when we normalise the quaternion. */
        realPart = 0.0f;
        w = abs(u.x) > abs(u.z) ? glm::vec3(-u.y, u.x, 0.f)
                                : glm::vec3(0.f, -u.z, u.y);
    }
    else
    {
        /* Otherwise, build quaternion the standard way. */
        w = cross(u, v);
    }

    _orientation = glm::normalize(glm::quat(realPart, w.x, w.y, w.z));
    isDirty(true);
}

void Transform::translate(const glm::vec3& direction) {
    _pos += getOrientation() * direction;
    isDirty(true);
}

} // Graphics
