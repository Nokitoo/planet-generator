#pragma once

#define MOUSE_BUTTONS(MACRO)            \
    MACRO(Left, SDL_BUTTON_LEFT)        \
    MACRO(Middle, SDL_BUTTON_MIDDLE)    \
    MACRO(Right, SDL_BUTTON_RIGHT)      \
    MACRO(X1, SDL_BUTTON_X1)            \
    MACRO(X2, SDL_BUTTON_X2)            \

#define MOUSE_ENUM(BUTTON, SDL_BUTTON) BUTTON,

namespace Window {
namespace Mouse {

enum class Button : int {
    Unknown,
    MOUSE_BUTTONS(MOUSE_ENUM)
};

} // Namespace Mouse
} // Namespace Window
