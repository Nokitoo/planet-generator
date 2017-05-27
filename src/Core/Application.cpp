#include <iostream> // std::cerr

#include <glm/vec3.hpp> // glm::vec3

#include <Core/Application.hpp> // Graphics::Core::Application

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

    float sphereSize = 100.0f;

    _camera.setPos({sphereSize / 2.0f, sphereSize * 1.5f, sphereSize * 1.5f});
    _camera.lookAt({0.0f, 0.0f, 0.0f});
    _camera.setNear(2.0f);
    _camera.setFar(1000.0f);
    _planets.push_back(std::make_unique<Core::SphereQuadTree>(sphereSize));

    return true;
}

bool Application::run() {
    Graphics::Debug& debug = _renderer->getDebug();
    while (1) {
        Window::Event event;

        while (_window->pollEvent(event)) {
            if (event.type == Window::Event::Type::Close ||
                (event.type == Window::Event::Type::KeyPressed &&
                    event.key.code == Window::Keyboard::Key::Escape)) {
                return true;
            }
            if (event.type == Window::Event::Type::MouseMoved) {
                updateCameraRotation(event);
            }

            if (event.type == Window::Event::Type::KeyPressed &&
                event.key.code == Window::Keyboard::Key::M) {
                debug.wireframeDisplayed(!debug.wireframeDisplayed());
            }
            if (event.type == Window::Event::Type::KeyPressed &&
                event.key.code == Window::Keyboard::Key::N) {
                debug.normalsDisplayed(!debug.normalsDisplayed());
            }
        }

        _window->beginFrame();
        onFrame();
        _renderer->render(_camera, _planets);
        _window->endFrame();
    }

    return false;
}

void Application::onFrame() {
    for (auto& planet: _planets) {
        planet->update(_camera);
    }
    updateCameraPosition();
}

void Application::updateCameraPosition() {
    float moveSpeed = 1.0f;
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
    if (_window->isKeyPressed(Window::Keyboard::Key::LShift)) {
        moveDirection.y -= moveSpeed;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::Space)) {
        moveDirection.y += moveSpeed;
    }

    _camera.translate(moveDirection);
}

void Application::updateCameraRotation(Window::Event& event) {
    float rotationSpeed = 0.5f;
    _camera.rotate(
            glm::radians(rotationSpeed),
            {
                event.mouse.moveOffset.y,
                event.mouse.moveOffset.x,
                0.0f
            }
        );
}

} // Namespace Core

