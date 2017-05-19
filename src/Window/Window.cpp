#include <iostream>

#include <SDL.h>
#include <gl/glew.h>

#include <Window/Window.hpp>

namespace Window {

Window::~Window() {
    destroy();
}

std::unique_ptr<Window> Window::create(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size) {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<Window> window(new Window());

    if (!window->init(title, pos, size)) {
        return nullptr;
    }

    return window;
}

bool Window::pollEvent(Event& event) {
    SDL_Event sdlEvent;
    if (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                event.type = Event::Type::Close;
                return true;
            default:
                // Event not handled
                return false;
        }
    }

    return false;
}

void Window::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::endFrame() {
    SDL_GL_SwapWindow(_window);
}

bool Window::init(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    _window = SDL_CreateWindow(title.c_str(), pos.x, pos.y, size.x, size.y, SDL_WINDOW_OPENGL);

    if (_window == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Can't create window: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    return initOpenGL();
}

bool Window::initOpenGL() {
    // Create OpenGL context
    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Can't create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    // Init glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Can't init glew: " <<  glewGetErrorString(glewError) << std::endl;
        return false;
    }

    //Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Can't init vsync: " << SDL_GetError() << std::endl;
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return true;
}

void Window::destroy() {
    if (_glContext != nullptr) {
        SDL_GL_DeleteContext(_glContext);
        _glContext = nullptr;
    }

    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

} // Window
