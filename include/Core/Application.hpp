#pragma once

#include <cstdint> // uint32_t

#include <Graphics/API/Buffer.hpp>
#include <Graphics/Renderer.hpp>
#include <Window/Window.hpp>

namespace Core {

class Application {
public:
    Application() = default;
    ~Application() = default;

    Application(const Application& app) = delete;
    Application(Application&& app) = delete;

    Application& operator=(const Application& app) = delete;
    Application&& operator=(Application&& app) = delete;

    bool init();
    bool run();

private:
    bool initCube();

private:
    std::unique_ptr<Window::Window> _window = nullptr;
    std::unique_ptr<Graphics::Renderer> _renderer = nullptr;

    Graphics::API::Buffer _cubeBuffer;
    uint32_t _cubeIndicesNb = 0;
};

} // Core

