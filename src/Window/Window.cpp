#include <iostream> // std::cerr

#include <imgui.h> // Imgui functions
#include <imgui_impl_sdl_gl3.h> // Imgui SDL abstraction functions
#include <gl/glew.h> // OpenGL functions
#include <SDL.h> // SDL_Window

#include <Window/Window.hpp> // Window::Window

#undef main

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
        // Send event to imgui
        ImGui_ImplSdlGL3_ProcessEvent(&sdlEvent);

        // Handle events
        switch (sdlEvent.type) {
            case SDL_QUIT:
                event.type = Event::Type::Close;
                return true;
            case SDL_KEYDOWN:
                event.type = Event::Type::KeyPressed;
                event.key.code = translateSDLKey(sdlEvent.key.keysym.sym);
                if (event.key.code == Keyboard::Key::Unknown) {
                    return pollEvent(event);
                }
                _keyStates[event.key.code] = true;
                return true;
            case SDL_KEYUP:
                event.type = Event::Type::KeyReleased;
                event.key.code = translateSDLKey(sdlEvent.key.keysym.sym);
                if (event.key.code == Keyboard::Key::Unknown) {
                    return pollEvent(event);
                }
                _keyStates[event.key.code] = false;
                return true;
            case SDL_MOUSEMOTION:
                event.type = Event::Type::MouseMoved;
                event.mouse.pos = {sdlEvent.button.x, sdlEvent.button.y};
                event.mouse.moveOffset = {sdlEvent.motion.xrel, sdlEvent.motion.yrel};
                return true;
            case SDL_MOUSEBUTTONDOWN:
                event.type = Event::Type::MousePressed;
                event.mouse.pos = {sdlEvent.button.x, sdlEvent.button.y};
                event.mouse.button = translateSDLMouseButton(sdlEvent.button.button);
                return true;
            case SDL_MOUSEBUTTONUP:
                event.type = Event::Type::MouseReleased;
                event.mouse.pos = {sdlEvent.button.x, sdlEvent.button.y};
                event.mouse.button = translateSDLMouseButton(sdlEvent.button.button);
                return true;
            case SDL_MOUSEWHEEL:
                event.type = Event::Type::MouseScroll;
                event.mouse.pos = {sdlEvent.button.x, sdlEvent.button.y};
                event.mouse.scrollOffset = {sdlEvent.wheel.x, sdlEvent.wheel.y};
                return true;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ||
                    sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    event.type = Event::Type::Resize;
                    return true;
                }
                return false;
            default:
                // Event not handled
                return pollEvent(event);
        }
    }

    return false;
}

void Window::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _size.x, _size.y);

    ImGui_ImplSdlGL3_NewFrame(_window);
}

void Window::endFrame() {
    ImGui::Render();

    SDL_GL_SwapWindow(_window);
}

bool Window::isKeyPressed(Keyboard::Key key) const {
    // Key does not exists
    if (_keyStates.find(key) == _keyStates.end()) {
        return false;
    }

    return _keyStates.at(key);
}

const char* Window::keyToString(Keyboard::Key key) const {
#define KEY_COND(KEY, SDL_KEY)          \
    if (Keyboard::Key::KEY == key) {    \
        return #KEY;                    \
    }
    KEYBOARD_KEYS(KEY_COND)
#undef KEY_COND

    return "";
}

bool Window::isMouseButtonPressed(Mouse::Button button) const {
    // Mouse button does not exists
    if (_mouseStates.find(button) == _mouseStates.end()) {
        return false;
    }

    return _mouseStates.at(button);
}

const char* Window::mouseButtonToString(Mouse::Button button) const {
#define MOUSE_COND(BUTTON, SDL_BUTTON)         \
    if (Mouse::Button::BUTTON == button) {     \
        return #BUTTON;                        \
    }
    MOUSE_BUTTONS(MOUSE_COND)
#undef MOUSE_COND

    return "";
}

bool Window::init(const std::string& title, const glm::ivec2& pos, const glm::ivec2& size) {
    _size = size;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Use a depth buffer of 24 bits
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    _window = SDL_CreateWindow(title.c_str(), pos.x, pos.y, size.x, size.y, SDL_WINDOW_OPENGL);

    if (_window == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Window::init: Can't create window: " << SDL_GetError() << std::endl;
        return false;
    }

    initKeyStates();
    initMouseStates();

    // Remove unwanted events from window creation
    // (Mouse motion whose value is very high)
    Event event;
    while (pollEvent(event)) {}

    SDL_SetRelativeMouseMode(SDL_TRUE);

    return initOpenGL();
}

const glm::ivec2& Window::getSize() const {
    return _size;
}

bool Window::initOpenGL() {
    // Create OpenGL context
    _glContext = SDL_GL_CreateContext(_window);
    if (_glContext == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Window::initOpenGL: Can't create OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_MakeCurrent(_window, _glContext);

    //Use Vsync
/*    if (SDL_GL_SetSwapInterval(1) < 0) {
        // TODO: replace this with logger
        std::cerr << "Window::initOpenGL: Can't init vsync: " << SDL_GetError() << std::endl;
        return false;
    }*/

    // Init glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        // TODO: replace this with logger
        std::cerr << "Window::initOpenGL: Can't init glew: " <<  glewGetErrorString(glewError) << std::endl;
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Setup imgui bindings
    _imguiInit = ImGui_ImplSdlGL3_Init(_window);
    if (!_imguiInit) {
        // TODO: replace this with logger
        std::cerr << "Window::initOpenGL: Can't init imgui bindings" << std::endl;
        return false;
    }

    return SDL_TRUE;
}

void Window::initKeyStates() {
#define KEY_INIT(KEY, SDL_KEY) _keyStates[Keyboard::Key::KEY] = false;
    KEYBOARD_KEYS(KEY_INIT)
#undef KEY_INIT
}

Keyboard::Key Window::translateSDLKey(int SDLKeyCode) {
#define KEY_COND(KEY, SDL_KEY)      \
    if (SDLKeyCode == SDL_KEY) {    \
        return Keyboard::Key::KEY;  \
    }
    KEYBOARD_KEYS(KEY_COND)
#undef KEY_COND

    return Keyboard::Key::Unknown;
}

void Window::initMouseStates() {
#define MOUSE_INIT(BUTTON, SDL_BUTTON) _mouseStates[Mouse::Button::BUTTON] = false;
    MOUSE_BUTTONS(MOUSE_INIT)
#undef MOUSE_INIT
}

Mouse::Button Window::translateSDLMouseButton(int SDLButtonCode) {
#define MOUSE_COND(BUTTON, SDL_BUTTON)  \
    if (SDLButtonCode == SDL_BUTTON) {  \
        return Mouse::Button::BUTTON;   \
    }
    MOUSE_BUTTONS(MOUSE_COND)
#undef MOUSE_COND

    return Mouse::Button::Unknown;
}

void Window::destroy() {
    if (_imguiInit) {
        ImGui_ImplSdlGL3_Shutdown();
    }

    if (_glContext != nullptr) {
        SDL_GL_DeleteContext(_glContext);
    }

    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
    }

    SDL_Quit();
}

void Window::sleep(float ms) {
    SDL_Delay(ms);
}

} // Namespace Window
