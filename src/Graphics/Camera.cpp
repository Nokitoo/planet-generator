#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::perspective

#include <Graphics/Camera.hpp>

namespace Graphics {

Camera::Camera(const Camera& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _pos = camera._pos;
    _needUpdateProj = true;
    _needUpdateView = true;
}

Camera::Camera(Camera&& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _pos = camera._pos;
    _needUpdateProj = true;
    _needUpdateView = true;
}

Camera& Camera::operator=(const Camera& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _pos = camera._pos;
    _needUpdateProj = true;
    _needUpdateView = true;

    return *this;
}

Camera& Camera::operator=(Camera&& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _pos = camera._pos;
    _needUpdateProj = true;
    _needUpdateView = true;

    return *this;
}

const glm::mat4& Camera::getView() {
    if (_needUpdateView) {
        updateView();
    }

    return _view;
}

const glm::mat4& Camera::getProj() {
    if (_needUpdateProj) {
        updateProj();
    }

    return _proj;
}

float Camera::getFov() const {
    return _fov;
}

float Camera::getFar() const {
    return _far;
}

float Camera::getNear() const {
    return _near;
}

float Camera::getAspect() const {
    return _aspect;
}

const glm::vec3& Camera::getPos() const {
    return _pos;
}

void Camera::setFov(float fov) {
    _fov = fov;

    _needUpdateProj = true;
}

void Camera::setNear(float near) {
    _near = near;

    _needUpdateProj = true;
}

void Camera::setFar(float far) {
    _far = far;

    _needUpdateProj = true;
}

void Camera::setAspect(float aspect) {
    _aspect = aspect;

    _needUpdateProj = true;
}

void Camera::setPos(const glm::vec3& pos) {
    _pos = pos;

    _needUpdateView = true;
}

void Camera::lookAt(const glm::vec3& pos) {
    glm::vec3 direction = pos - _pos;
    float cos_theta = glm::dot(glm::normalize(direction), glm::normalize(_forward));
    float angle = acos(cos_theta);
    glm::vec3 w = glm::normalize(glm::cross(direction, _forward));

    _orientation = glm::angleAxis(angle, w);
}

void Camera::updateProj() {
    _proj = glm::perspective(glm::radians(_fov), _aspect, _near, _far);

    _needUpdateProj = false;
}

void Camera::updateView() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0), -_pos);
    glm::mat4 rotate = glm::mat4_cast(_orientation);
    _view = rotate * translate;

    _needUpdateView = false;
}

} // Graphics
