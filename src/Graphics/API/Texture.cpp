#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Graphics {
namespace API {

Texture::Texture(
    GLuint texture,
    uint32_t width,
    uint32_t _height,
    GLenum type,
    GLint internalFormat,
    GLint format,
    GLint dataType
): _texture(texture), _width(width), _height(_height), _type(type), _internalFormat(internalFormat), _format(format), _dataType(dataType) {}

Texture::~Texture() {
    destroy();
}

Texture::Texture(Texture&& texture) {
    _texture = texture._texture;
    _width = texture._width;
    _height = texture._height;
    _type = texture._type;
    _internalFormat = texture._internalFormat;
    _format = texture._format;
    _dataType = texture._dataType;

    texture._texture = 0;
    texture._width = 0;
    texture._height = 0;
}

Texture& Texture::operator=(Texture&& texture) {
    destroy();

    _texture = texture._texture;
    _width = texture._width;
    _height = texture._height;
    _type = texture._type;
    _internalFormat = texture._internalFormat;
    _format = texture._format;
    _dataType = texture._dataType;

    texture._texture = 0;
    texture._width = 0;
    texture._height = 0;

    return *this;
}

void Texture::bind(GLenum unit) const {
    glActiveTexture(unit);
    glBindTexture(_type, _texture);
}

void Texture::unBind() const {
        (_type, 0);
}

void Texture::updateData(void* data, GLenum type) const {
    bind();

    if (!type) {
        type = _type;
    }

    glTexImage2D(type, 0, _internalFormat, _width, _height, 0, _format, _dataType, data);
}

uint32_t Texture::getWidth() const {
    return _width;
}

uint32_t Texture::getHeight() const {
    return _height;
}

void Texture::destroy() {
    if (_texture) {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }
}

} // Namespace API
} // Namespace Graphics
