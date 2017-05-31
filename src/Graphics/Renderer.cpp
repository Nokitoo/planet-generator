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
    if (!_debug.wireframeDisplayed()) {
        renderPlanets(_mainShaderProgram, camera, planets);
    }

    if (_debug.isActivated()) {
        _debugShaderProgram.use();

        glUniform1i(_debugShaderProgram.getUniformLocation("wireframeDisplayed"), _debug.wireframeDisplayed());
        glUniform1i(_debugShaderProgram.getUniformLocation("verticesNormalsDisplayed"), _debug.verticesNormalsDisplayed());
        glUniform1i(_debugShaderProgram.getUniformLocation("facesNormalsDisplayed"), _debug.facesNormalsDisplayed());

        renderPlanets(_debugShaderProgram, camera, planets);
        _mainShaderProgram.use();
    }
}

Debug& Renderer::getDebug() {
    return _debug;
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

void Renderer::renderPlanets(API::ShaderProgram& shaderProgram, Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets) {
    glUniformMatrix4fv(shaderProgram.getUniformLocation("view"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.getView()));
    glUniformMatrix4fv(shaderProgram.getUniformLocation("proj"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.getProj()));

    for (const auto& planet: planets) {
        glUniform1f(shaderProgram.getUniformLocation("planetSize"), planet->getSize());
        glUniform1f(shaderProgram.getUniformLocation("maxHeight"), planet->getMaxHeight());
        planet->getBuffer().bind();
        planet->getHeightMap().bind();
        glDrawElements(
            GL_TRIANGLES,
            (GLuint)planet->getBuffer().getIndicesNb(),
            GL_UNSIGNED_INT,
            0
            );
    }
}

bool Renderer::initShaderProgram() {
    // Main shader program
    {
        API::Builder::ShaderProgram shaderProgramBuilder;
        if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert") ||
            !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag")) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't init shaders" << std::endl;
            return false;
        }

        if (!shaderProgramBuilder.build(_mainShaderProgram)) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't create main shader program" << std::endl;
            return false;
        }
    }

    // Debug shader program
    {
        API::Builder::ShaderProgram shaderProgramBuilder;
        if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert") ||
            !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/debug.frag") ||
            !shaderProgramBuilder.setShader(GL_GEOMETRY_SHADER, "resources/shaders/debug.geom")) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't init shaders" << std::endl;
            return false;
        }

        if (!shaderProgramBuilder.build(_debugShaderProgram)) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't create debug shader program" << std::endl;
            return false;
        }
    }

    _mainShaderProgram.use();

    return true;
}

} // Namespace Graphics
