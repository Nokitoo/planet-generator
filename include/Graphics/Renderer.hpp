#pragma once

#include <memory> // std::unique_ptr

#include <Graphics/API/ShaderProgram.hpp>

namespace Graphics {

class Renderer {
public:
    ~Renderer() = default;

    Renderer(const Renderer& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    Renderer& operator=(const Renderer& renderer) = delete;
    Renderer& operator=(Renderer&& renderer) = delete;

    static std::unique_ptr<Renderer> create();

private:
    // Only the Renderer::create can create the renderer
    Renderer() = default;

    bool init();

private:
    API::ShaderProgram _shaderProgram;
};

} // Graphics
