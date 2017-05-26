#include <iostream> // std::cerr

// Define STB_IMAGE_IMPLEMENTATION before stb_image.h to create the implementation
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Graphics/API/Builder/Texture.hpp> // Graphics::API::Builder::Texture

namespace Graphics {
namespace API {
namespace Builder {

Texture::Image::Image(GLenum type): _type(type) {}

Texture::Image::~Image() {
    if (_data != nullptr) {
        STBI_FREE(_data);
    }
}

bool Texture::Image::getData(unsigned char*& data, GLsizei& width, GLsizei& height) {
    data = nullptr;
    width = _texture->_width;
    height = _texture->_height;

    // Already loaded, just return it
    if (_data != nullptr) {
        data = _data;
        return true;
    }
    // Don't need to load data
    else if (_fileName.size() == 0) {
        return true;
    }

    // Load image data
    // Set number of components of the data depending of the format
    int WantedCompNb = 0;
    switch (_texture->_format) {
        case GL_RGBA:
            WantedCompNb = 4;
            break;
        case GL_RGB:
            WantedCompNb = 3;
            break;
        default:
            // TODO: replace this with logger
            std::cerr << "Texture::Image::getData: Format not supported for loading image data" << std::endl;
            return false;
    }

    // TODO: Use resources manager
    int compNb = 0;
    _data = stbi_load(_fileName.c_str(), &_width, &_height, &compNb, WantedCompNb);

    if (_data == nullptr) {
        // TODO: replace this with logger
        std::cerr << "Texture::Image::getData: Failed to load texture \"" << _fileName << "\"" << std::endl;
        return false;
    }
    else if (compNb != WantedCompNb) {
        // TODO: replace this with logger
        std::cerr << "Texture::Image::getData: Loaded image format do not correspond to given format for texture \"" << _fileName << "\"" << std::endl;
    }

    width = _width;
    height = _height;
    data = _data;

    return true;
}

bool Texture::build(API::Texture& texture) {
    if (_images.size() == 0) {
        // TODO: replace this with logger
        std::cerr << "Texture::build: Need at least one image" << std::endl;
        return false;
    }

    texture._type = _type;

    glGenTextures(1, &texture._texture);
    texture.bind();

    // Generate texture images
    for (Texture::Image& image: _images) {
        GLenum type = image._type;
        if (type == 0) {
            type = _type;
        }

        unsigned char* data = nullptr;
        GLsizei width = 0;
        GLsizei height = 0;
        if (!image.getData(data, width, height)) {
            // TODO: replace this with logger
            std::cerr << "Texture::build: Can't get image data" << std::endl;
            return false;
        }

        glTexImage2D(type, 0, _internalFormat, width, height, 0, _format, _dataType, data);
    }

    //glGenerateMipmap(_type);
    // Set texture parameters
    for (auto& param: _parameters) {
        glTexParameteri(_type, param.first, param.second);
    }

    texture._width = _width;
    texture._height = _height;

    texture.unBind();

    return true;
}

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
