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

    explicit operator GLuint() const {
        return _texture;
    }

    void bind(GLenum unit = GL_TEXTURE0) const;
    void unBind() const;

    void updateData(void* data, GLenum type = 0) const;

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    void destroy();

private:
    Texture(GLuint texture,
        uint32_t width,
        uint32_t _height,
        GLenum type,
        GLint internalFormat,
        GLint format,
        GLint dataType
    );

private:
    GLuint _texture = 0;

    uint32_t _width = 0;
    uint32_t _height = 0;
    GLenum _type = GL_TEXTURE_2D;

    GLint _internalFormat = GL_RGBA;
    GLint _format = GL_RGBA;
    GLenum _dataType = GL_UNSIGNED_BYTE;
};

} // Namespace API
} // Namespace Graphics
