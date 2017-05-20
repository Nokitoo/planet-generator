#pragma once

#include <memory> // std::unique_ptr

#include <GL/glew.h> // GLuint

#include <Graphics/API/Buffer.hpp>
#include <Graphics/API/ShaderProgram.hpp>
#include <Graphics/Camera.hpp>

namespace Graphics {

class Renderer {
public:
    ~Renderer() = default;

    Renderer(const Renderer& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    Renderer& operator=(const Renderer& renderer) = delete;
    Renderer& operator=(Renderer&& renderer) = delete;

    static std::unique_ptr<Renderer> create();

    void render(Camera& camera, const API::Buffer& buffer, uint32_t indicesNb);

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

