#include <iostream> // std::cerr

#include <imgui.h> // Imgui functions
#include <glm/vec3.hpp> // glm::vec3

#include <System/Timer.hpp> // System::Timer

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

    _camera.setPos({sphereSize / 2.0f, 0.0f, sphereSize + 50.0f});
    _camera.lookAt({0.0f, 0.0f, 0.0f});
    _camera.setNear(1.0f);
    _camera.setFar(1000.0f);
    _camera.setAspect((float)_window->getSize().x / (float)_window->getSize().y);
    _planets.push_back(std::make_unique<Core::SphereQuadTree>(sphereSize));

    return true;
}

bool Application::run() {
    System::Timer timer;

    while (1) {
        float elapsedTime = timer.getElapsedTime();
        timer.reset();

        if (!handleEvents()) {
            break;
        }

        _window->beginFrame();
        onFrame(elapsedTime);
        _renderer->render(_camera, _planets);
        _window->endFrame();
    }

    return true;
}

bool Application::handleEvents() {
    Graphics::Debug& debug = _renderer->getDebug();
    Window::Event event;

    while (_window->pollEvent(event)) {
        if (event.type == Window::Event::Type::Close ||
            (event.type == Window::Event::Type::KeyPressed &&
                event.key.code == Window::Keyboard::Key::Escape)) {
            return false;
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
            debug.verticesNormalsDisplayed(!debug.verticesNormalsDisplayed());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::B) {
            debug.facesNormalsDisplayed(!debug.facesNormalsDisplayed());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::L) {
            _frustumLocked = !_frustumLocked;
            _camera.lockFrustum(_frustumLocked);
        }
        if (event.type == Window::Event::Type::Resize) {
            _camera.setAspect((float)_window->getSize().x / (float)_window->getSize().y);
        }
    }

    return true;
}

void Application::onFrame(float elapsedTime) {
    for (auto& planet: _planets) {
        planet->update(_camera);
    }

    updateCameraPosition(elapsedTime);

    {
        static float totalElapsedTime = 0.0f;
        static int framesNb = 0;
        static float fps = 0.0f;

        totalElapsedTime += elapsedTime;
        ++framesNb;

        if (totalElapsedTime >= 1.0f) {
            fps = static_cast<float>(framesNb) / totalElapsedTime;
            totalElapsedTime = 0.0f;
            framesNb = 0;
        }

        ImGui::SetNextWindowPos(ImVec2(10,10));
        if (!ImGui::Begin(
            "Example: Fixed Overlay",
            nullptr,
            ImVec2(0,0),
            0.3f,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings
        ))
        {
            ImGui::End();
            return;
        }
        ImGui::Text("FPS: %.3f", round(fps * 100.0f) / 100.0f);
        ImGui::End();
    }
}

void Application::updateCameraPosition(float elapsedTime) {
    float moveSpeed = 50.0f;
    glm::vec3 moveDirection;

    if (_window->isKeyPressed(Window::Keyboard::Key::Q)) {
        moveDirection.x -= moveSpeed * elapsedTime;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::D)) {
        moveDirection.x += moveSpeed * elapsedTime;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::Z)) {
        moveDirection.z -= moveSpeed * elapsedTime;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::S)) {
        moveDirection.z += moveSpeed * elapsedTime;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::LShift)) {
        moveDirection.y -= moveSpeed * elapsedTime;
    }
    if (_window->isKeyPressed(Window::Keyboard::Key::Space)) {
        moveDirection.y += moveSpeed * elapsedTime;
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

