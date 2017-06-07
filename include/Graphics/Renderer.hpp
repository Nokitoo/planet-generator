#pragma once

#include <memory> // std::unique_ptr

#include <GL/glew.h> // GLuint

#include <Core/SphereQuadTree.hpp> // Graphics::API::Buffer
#include <Graphics/API/ShaderProgram.hpp> // Graphics::API::ShaderProgram
#include <Graphics/API/Texture.hpp> // Graphics::API::Texture
#include <Graphics/Debug.hpp> // Graphics::Debug
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Window/Window.hpp> // Window::Window

namespace Graphics {

class Renderer {
public:
    ~Renderer() = default;

    Renderer(const Renderer& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    Renderer& operator=(const Renderer& renderer) = delete;
    Renderer& operator=(Renderer&& renderer) = delete;

    static std::unique_ptr<Renderer> create(const Window::Window* window);

    void render(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);

    Debug& getDebug();

    void createNormalMapFromHeightMap(const API::Texture& heightMap, const API::Texture& normalMap, float maxHeight) const;

private:
    // Only the Renderer::create can create the renderer
    Renderer(const Window::Window* window);
    bool init();

    void renderPlanets(API::ShaderProgram& shaderProgram, Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);
    void renderPlanetsAABBDebug(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets);

private:
    bool initShaderProgram();

private:
    const Window::Window* _window = nullptr;

    API::ShaderProgram _mainShaderProgram;
    API::ShaderProgram _debugShaderProgram;
    API::ShaderProgram _aabbDebugShaderProgram;

    API::ShaderProgram _normalMapShaderProgram;

    Debug _debug;
};

} // Namespace Graphics

