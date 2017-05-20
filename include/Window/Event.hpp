#include <Window/Keyboard.hpp>

namespace Window {

    struct Event {
        // TODO: add other events
        enum class Type : uint32_t {
            Close,
            KeyPressed,
            KeyReleased
        };

        Type type;

        Keyboard::Key keyCode;
    };

}

