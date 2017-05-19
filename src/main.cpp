#include <Core/Application.hpp>

int main(int, char**){
    Core::Application app;

    if (!app.init()) {
        return 1;
    }

    return !!app.run();
}
