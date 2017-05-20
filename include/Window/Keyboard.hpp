#pragma once

#define KEYBOARD_KEYS(MACRO)        \
    MACRO(A, SDLK_a)                \
    MACRO(B, SDLK_b)                \
    MACRO(C, SDLK_c)                \
    MACRO(D, SDLK_d)                \
    MACRO(E, SDLK_e)                \
    MACRO(F, SDLK_f)                \
    MACRO(G, SDLK_g)                \
    MACRO(H, SDLK_h)                \
    MACRO(I, SDLK_i)                \
    MACRO(J, SDLK_j)                \
    MACRO(K, SDLK_k)                \
    MACRO(L, SDLK_l)                \
    MACRO(M, SDLK_m)                \
    MACRO(N, SDLK_n)                \
    MACRO(O, SDLK_o)                \
    MACRO(P, SDLK_p)                \
    MACRO(Q, SDLK_q)                \
    MACRO(R, SDLK_r)                \
    MACRO(S, SDLK_s)                \
    MACRO(T, SDLK_t)                \
    MACRO(U, SDLK_u)                \
    MACRO(V, SDLK_v)                \
    MACRO(W, SDLK_w)                \
    MACRO(X, SDLK_x)                \
    MACRO(Y, SDLK_y)                \
    MACRO(Z, SDLK_z)                \
                                    \
    MACRO(Num0, SDLK_0)             \
    MACRO(Num1, SDLK_1)             \
    MACRO(Num2, SDLK_2)             \
    MACRO(Num3, SDLK_3)             \
    MACRO(Num4, SDLK_4)             \
    MACRO(Num5, SDLK_5)             \
    MACRO(Num6, SDLK_6)             \
    MACRO(Num7, SDLK_7)             \
    MACRO(Num8, SDLK_8)             \
    MACRO(Num9, SDLK_9)             \
                                    \
    MACRO(LControl, SDLK_LCTRL)     \
    MACRO(LShift, SDLK_LSHIFT)      \
    MACRO(LAlt, SDLK_LALT)          \
    MACRO(LSystem, SDLK_LGUI)       \
    MACRO(RControl, SDLK_RCTRL)     \
    MACRO(RShift, SDLK_RSHIFT)      \
    MACRO(RAlt, SDLK_RALT)          \
    MACRO(RSystem, SDLK_RGUI)

#define KEYBOARD_ENUM(KEY, SDL_KEY) KEY,

namespace Window {
namespace Keyboard {

enum class Key : int {
    Unknown,
    KEYBOARD_KEYS(KEYBOARD_ENUM)
};

} // Keyboard
} // Window
