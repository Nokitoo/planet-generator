#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::perspective

#include <Graphics/Camera.hpp> // Graphics::Camera

namespace Graphics {

Camera::Camera(const Camera& camera): Transform(camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _needUpdateProj = true;
}

Camera::Camera(Camera&& camera): Transform(camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _needUpdateProj = true;
}

Camera& Camera::operator=(const Camera& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _needUpdateProj = true;

    Transform::operator=(camera);

    return *this;
}

Camera& Camera::operator=(Camera&& camera) {
    _fov = camera._fov;
    _near = camera._near;
    _far = camera._far;
    _aspect = camera._aspect;
    _needUpdateProj = true;

    Transform::operator=(camera);

    return *this;
}

const glm::mat4& Camera::getView() {
    if (isDirty()) {
        updateView();
        isDirty(false);
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

void Camera::updateProj() {
    _proj = glm::perspective(glm::radians(_fov), _aspect, _near, _far);

    _needUpdateProj = false;
}

void Camera::updateView() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0), -getPos());
    glm::mat4 rotate = glm::mat4_cast(getOrientation());
    _view = rotate * translate;
}

} // Namespace Graphics
