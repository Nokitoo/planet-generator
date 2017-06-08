#include <iostream> // std::cerr

#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <Graphics/API/Builder/Framebuffer.hpp> // Graphics::API::Builder::Framebuffer
#include <Graphics/API/Builder/ShaderProgram.hpp> // Graphics::API::Builder::ShaderProgram
#include <Graphics/API/Framebuffer.hpp> // Graphics::API::Framebuffer

#include <Graphics/Renderer.hpp> // Graphics::Renderer

namespace Graphics {

Renderer::Renderer(const Window::Window* window): _window(window) {}

std::unique_ptr<Renderer> Renderer::create(const Window::Window* window) {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<Renderer> renderer(new Renderer(window));

    if (!renderer->init()) {
        return nullptr;
    }

    return renderer;
}

void Renderer::render(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets) {
    if (!_debug.wireframeDisplayed()) {
        glUniform1i(_mainShaderProgram.getUniformLocation("heightMap"), 0);
        glUniform1i(_mainShaderProgram.getUniformLocation("normalMap"), 1);
        renderPlanets(_mainShaderProgram, camera, planets);
    }

    if (_debug.isActivated()) {
        // Use debug shader
        _debugShaderProgram.use();

        glUniform1i(_debugShaderProgram.getUniformLocation("heightMap"), 0);
        glUniform1i(_debugShaderProgram.getUniformLocation("normalMap"), 1);

        glUniform1i(_debugShaderProgram.getUniformLocation("wireframeDisplayed"), _debug.wireframeDisplayed());
        glUniform1i(_debugShaderProgram.getUniformLocation("verticesNormalsDisplayed"), _debug.verticesNormalsDisplayed());
        glUniform1i(_debugShaderProgram.getUniformLocation("facesNormalsDisplayed"), _debug.facesNormalsDisplayed());

        renderPlanets(_debugShaderProgram, camera, planets);

        // Use main shader program here
        // so we don't do multiple times _mainShaderProgram.use() if debug is not activated
        _mainShaderProgram.use();
    }

    if (_debug.aabbDisplayed()) {
        renderPlanetsAABBDebug(camera, planets);
    }
}

Debug& Renderer::getDebug() {
    return _debug;
}

void Renderer::createNormalMapFromHeightMap(const API::Texture& heightMap, const API::Texture& normalMap, float maxHeight) const {
    static Graphics::API::Framebuffer* fbo = nullptr;

    // Init framebuffer
    // TODO: Move in SphereQuadTree to prevent recreating the color attachments array ?
    if (!fbo) {
        fbo = new Graphics::API::Framebuffer();
        Graphics::API::Builder::Framebuffer framebufferBuilder;

        framebufferBuilder.build(*fbo);
    }

    fbo->bind();

    // Update color attachments
    fbo->removeColorAttachments();
    for (int i = 0; i < 6; ++i) {
        fbo->addColorAttachment(&normalMap, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    // Use the framebuffer to specify the attachments used in the fragment shader
    fbo->use();

    _normalMapShaderProgram.use();

    glUniform1f(_normalMapShaderProgram.getUniformLocation("maxHeight"), maxHeight);
    glUniform1f(_normalMapShaderProgram.getUniformLocation("imageSize"), (GLfloat)normalMap.getWidth());

    // Bind height map for read
    glBindImageTexture(
        0,
        static_cast<GLuint>(heightMap),
        0,
        GL_TRUE,
        0,
        GL_READ_ONLY,
        GL_RGBA32F
    );

    glViewport(0, 0, normalMap.getWidth(), normalMap.getWidth());

    // Render the normal map
    glDrawArrays(GL_TRIANGLES, 0, 3);

    fbo->unBind();
    _mainShaderProgram.use();

    // Reset viewport
    glViewport(0, 0, _window->getSize().x, _window->getSize().y);
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
        planet->getHeightMap().bind(GL_TEXTURE0);
        planet->getNormalMap().bind(GL_TEXTURE1);
        glDrawElements(
            GL_TRIANGLES,
            (GLuint)planet->getBuffer().getIndicesNb(),
            GL_UNSIGNED_INT,
            0
            );
    }
}

void Renderer::renderPlanetsAABBDebug(Camera& camera, const std::vector<std::unique_ptr<Core::SphereQuadTree>>& planets) {
    // Disable back culling to see AABB when we are inside it
    // Setup blending
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Use AABB debug shader
    _aabbDebugShaderProgram.use();

    glUniformMatrix4fv(_aabbDebugShaderProgram.getUniformLocation("view"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.getView()));
    glUniformMatrix4fv(_aabbDebugShaderProgram.getUniformLocation("proj"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.getProj()));

    for (const auto& planet: planets) {
        planet->getDebugBuffer().bind();
        glDrawElements(
            GL_TRIANGLES,
            (GLuint)planet->getDebugBuffer().getIndicesNb(),
            GL_UNSIGNED_INT,
            0
            );
    }

    // Use main shader program here
    // so we don't do multiple times _mainShaderProgram.use() if debug is not activated
    _mainShaderProgram.use();

    // Restore back culling and blend states
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
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

    // Debug shader program to debug normals
    {
        API::Builder::ShaderProgram shaderProgramBuilder;
        if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/shader.vert") ||
            !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/debug.frag") ||
            !shaderProgramBuilder.setShader(GL_GEOMETRY_SHADER, "resources/shaders/debug.geom")) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't init debug shaders" << std::endl;
            return false;
        }

        if (!shaderProgramBuilder.build(_debugShaderProgram)) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't create debug shader program" << std::endl;
            return false;
        }
    }


    // Debug shader program to debug quadtrees AABB (used for frustum culling and horizon culling)
    {
        API::Builder::ShaderProgram shaderProgramBuilder;
        if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/aabb.debug.vert") ||
            !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/aabb.debug.frag")) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't init aabb debug shaders" << std::endl;
            return false;
        }

        if (!shaderProgramBuilder.build(_aabbDebugShaderProgram)) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't create aabb debug shader program" << std::endl;
            return false;
        }
    }

    // Compute shader program
    {
        API::Builder::ShaderProgram shaderProgramBuilder;
        if (!shaderProgramBuilder.setShader(GL_VERTEX_SHADER, "resources/shaders/screen-triangle.vert") ||
            !shaderProgramBuilder.setShader(GL_FRAGMENT_SHADER, "resources/shaders/normal.frag")) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't init normal map shaders" << std::endl;
            return false;
        }

        if (!shaderProgramBuilder.build(_normalMapShaderProgram)) {
            // TODO: replace this with logger
            std::cerr << "Renderer::init: Can't create normal map shader program" << std::endl;
            return false;
        }
    }

    // Make the ShaderProgram store "maxHeight" and "imageSize" locations because it's used in Renderer::createNormalMapFromHeightMap
    // which is const and can't modify the ShaderProgram
    _normalMapShaderProgram.getUniformLocation("maxHeight");
    _normalMapShaderProgram.getUniformLocation("imageSize");

    _mainShaderProgram.use();

    glUniform1i(_mainShaderProgram.getUniformLocation("heightMap"), 0);
    glUniform1i(_mainShaderProgram.getUniformLocation("normalMap"), 1);

    return true;
}

} // Namespace Graphics
