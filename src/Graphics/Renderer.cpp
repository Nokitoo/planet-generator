#include <iostream> // std::cerr

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <Graphics/API/Builder/ShaderProgram.hpp> // Graphics::API::Builder::ShaderProgram

#include <Graphics/Renderer.hpp> // Graphics::Renderer

namespace Graphics {

std::unique_ptr<Renderer> Renderer::create() {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<Renderer> renderer(new Renderer());

    if (!renderer->init()) {
        return nullptr;
    }

    return renderer;
}

void Renderer::render(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets) {
    glUniformMatrix4fv(_viewUniformLocation,
        1,
        GL_FALSE,
        glm::value_ptr(camera.getView()));
    glUniformMatrix4fv(_projUniformLocation,
        1,
        GL_FALSE,
        glm::value_ptr(camera.getProj()));

    for (const auto& planet: planets) {
        planet->getBuffer().bind();
        glDrawElements(
            GL_TRIANGLES,
            (GLuint)planet->getBuffer().getIndicesNb(),
            GL_UNSIGNED_INT,
            0
            );
    }
}

bool Renderer::init() {
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Activate back culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return initShaderProgram();
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

    _viewUniformLocation = _shaderProgram.getUniformLocation("view");
    _projUniformLocation = _shaderProgram.getUniformLocation("proj");

    return true;
}

} // Namespace Graphics
