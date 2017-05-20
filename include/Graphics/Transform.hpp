#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtx/quaternion.hpp> // glm::quat

namespace Graphics {

class Transform
{
public:
    Transform() = default;
    virtual ~Transform() = default;

    Transform(const Transform& transform);
    Transform(Transform&& transform);

    Transform& operator=(const Transform& transform);
    Transform& operator=(Transform&& transform);

    const glm::vec3& getPos() const;
    const glm::quat& getOrientation() const;
    bool isDirty() const;

    void setPos(const glm::vec3& pos);
    void isDirty(bool dirty);

    void lookAt(const glm::vec3& pos);
    void translate(const glm::vec3& direction);
    void rotate(float amount, const glm::vec3& axis);

private:
    glm::vec3 _pos;
    glm::vec3 _forward = {0.0f, 0.0f, -1.0f};
    glm::quat _orientation;

    bool _dirty = true;
};

} // Namespace Graphics
