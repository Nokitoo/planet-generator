#include <Core/Application.hpp>

namespace Core {

bool Application::init() {
    _window = Window::Window::create("Application window", {100, 100}, {1280, 720});

    return _window != nullptr;
}

bool Application::run() {
    while (1) {
        Window::Event event;

        while (_window->pollEvent(event)) {
            if (event.type == Window::Event::Type::Close) {
                return true;
            }
        }
    }

    return false;
}

} // Core

