#pragma once

#include <memory> // std::unique_ptr

#include <GL/glew.h> // GLuint

#include <Core/SphereQuadTree.hpp> // Graphics::API::Buffer
#include <Graphics/API/ShaderProgram.hpp> // Graphics::API::ShaderProgram
#include <Graphics/Debug.hpp> // Graphics::Debug
#include <Graphics/Camera.hpp> // Graphics::Camera

namespace Graphics {

class Renderer {
public:
    ~Renderer() = default;

    Renderer(const Renderer& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    Renderer& operator=(const Renderer& renderer) = delete;
    Renderer& operator=(Renderer&& renderer) = delete;

    static std::unique_ptr<Renderer> create();

    void render(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);

    Debug& getDebug();

private:
    // Only the Renderer::create can create the renderer
    Renderer() = default;
    bool init();

    void renderPlanets(API::ShaderProgram& shaderProgram, Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);
    void renderPlanetsAABBDebug(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);

private:
    bool initShaderProgram();

private:
    API::ShaderProgram _mainShaderProgram;
    API::ShaderProgram _debugShaderProgram;
    API::ShaderProgram _aabbDebugShaderProgram;

    API::ShaderProgram* _currentShaderProgram = nullptr;

    Debug _debug;
};

} // Namespace Graphics

