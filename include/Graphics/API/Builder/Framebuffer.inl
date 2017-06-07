inline void Framebuffer::addColorAttachment(const API::Texture* texture, GLenum textureType) {
    _colorAttachments.push_back({texture, textureType});
}

inline void Framebuffer::setDepthAttachment(GLenum internalformat, GLsizei width, GLsizei height) {
    _depthBuffer.internalformat = internalformat;
    _depthBuffer.width = width;
    _depthBuffer.height = height;
}
