#include <Graphics/API/Framebuffer.hpp> // Graphics::API::Framebuffer

namespace Graphics {
namespace API {

Framebuffer::Framebuffer(GLuint fbo): _fbo(fbo) {}

Framebuffer::~Framebuffer() {
    destroy();
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) {
    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = std::move(framebuffer._colorAttachmentsIds);
    _depthBuffer = framebuffer._depthBuffer;

    framebuffer._fbo = 0;
    framebuffer._depthBuffer = 0;
    framebuffer._colorAttachments.clear();
    framebuffer._colorAttachmentsIds.clear();
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer) {
    destroy();

    _fbo = framebuffer._fbo;
    _colorAttachments = std::move(framebuffer._colorAttachments);
    _colorAttachmentsIds = std::move(framebuffer._colorAttachmentsIds);
    _depthBuffer = framebuffer._depthBuffer;

    framebuffer._fbo = 0;
    framebuffer._depthBuffer = 0;
    framebuffer._colorAttachments.clear();
    framebuffer._colorAttachmentsIds.clear();

    return (*this);
}

void Framebuffer::bind(GLenum target) const {
    glBindFramebuffer(target, _fbo);
}

void Framebuffer::unBind(GLenum target) const {
    glBindFramebuffer(target, 0);
}

void Framebuffer::use(GLenum target) const {
    bind(target);

    // Specify the color attachments to be used in fragment shader
    if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER) {
        glDrawBuffers((GLsizei)_colorAttachmentsIds.size(), _colorAttachmentsIds.data());
    }
}

void Framebuffer::removeColorAttachments() {
    _colorAttachmentsIds.clear();
    _colorAttachments.clear();
}

void Framebuffer::addColorAttachment(const Texture* texture, GLenum textureType) {
    GLuint colorAttachmentId = GL_COLOR_ATTACHMENT0 + (GLuint)_colorAttachments.size();

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        colorAttachmentId,
        textureType,
        static_cast<GLuint>(*texture),
        0
    );

    _colorAttachmentsIds.push_back(colorAttachmentId);
    _colorAttachments.push_back(texture);
}

void Framebuffer::setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height) {
    if (!_depthBuffer) {
        glGenRenderbuffers(1, &_depthBuffer);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
}

bool Framebuffer::isComplete() const {
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

const std::vector<const Texture*>& Framebuffer::getColorAttachments() const {
    return (_colorAttachments);
}

void Framebuffer::destroy() {
    if (_depthBuffer) {
        glDeleteRenderbuffers(1, &_depthBuffer);
        _depthBuffer = 0;
    }

    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = 0;
    }

    _colorAttachments.clear();
    _colorAttachmentsIds.clear();
}

} // Namespace API
} // Namespace Graphics
