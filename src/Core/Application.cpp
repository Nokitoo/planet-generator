#include <iostream>

#include <glm/vec3.hpp> // glm::vec3

#include <Graphics/API/Builder/Buffer.hpp>

#include <Core/Application.hpp>

namespace Core {

bool Application::init() {
    _window = Window::Window::create("Application window", {100, 100}, {1280, 720});
    if (!_window) {
        // TODO: replace this with logger
        std::cerr << "Application::init: failed to create window" << std::endl;
        return false;
    }

    _renderer = Graphics::Renderer::create();
    if (!_renderer) {
        // TODO: replace this with logger
        std::cerr << "Application::init: failed to create renderer" << std::endl;
        return false;
    }

    return initCube();
}

bool Application::run() {
    while (1) {
        Window::Event event;

        while (_window->pollEvent(event)) {
            if (event.type == Window::Event::Type::Close) {
                return true;
            }
        }

        _window->beginFrame();
        _cubeBuffer.bind();
        glDrawElements(
            GL_TRIANGLES,
            (GLuint)_cubeIndicesNb,
            GL_UNSIGNED_INT,
            0
            );
        _window->endFrame();
    }

    return false;
}

bool Application::initCube() {
    struct Vertex {
        glm::vec3 pos;
    };

    float width = 0.5f;
    float height = 0.5f;
    float length = 0.5f;

    Vertex vertices[] = {
        // Front
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f)},

        // Back
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f)},

        // Left
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f)},

        // Right
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f)},

        // Top
        {glm::vec3(-width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(-width / 2.0f, height / 2.0f, length / 2.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, height / 2.0f, length / 2.0f)},

        // Bottom
        {glm::vec3(-width / 2.0f, -height / 2.0f, -length / 2.0f)},
        {glm::vec3(-width / 2.0f, -height / 2.0f, length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, -length / 2.0f)},
        {glm::vec3(width / 2.0f, -height / 2.0f, length / 2.0f)}
    };

    uint32_t indices[] = {
        // Front
        0, 1, 3,
        3, 2, 0,

        // Left
        8, 10, 11,
        11, 9, 8,

        // Back
        4, 6, 7,
        7, 5, 4,

        // Right
        13, 15, 14,
        14, 12, 13,

        // Bottom
        23, 21, 20,
        20, 22, 23,

        // Top
        19, 18, 16,
        16, 17, 19,
    };

    _cubeIndicesNb = sizeof(indices) / sizeof(indices[0]);

    Graphics::API::Builder::Buffer bufferBuilder;
    bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        0
    });
    bufferBuilder.setVertices((char*)vertices, sizeof(vertices));
    bufferBuilder.setIndices((char*)indices, sizeof(indices));

    if (!bufferBuilder.build(_cubeBuffer)) {
        // TODO: replace this with logger
        std::cerr << "Application::init: failed to create renderer" << std::endl;
        return false;
    }

    return true;
}

} // Core

