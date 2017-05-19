#pragma once

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
    std::unique_ptr<Window::Window> _window = nullptr;
};

} // Core

