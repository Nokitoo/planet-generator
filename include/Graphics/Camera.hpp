#pragma once

#include <glm/mat4x4.hpp> // glm::mat4

#include <Graphics/Transform.hpp>

namespace Graphics {

class Camera final: public Transform
{
public:
    Camera() = default;
    ~Camera() final override = default;

    Camera(const Camera& camera);
    Camera(Camera&& camera);

    Camera& operator=(const Camera& camera);
    Camera& operator=(Camera&& camera);

    const glm::mat4& getView();
    const glm::mat4& getProj();
    float getFov() const;
    float getFar() const;
    float getNear() const;
    float getAspect() const;

    void  setFov(float fov);
    void  setNear(float near);
    void  setFar(float far);
    void  setAspect(float far);

private:
    void  updateProj();
    void  updateView();

private:
    bool  _needUpdateProj = true;

    // Field of view
    float _fov = 45.f;
    // Near clipping plane
    float _near = 0.1f;
    // Far clipping plane
    float _far = 100.0f;

    float _aspect = 16.0f / 9.0f;

    glm::mat4 _proj;
    glm::mat4 _view;
};

} // Graphics
