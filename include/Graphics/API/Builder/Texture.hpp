#pragma once

#include <string> // std::string
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

#include <GL/glew.h> // GLint, GLenum, GLsizei

#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Graphics {
namespace API {
namespace Builder {

class Texture {
public:
    class Image {
        friend Texture;

    public:
        Image(GLenum type = 0);
        ~Image();

        void setType(GLenum type);
        void setFileName(const std::string& fileName);

    private:
        bool getData(void*& data, GLsizei& width, GLsizei& height);

    private:
        GLenum _type = 0; // 0 means use same as texture type
        std::string _fileName; // Optionally load texture from a file

        // Used internally
        void* _data = nullptr;
        int _width = 0;
        int _height = 0;
        Texture* _texture = nullptr;
    };

    friend Image;

public:
    Texture() = default;
    ~Texture() = default;

    Texture(const Texture& texture) = delete;
    Texture(Texture&& texture) = delete;

    Texture& operator=(const Texture& texture) = delete;
    Texture& operator=(Texture&& texture) = delete;

    bool build(API::Texture& texture);

    void setType(GLenum type);
    void setWidth(GLsizei width);
    void setHeight(GLsizei height);
    void setInternalFormat(GLint internalFormat);
    void setFormat(GLint format);
    void setDataType(GLenum dataType);
    void setParameter(GLenum paramName, GLint paramValue);

    // Don't keep the pointer after 2nd call to addImage
    template<typename... Args>
    Image* addImage(Args... args);

private:
    GLenum _type = GL_TEXTURE_2D;
    GLsizei _width = 0;
    GLsizei _height = 0;

    GLint _internalFormat = GL_RGBA;
    GLint _format = GL_RGBA;
    GLenum _dataType = GL_UNSIGNED_BYTE;

    std::vector<Image> _images;
    std::unordered_map<GLenum, GLint> _parameters;
};

#include <Graphics/API/Builder/Texture.inl>

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
