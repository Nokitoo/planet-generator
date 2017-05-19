#pragma once

#include <Window/Window.hpp>

namespace Core {

class Application {
public:
    Application() = default;
    ~Application() = default;

    bool init();
    bool run();

private:
    std::unique_ptr<Window::Window> _window = nullptr;
};

} // Core

