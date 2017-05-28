#pragma once

#include <cstdint> // uint8_t
#include <unordered_map> // uint8_t

#include <glm/vec3.hpp>

namespace Graphics {

class Camera;

class Frustum
{
private:
    struct NearPlane {
        float width = 0.0f;
        float height = 0.0f;
        float distance = 0.0f;
    };

    struct FarPlane {
        float width = 0.0f;
        float height = 0.0f;
        float distance = 0.0f;
    };

    struct Plane {
        glm::vec3 point;
        glm::vec3 normal;
        float d;

        float distance(const glm::vec3& p) const;
        float signedDistance(const glm::vec3& p) const;
    };

    enum class PlaneOrientation: uint8_t {
        TOP = 0,
        BOTTOM = 1,
        LEFT = 2,
        RIGHT = 3,
        FRONT = 4,
        BACK = 5
    };

public:
    Frustum() = default;
    ~Frustum() = default;

    Frustum(const Frustum& frustum);
    Frustum(Frustum&& frustum);

    Frustum& operator=(const Frustum& frustum);
    Frustum& operator=(Frustum&& frustum);

    void update(const Camera& camera);

    bool isPointInside(const glm::vec3& point) const;
    bool isPlaneInside(
        const glm::vec3& posA,
        const glm::vec3& posB,
        const glm::vec3& posC,
        const glm::vec3& posD
    ) const;
    bool isShapeInside(const std::vector<glm::vec3>& pos) const;

private:
    NearPlane _nearPlane;
    FarPlane _farPlane;

    std::unordered_map<PlaneOrientation, Plane> _planes;
};

} // Namespace Graphics
