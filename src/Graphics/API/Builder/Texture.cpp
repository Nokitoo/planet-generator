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

bool Texture::Image::getData(void*& data, GLsizei& width, GLsizei& height) {
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
    if (_texture->_dataType == GL_FLOAT) {
        _data = stbi_loadf(_fileName.c_str(), &_width, &_height, &compNb, WantedCompNb);
    }
    else if (_texture->_dataType == GL_UNSIGNED_BYTE) {
        _data = stbi_load(_fileName.c_str(), &_width, &_height, &compNb, WantedCompNb);
    }
    else {
        // TODO: replace this with logger
        std::cerr << "Texture::Image::getData: Can't load image from file \"" << _fileName << "\"";
        std::cerr << ": Data type not supported" << std::endl;
        return false;
    }

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

    GLuint glTexture = 0;
    glGenTextures(1, &glTexture);
    glBindTexture(_type, glTexture);

    GLsizei textureWidth = 0;
    GLsizei textureHeight = 0;

    // Generate texture images
    for (Texture::Image& image: _images) {
        GLenum type = image._type;
        if (type == 0) {
            type = _type;
        }

        void* data = nullptr;
        GLsizei width = 0;
        GLsizei height = 0;
        if (!image.getData(data, width, height)) {
            // TODO: replace this with logger
            std::cerr << "Texture::build: Can't get image data" << std::endl;
            return false;
        }

        // Check images have the same size
        if (!(!textureWidth && !textureHeight) &&
            (width != textureWidth || height != textureHeight)) {
            // TODO: replace this with logger
            std::cerr << "Texture::build: Images don't have the same size. ";
            std::cerr << "Image loaded with width " << textureWidth << " and height " << textureHeight;
            std::cerr << " but an other image has dimensions width " << width << " and height " << height << std::endl;
            return false;
        }

        glTexImage2D(type, 0, _internalFormat, width, height, 0, _format, _dataType, data);

        textureWidth = width;
        textureHeight = height;
    }

    // Set texture parameters
    for (auto& param: _parameters) {
        glTexParameteri(_type, param.first, param.second);
    }

    glBindTexture(_type, 0);

    texture = API::Texture(
        glTexture,
        textureWidth,
        textureHeight,
        _type,
        _internalFormat,
        _format,
        _dataType
    );

    return true;
}

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
