#pragma once

#include <memory> // std::unique_ptr

#include <GL/glew.h> // GLuint

#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer
#include <Graphics/API/ShaderProgram.hpp> // Graphics::API::ShaderProgram
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

    void render(Camera& camera, const std::vector<const API::Buffer*>& buffers);

private:
    // Only the Renderer::create can create the renderer
    Renderer() = default;
    bool init();

private:
    bool initShaderProgram();

private:
    API::ShaderProgram _shaderProgram;

    GLuint _viewUniformLocation;
    GLuint _projUniformLocation;
};

} // Namespace Graphics

