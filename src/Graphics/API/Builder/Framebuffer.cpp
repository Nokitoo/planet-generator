#include <Graphics/API/Builder/Framebuffer.hpp> // Graphics::API::Builder::Framebuffer

namespace Graphics {
namespace API {
namespace Builder {

bool Framebuffer::build(API::Framebuffer& framebuffer) {
    GLuint glFramebuffer = 0;
    glGenFramebuffers(1, &glFramebuffer);

    framebuffer = API::Framebuffer(glFramebuffer);

    framebuffer.bind();

    for (const auto& colorAttachment: _colorAttachments) {
        framebuffer.addColorAttachment(colorAttachment.texture, colorAttachment.textureType);
    }

    if (_depthBuffer.width != 0 &&
        _depthBuffer.height != 0) {
        framebuffer.setDepthAttachment(_depthBuffer.internalformat, _depthBuffer.width, _depthBuffer.height);
    }

    framebuffer.unBind();

    return true;
}

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
