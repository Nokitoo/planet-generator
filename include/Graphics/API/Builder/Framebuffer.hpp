#pragma once

#include <vector> // std::vector

#include <GL/glew.h> // GLsizei, GLenum, GLsizei

#include <Graphics/API/Framebuffer.hpp> // Graphics::API::Framebuffer

namespace Graphics {
namespace API {
namespace Builder {

class Framebuffer {
private:
    struct DepthBuffer {
        GLenum internalformat = 0;
        GLsizei width = 0;
        GLsizei height = 0;
    };

    struct ColorAttachment {
        const API::Texture* texture;
        GLenum textureType;
    };

public:
    Framebuffer() = default;
    ~Framebuffer() = default;

    Framebuffer(const Framebuffer& texture) = delete;
    Framebuffer(Framebuffer&& texture) = delete;

    Framebuffer& operator=(const Framebuffer& texture) = delete;
    Framebuffer& operator=(Framebuffer&& texture) = delete;

    bool build(API::Framebuffer& texture);

    void addColorAttachment(const API::Texture* texture, GLenum textureType = GL_TEXTURE_2D);
    void setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height);

private:
    std::vector<ColorAttachment> _colorAttachments;

    DepthBuffer _depthBuffer;
};

#include <Graphics/API/Builder/Framebuffer.inl>

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
