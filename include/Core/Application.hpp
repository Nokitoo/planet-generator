#pragma once

#include <cstdint> // uint32_t
#include <memory> // std::unique_ptr
#include <vector> // std::vector

#include <Core/SphereQuadTree.hpp> // Core::SphereQuadTree
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Graphics/Renderer.hpp> // Graphics::Renderer
#include <Window/Window.hpp> // Window::Window

namespace Core {

class Application {
public:
    Application() = default;
    ~Application() = default;

    Application(const Application& app) = delete;
    Application(Application&& app) = delete;

    Application& operator=(const Application& app) = delete;
    Application& operator=(Application&& app) = delete;

    bool init();
    bool run();

private:
    bool handleEvents();
    void onFrame(float elapsedTime);

    void displayOverlayWindow(float elapsedTime);
    void displayCommandsWindow();
    void displayDebugWindow();
    void displayEditorWindow();

    void updateCameraPosition(float elapsedTime);
    void updateCameraRotation(Window::Event& event);

private:
    std::unique_ptr<Window::Window> _window = nullptr;
    std::unique_ptr<Graphics::Renderer> _renderer = nullptr;

    std::vector<std::unique_ptr<Core::SphereQuadTree>> _planets;

    Graphics::Camera _camera;
};

} // Namespace Core

