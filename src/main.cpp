#include <Core/Application.hpp> // Core::Application

int main(int, char**){
    Core::Application app;

    if (!app.init()) {
        return 1;
    }

    return !!app.run();
}
