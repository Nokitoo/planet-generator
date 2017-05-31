#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtx/quaternion.hpp> // glm::quat

namespace Graphics {

class Transform
{
public:
    Transform();
    virtual ~Transform() = default;

    Transform(const Transform& transform);
    Transform(Transform&& transform);

    Transform& operator=(const Transform& transform);
    Transform& operator=(Transform&& transform);

    const glm::vec3& getPos() const;
    const glm::quat& getOrientation() const;
    const glm::vec3& getForward() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getUp() const;
    bool isDirty() const;

    void setPos(const glm::vec3& pos);
    void isDirty(bool dirty);

    void lookAt(const glm::vec3& pos);
    void translate(const glm::vec3& direction);
    void rotate(float amount, const glm::vec3& axis);

private:
    glm::vec3 _pos;
    glm::quat _orientation;

    glm::vec3 _worldForward = {0.0f, 0.0f, -1.0f};
    glm::vec3 _worldRight = {1.0f, 0.0f, 0.0f};
    glm::vec3 _worldUp = {0.0f, 1.0f, 0.0f};

    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;

    bool _dirty = true;
};

} // Namespace Graphics
