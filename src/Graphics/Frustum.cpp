#include <Graphics/Camera.hpp> // Graphics::Camera

#include <Graphics/Frustum.hpp> // Graphics::Frustum

namespace Graphics {

float Frustum::Plane::distance(const glm::vec3& p) const {
    return abs(glm::dot(normal, p) + d);
}

float Frustum::Plane::signedDistance(const glm::vec3& p) const {
    return glm::dot(normal, p) + d;
}

Frustum::Frustum(const Frustum& frustum) {
    _nearPlane = frustum._nearPlane;
    _farPlane = frustum._farPlane;
    _planes = frustum._planes;
}

Frustum::Frustum(Frustum&& frustum) {
    _nearPlane = frustum._nearPlane;
    _farPlane = frustum._farPlane;
    _planes = std::move(frustum._planes);
}

Frustum& Frustum::operator=(const Frustum& frustum) {
    _nearPlane = frustum._nearPlane;
    _farPlane = frustum._farPlane;
    _planes = frustum._planes;

    return *this;
}

Frustum& Frustum::operator=(Frustum&& frustum) {
    _nearPlane = frustum._nearPlane;
    _farPlane = frustum._farPlane;
    _planes = std::move(frustum._planes);

    return *this;
}

void Frustum::update(const Camera& camera) {
    float fov = glm::radians(camera.getFov());
    _nearPlane.distance = camera.getNear();
    _nearPlane.height = 2.0f * tan(fov / 2.0f) * _nearPlane.distance;
    _nearPlane.width = _nearPlane.height * camera.getAspect();

    _farPlane.distance = camera.getFar();
    _farPlane.height = 2.0f * tan(fov / 2.0f) * _farPlane.distance;
    _farPlane.width = _farPlane.height * camera.getAspect();

    _planes[PlaneOrientation::FRONT].point = camera.getPos() + (camera.getDir() * _nearPlane.distance);
    _planes[PlaneOrientation::FRONT].normal = camera.getDir();
    _planes[PlaneOrientation::FRONT].d = -glm::dot(_planes[PlaneOrientation::FRONT].normal, _planes[PlaneOrientation::FRONT].point);

    _planes[PlaneOrientation::BACK].point = camera.getPos() + (camera.getDir() * _farPlane.distance);
    _planes[PlaneOrientation::BACK].normal = -camera.getDir();
    _planes[PlaneOrientation::BACK].d = -glm::dot(_planes[PlaneOrientation::BACK].normal, _planes[PlaneOrientation::BACK].point);

    _planes[PlaneOrientation::TOP].point = _planes[PlaneOrientation::FRONT].point + (camera.getUp() * (_nearPlane.height / 2.0f));
    _planes[PlaneOrientation::TOP].normal = glm::cross(camera.getRight(), _planes[PlaneOrientation::TOP].point - camera.getPos());
    _planes[PlaneOrientation::TOP].normal = glm::normalize(-_planes[PlaneOrientation::TOP].normal);
    _planes[PlaneOrientation::TOP].d = -glm::dot(_planes[PlaneOrientation::TOP].normal, _planes[PlaneOrientation::TOP].point);

    _planes[PlaneOrientation::BOTTOM].point = _planes[PlaneOrientation::FRONT].point - (camera.getUp() * (_nearPlane.height / 2.0f));
    _planes[PlaneOrientation::BOTTOM].normal = glm::cross(camera.getRight(), _planes[PlaneOrientation::BOTTOM].point - camera.getPos());
    _planes[PlaneOrientation::BOTTOM].normal = glm::normalize(_planes[PlaneOrientation::BOTTOM].normal);
    _planes[PlaneOrientation::BOTTOM].d = -glm::dot(_planes[PlaneOrientation::BOTTOM].normal, _planes[PlaneOrientation::BOTTOM].point);

    _planes[PlaneOrientation::LEFT].point = _planes[PlaneOrientation::FRONT].point - (camera.getRight() * (_nearPlane.width / 2.0f));
    _planes[PlaneOrientation::LEFT].normal = glm::cross(camera.getUp(), _planes[PlaneOrientation::LEFT].point - camera.getPos());
    _planes[PlaneOrientation::LEFT].normal = glm::normalize(-_planes[PlaneOrientation::LEFT].normal);
    _planes[PlaneOrientation::LEFT].d = -glm::dot(_planes[PlaneOrientation::LEFT].normal, _planes[PlaneOrientation::LEFT].point);

    _planes[PlaneOrientation::RIGHT].point = _planes[PlaneOrientation::FRONT].point + (camera.getRight() * (_nearPlane.width / 2.0f));
    _planes[PlaneOrientation::RIGHT].normal = glm::cross(camera.getUp(), _planes[PlaneOrientation::RIGHT].point - camera.getPos());
    _planes[PlaneOrientation::RIGHT].normal = glm::normalize(_planes[PlaneOrientation::RIGHT].normal);
    _planes[PlaneOrientation::RIGHT].d = -glm::dot(_planes[PlaneOrientation::RIGHT].normal, _planes[PlaneOrientation::RIGHT].point);
}

bool Frustum::isPointInside(const glm::vec3& point) const {
    for (auto& plane: _planes) {
        if (plane.second.signedDistance(point) < 0) {
            return false;
        }
    }

    return true;
}

bool Frustum::isPlaneInside(
    const glm::vec3& posA,
    const glm::vec3& posB,
    const glm::vec3& posC,
    const glm::vec3& posD
) const {
    for (auto& plane: _planes) {
        if (plane.second.signedDistance(posA) < 0 &&
            plane.second.signedDistance(posB) < 0 &&
            plane.second.signedDistance(posC) < 0 &&
            plane.second.signedDistance(posD) < 0) {
            return false;
        }
    }

    return true;
}

bool Frustum::isAABBInside(
    const glm::vec3& posA,
    const glm::vec3& posB,
    const glm::vec3& posC,
    const glm::vec3& posD,
    const glm::vec3& posE,
    const glm::vec3& posF,
    const glm::vec3& posG,
    const glm::vec3& posH
) const {
    for (auto& plane: _planes) {
        if (plane.second.signedDistance(posA) < 0 &&
            plane.second.signedDistance(posB) < 0 &&
            plane.second.signedDistance(posC) < 0 &&
            plane.second.signedDistance(posD) < 0 &&
            plane.second.signedDistance(posE) < 0 &&
            plane.second.signedDistance(posF) < 0 &&
            plane.second.signedDistance(posG) < 0 &&
            plane.second.signedDistance(posH) < 0) {
            return false;
        }
    }

    return true;
}

} // Namespace Graphics
