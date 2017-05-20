#include <cstdint> // uint32_t

#include <Window/Keyboard.hpp>
#include <Window/Mouse.hpp>

namespace Window {

    struct Event {
        // TODO: add other events
        enum class Type : uint32_t {
            Close,
            KeyPressed,
            KeyReleased,
            MouseMoved,
            MousePressed,
            MouseReleased,
            MouseScroll
        };

        struct Key {
            Keyboard::Key code;
        };

        struct Mouse {
            Window::Mouse::Button button;

            // Don't use glm::vec2 otheriwe the compiler can't find a default constructor of Window::Event
            struct {
                int32_t x;
                int32_t y;
            } scrollOffset;

            struct {
                int32_t x;
                int32_t y;
            } moveOffset;

            struct {
                int32_t x;
                int32_t y;
            } pos;
        };

        Type type;

        union {
            Key key;
            Event::Mouse mouse;
        };
    };

}

