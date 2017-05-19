#pragma once

#include <memory> // std::unique_ptr
#include <string> // std::string

#include <glm/vec2.hpp> // glm::ivec2

#include <Window/Event.hpp>

// We don't want to include SDL2.hpp in Window.hpp
struct SDL_Window;

namespace Window {

class Window {
public:
    ~Window();

    Window(const Window& window) = delete;
    Window(Window&& window) = delete;

    Window& operator=(const Window& window) = delete;
    Window& operator=(Window&& window) = delete;

    static std::unique_ptr<Window> create(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size);

    bool pollEvent(Event& event);

    void beginFrame();
    void endFrame();

private:
    // Only the Window::create can create the window
    Window() = default;

    bool init(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size);
    bool initOpenGL();

    void destroy();

private:
    SDL_Window* _window = nullptr;
    void* _glContext = nullptr;

    glm::ivec2  _size;
};

} // Window

