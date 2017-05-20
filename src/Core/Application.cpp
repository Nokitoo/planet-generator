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

    _camera.setPos({0.0f, 30.0f, 30.0f});
    _camera.lookAt({0.0f, 0.0f, 0.0f});

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
        onFrame();
        _renderer->render(_camera, _cubeBuffer, _cubeIndicesNb);
        _window->endFrame();
    }

    return false;
}

bool Application::initCube() {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
    };

    float width = 10.0f;
    float height = 10.0f;
    float length = 10.0f;

    Vertex vertices[] = {
        // Front
        { {-width / 2.0f, height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {width / 2.0f, height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {-width / 2.0f, -height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
        { {width / 2.0f, -height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },

        // Back
        { {-width / 2.0f, height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { {width / 2.0f, height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { {-width / 2.0f, -height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
        { {width / 2.0f, -height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },

        // Left
        { {-width / 2.0f, height / 2.0f, -length / 2.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
        { {-width / 2.0f, height / 2.0f, length / 2.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
        { {-width / 2.0f, -height / 2.0f, -length / 2.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
        { {-width / 2.0f, -height / 2.0f, length / 2.0f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },

        // Right
        { {width / 2.0f, height / 2.0f, -length / 2.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { {width / 2.0f, height / 2.0f, length / 2.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { {width / 2.0f, -height / 2.0f, -length / 2.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
        { {width / 2.0f, -height / 2.0f, length / 2.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },

        // Top
        { {-width / 2.0f, height / 2.0f, -length / 2.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { {-width / 2.0f, height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { {width / 2.0f, height / 2.0f, -length / 2.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
        { {width / 2.0f, height / 2.0f, length / 2.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },

        // Bottom
        { {-width / 2.0f, -height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {-width / 2.0f, -height / 2.0f, length / 2.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {width / 2.0f, -height / 2.0f, -length / 2.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
        { {width / 2.0f, -height / 2.0f, length / 2.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}
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
    // Position attribute
    bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, pos)
    });
    // Color attribute
    bufferBuilder.addAttribute({
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, color)
    });
    // Normal attribute
    bufferBuilder.addAttribute({
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, normal)
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

void Application::onFrame() {
    float moveSpeed = 0.5f;
    glm::vec3 moveDirection;

    if (_window->isKeyPressed(Window::Keyboard::Key::Q)) {
        moveDirection.x -= moveSpeed;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::D)) {
        moveDirection.x += moveSpeed;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::Z)) {
        moveDirection.z -= moveSpeed;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::S)) {
        moveDirection.z += moveSpeed;
    }

    _camera.translate(moveDirection);
}

} // Core

