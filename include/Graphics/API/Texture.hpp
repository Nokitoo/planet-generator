#pragma once

#include <cstdint> // uint32_t

#include <GL/glew.h> // GLuint, GLenum

namespace Graphics {
namespace API {

namespace Builder {
    class Texture;
}

class Texture {
    friend Builder::Texture;
public:
    Texture() = default;
    ~Texture();

    Texture(const Texture& texture) = delete;
    Texture(Texture&& texture);

    Texture& operator=(const Texture& texture) = delete;
    Texture& operator=(Texture&& texture);

    void bind(GLenum unit = GL_TEXTURE0) const;
    void unBind() const;

private:
    GLuint _texture = 0;

    uint32_t _width = 0;
    uint32_t _height = 0;
    GLenum _type = GL_TEXTURE_2D;
};

} // Namespace API
} // Namespace Graphics
