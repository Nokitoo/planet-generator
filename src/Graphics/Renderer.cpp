#include <Graphics/Renderer.hpp>

namespace Graphics {

std::unique_ptr<Renderer> Renderer::create() {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<Renderer> renderer(new Renderer());

    if (!renderer->init()) {
        return nullptr;
    }

    return renderer;
}

bool Renderer::init() {
    return true;
}

} // Graphics
