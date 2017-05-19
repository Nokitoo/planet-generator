#include <iostream>

#include <Graphics/API/Builder/ShaderProgram.hpp>

#include <Graphics/Renderer.hpp>

namespace Graphics {

std::unique_ptr<Renderer> Renderer::create() {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<Renderer> renderer(new Renderer());

    if (!renderer->init()) {
        return nullptr;
    }

    return renderer;
}

bool Renderer::init() {
    API::Builder::ShaderProgram shaderProgramBuilder;

    if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert") ||
        !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag")) {
        // TODO: replace this with logger
        std::cerr << "Renderer::init: Can't init shaders" << std::endl;
        return false;
    }

    if (!shaderProgramBuilder.build(_shaderProgram)) {
        // TODO: replace this with logger
        std::cerr << "Renderer::init: Can't create shader program" << std::endl;
        return false;
    }

    return true;
}

} // Graphics
