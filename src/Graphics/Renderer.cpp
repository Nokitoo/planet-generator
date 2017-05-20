#include <iostream>

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

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

void Renderer::render(const API::Buffer& buffer, uint32_t indicesNb) {
    glUniformMatrix4fv(_viewUniformLocation,
        1,
        GL_FALSE,
        glm::value_ptr(_camera.getView()));
    glUniformMatrix4fv(_projUniformLocation,
        1,
        GL_FALSE,
        glm::value_ptr(_camera.getProj()));

    buffer.bind();
    glDrawElements(
        GL_TRIANGLES,
        (GLuint)indicesNb,
        GL_UNSIGNED_INT,
        0
        );
}

bool Renderer::init() {
    return initShaderProgram() && initCamera();
}

bool Renderer::initShaderProgram() {
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

    _shaderProgram.use();

    return true;
}

bool Renderer::initCamera() {
    _camera.setPos({0.0f, 0.0f, 10.0f});

    _viewUniformLocation = _shaderProgram.getUniformLocation("view");
    _projUniformLocation = _shaderProgram.getUniformLocation("proj");

    return true;
}

} // Graphics
