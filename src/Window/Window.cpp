#include <iostream>

#include <SDL.h>

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

bool Window::init(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "Window::init: Error: " << SDL_GetError() << std::endl;
        return false;
    }

    _window = SDL_CreateWindow(title.c_str(), pos.x, pos.y, size.x, size.y, SDL_WINDOW_SHOWN);

    if (_window == nullptr){
        // TODO: replace this with logger
        std::cout << "Window::init: Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    _renderer = SDL_CreateRenderer(
        _window,
        -1, // Index of the rendering driver, -1 to choose the first one supporting the requested flags
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

    if (_renderer == nullptr){
        std::cout << "Window::init: Error: " << SDL_GetError() << std::endl;
        destroy();
        return 1;
    }

    return true;
}

void Window::destroy() {
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

} // Window
