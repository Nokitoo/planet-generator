#pragma once

#include <vector> // std::vector

#include <GL/glew.h> // GLuint, GLenum

#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Graphics {
namespace API {

namespace Builder {
    class Framebuffer;
}

class Framebuffer {
    friend Builder::Framebuffer;

public:
    Framebuffer() = default;
    ~Framebuffer();

    Framebuffer(Framebuffer&& framebuffer);
    Framebuffer& operator=(Framebuffer&& framebuffer);

    void bind(GLenum target = GL_FRAMEBUFFER) const;
    void unBind(GLenum target = GL_FRAMEBUFFER) const;
    void use(GLenum target = GL_FRAMEBUFFER) const;

    void removeColorAttachments();

    void addColorAttachment(const Texture* texture, GLenum textureType = GL_TEXTURE_2D);
    void setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height);

    bool isComplete() const;

    const std::vector<const Texture*>& getColorAttachments() const;

    void destroy();

private:
    GLuint _fbo;

    GLuint _depthBuffer = 0;

    std::vector<const Texture*> _colorAttachments;
    std::vector<GLuint> _colorAttachmentsIds;
};

} // Namespace API
} // Namespace Graphics
