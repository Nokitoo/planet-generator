#include <iostream> // std::cerr

#include <imgui.h> // Imgui functions
#include <glm/vec3.hpp> // glm::vec3

#include <System/Timer.hpp> // System::Timer

#include <Core/Application.hpp> // Graphics::Core::Application

namespace Core {

bool Application::init() {
    _window = Window::Window::create("Application window", {100, 100}, {1280, 720});
    if (_window == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Application::init: failed to create window" << std::endl;
        return false;
    }

    _renderer = Graphics::Renderer::create(_window.get());
    if (_renderer == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Application::init: failed to create renderer" << std::endl;
        return false;
    }

    float planetSize = 100.0f;
    float planetMaxHeight = 20.0f;

    _camera.setPos({0.0f, 0.0f, planetSize + 50.0f});
    _camera.setNear(1.0f);
    _camera.setFar(9999999.0f);
    _camera.setAspect((float)_window->getSize().x / (float)_window->getSize().y);

    std::unique_ptr<Core::SphereQuadTree> planet = Core::SphereQuadTree::create(_renderer.get(), planetSize, planetMaxHeight);
    if (planet == nullptr) {
        std::cerr << "Application::init: failed to create planet" << std::endl;
        return false;
    }

    _planets.push_back(std::move(planet));

    // TODO: Remove this line
    // Why is the normal map not generating without the update ?
    _planets.back()->update(_camera);

    _renderer->createNormalMapFromHeightMap(_planets.back()->getHeightMap(), _planets.back()->getNormalMap(), _planets.back()->getMaxHeight());

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
        if (event.type == Window::Event::Type::MousePressed &&
            event.mouse.button == Window::Mouse::Button::Right) {
            _window->relativeMouseModeEnabled(!_window->relativeMouseModeEnabled());
        }

        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::F1) {
            debug.verticesNormalsDisplayed(!debug.verticesNormalsDisplayed());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::F2) {
            debug.facesNormalsDisplayed(!debug.facesNormalsDisplayed());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::F3) {
            debug.wireframeDisplayed(!debug.wireframeDisplayed());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::F4) {
            _camera.frustumLocked(!_camera.frustumLocked());
        }
        if (event.type == Window::Event::Type::KeyPressed &&
            event.key.code == Window::Keyboard::Key::F5) {
            debug.aabbDisplayed(!debug.aabbDisplayed());
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

    displayOverlayWindow(elapsedTime);
    displayCommandsWindow();
    displayDebugWindow();
    displayEditorWindow();

    updateCameraPosition(elapsedTime);
}

void Application::displayOverlayWindow(float elapsedTime) {
    // Display overlay window
    {
        ImGui::SetNextWindowSize(ImVec2(400, 50));
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        if (!ImGui::Begin(
            "Fixed Overlay",
            nullptr,
            ImVec2(0, 0),
            0.3f,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
        ))
        {
            ImGui::End();
            return;
        }
    }

    // Display FPS
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

        ImGui::Text("FPS: %.3f", round(fps * 100.0f) / 100.0f);
    }

    // Display planets vertices count
    for (uint32_t i = 0; i < _planets.size(); ++i) {
        ImGui::Text(
            "Planet %d vertices: %d (%d Kb)",
            i,
            _planets[i]->getBuffer().getVerticesNb(),
            sizeof(QuadTree::Vertex) * _planets[i]->getBuffer().getVerticesNb() / 1000
        );
    }

    ImGui::End();
}

void Application::displayCommandsWindow() {
    // Display overlay window
    {
        ImGui::SetNextWindowSize(ImVec2(400, 100));
        ImGui::SetNextWindowPos(ImVec2(10, 70));
        if (!ImGui::Begin(
            "Commands",
            nullptr,
            ImVec2(0, 0),
            0.3f,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
        ))
        {
            ImGui::End();
            return;
        }
    }

    ImGui::Text("Commands:");
    ImGui::Text("Z,Q,S,D: Move front,left,back,right");
    ImGui::Text("Space,Left Shift: Move up/down");
    ImGui::Text("Hold Left Control: Increase move speed");
    ImGui::Text("Right mouse click: Toggle mouse mode");
    ImGui::End();
}

void Application::displayDebugWindow() {
    ImGui::SetNextWindowPos(ImVec2(10, 230), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Debug##Display", nullptr, ImVec2(0, 0)))
    {
        ImGui::End();
        return;
    }

    bool verticesNormalsDisplayed = _renderer->getDebug().verticesNormalsDisplayed();
    if (ImGui::Checkbox("Vertices normals (F1)", &verticesNormalsDisplayed)) {
        _renderer->getDebug().verticesNormalsDisplayed(verticesNormalsDisplayed);
    }

    bool facesNormalsDisplayed = _renderer->getDebug().facesNormalsDisplayed();
    if (ImGui::Checkbox("Faces normals (F2)", &facesNormalsDisplayed)) {
        _renderer->getDebug().facesNormalsDisplayed(facesNormalsDisplayed);
    }

    bool wireframeDisplayed = _renderer->getDebug().wireframeDisplayed();
    if (ImGui::Checkbox("Wireframe (F3)", &wireframeDisplayed)) {
        _renderer->getDebug().wireframeDisplayed(wireframeDisplayed);
    }

    bool frustumLocked = _camera.frustumLocked();
    if (ImGui::Checkbox("Frustum locked (F4)", &frustumLocked)) {
        _camera.frustumLocked(frustumLocked);
    }

    bool aabbDisplayed = _renderer->getDebug().aabbDisplayed();
    if (ImGui::Checkbox("AABB (F5)", &aabbDisplayed)) {
        _renderer->getDebug().aabbDisplayed(aabbDisplayed);
    }

    ImGui::End();
}

void Application::displayEditorWindow() {
    if (!_planets.size()) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(10, 400), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Editor", nullptr, ImVec2(0, 0)))
    {
        ImGui::End();
        return;
    }

    // TODO: Editor multiple planets
    auto& planet = _planets.front();

    ImGui::PushItemWidth(200);

    float maxHeight = planet->getMaxHeight();
    if (ImGui::SliderFloat("Max height", &maxHeight, 0.0f, 500.0f, "%.0f")) {
        planet->setMaxHeight(maxHeight);
        _renderer->createNormalMapFromHeightMap(planet->getHeightMap(), planet->getNormalMap(), planet->getMaxHeight());
    }

    float size = planet->getSize();
    if (ImGui::SliderFloat("Size", &size, 0.0f, 500.0f, "%.0f")) {
        planet->setSize(size);
    }

    ImGui::PopItemWidth();

    ImGui::End();
}

void Application::updateCameraPosition(float elapsedTime) {
    float moveSpeed = 50.0f;
    glm::vec3 moveDirection;

    if (_window->isKeyPressed(Window::Keyboard::Key::LControl)) {
        moveSpeed *= 5.0f;
    }

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
    if (!_window->relativeMouseModeEnabled()) {
        return;
    }

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

