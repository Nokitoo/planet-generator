#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Graphics {
namespace API {

Texture::~Texture() {
    if (_texture) {
        glDeleteTextures(1, &_texture);
    }
}

Texture::Texture(Texture&& texture) {
    _texture = texture._texture;
    _width = texture._width;
    _height = texture._height;
    _type = texture._type;

    texture._texture = 0;
    texture._width = 0;
    texture._height = 0;
}

Texture& Texture::operator=(Texture&& texture) {
    _texture = texture._texture;
    _width = texture._width;
    _height = texture._height;
    _type = texture._type;

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
    glBindTexture(_type, 0);
}

} // Namespace API
} // Namespace Graphics
