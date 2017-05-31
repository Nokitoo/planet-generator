#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::perspective

#include <Graphics/Transform.hpp> // Graphics::Transform

namespace Graphics {

Transform::Transform(const Transform& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    _localForward = camera._localForward;
    _localRight = camera._localRight;
    _localUp = camera._localUp;
    isDirty(true);
}

Transform::Transform(Transform&& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    _localForward = camera._localForward;
    _localRight = camera._localRight;
    _localUp = camera._localUp;
    isDirty(true);

    camera._pos = {};
    camera._orientation = {};
}

Transform& Transform::operator=(const Transform& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    _localForward = camera._localForward;
    _localRight = camera._localRight;
    _localUp = camera._localUp;
    isDirty(true);

    return *this;
}

Transform& Transform::operator=(Transform&& camera) {
    _pos = camera._pos;
    _orientation = camera._orientation;
    _localForward = camera._localForward;
    _localRight = camera._localRight;
    _localUp = camera._localUp;
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

const glm::vec3& Transform::getDir() const {
    return _dir;
}

const glm::vec3& Transform::getRight() const {
    return _right;
}
const glm::vec3& Transform::getUp() const {
    return _up;
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
    glm::vec3 v = _localForward;

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
    _dir = glm::normalize(_localForward * _orientation);
    _right = glm::normalize(_localRight * _orientation);
    _up = glm::normalize(_localUp * _orientation);

    isDirty(true);
}

void Transform::translate(const glm::vec3& direction) {
    if (!direction.x && !direction.y && !direction.z) {
        return;
    }

    _pos += direction * getOrientation();
    isDirty(true);
}

void Transform::rotate(float amount, const glm::vec3& axis) {
    if (!amount) {
        return;
    }

    glm::quat pitch = glm::angleAxis(axis.x * amount, _localRight);
    glm::quat yaw = glm::angleAxis(axis.y * amount, _localUp);

    _orientation = glm::normalize(pitch * _orientation * yaw);

    _dir = glm::normalize(_localForward * _orientation);
    _right = glm::normalize(_localRight * _orientation);
    _up = glm::normalize(_localUp * _orientation);

    isDirty(true);
}

} // Namespace Graphics
