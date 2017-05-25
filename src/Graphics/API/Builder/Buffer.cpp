#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer

namespace Graphics {
namespace API {
namespace Builder {

Buffer::~Buffer() {
    if (_verticesData != nullptr) {
        delete[] _verticesData;
    }

    if (_indicesData != nullptr) {
        delete[] _indicesData;
    }
}

bool Buffer::build(API::Buffer& returnBuffer) {
    API::Buffer buffer;

    glGenVertexArrays(1, &buffer._VAO);
    glGenBuffers(1, &buffer._VBO);
    glGenBuffers(1, &buffer._EBO);

    buffer.bind();

    // Update vertices buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer._VBO);
    buffer.updateVertices(_verticesData, _verticesSize, _verticesNb, _verticesUsage);

    // Update indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._EBO);
    buffer.updateIndices(_indicesData, _indicesSize, _indicesNb, _indicesUsage);

    // Configure buffer attribute
    for (const auto& attribute: _attributes) {
        glVertexAttribPointer(
            attribute.location,
            attribute.componentsNb,
            attribute.componentType,
            attribute.normalized,
            attribute.stride,
            (GLvoid*)attribute.offset);

        glEnableVertexAttribArray(attribute.location);
    }

    glBindVertexArray(0);

    returnBuffer = std::move(buffer);

    return true;
}

void Buffer::addAttribute(const Attribute& attribute) {
    _attributes.push_back(attribute);
}

void Buffer::setVertices(const char* data, uint32_t size, uint32_t verticesNb) {
    if (_verticesData) {
        delete[] _verticesData;
    }

    _verticesSize = size;
    _verticesNb = verticesNb;
    _verticesData = new char[_verticesSize];

    std::memcpy(_verticesData, data, _verticesSize);
}

void Buffer::setIndices(const char* data, uint32_t size, uint32_t indicesNb) {
    if (_indicesData) {
        delete[] _indicesData;
    }

    _indicesSize = size;
    _indicesNb = indicesNb;
    _indicesData = new char[_indicesSize];

    std::memcpy(_indicesData, data, _indicesSize);
}

void Buffer::setVerticesUsage(GLenum usage) {
    _verticesUsage = usage;
}

void Buffer::setIndicesUsage(GLenum usage) {
    _indicesUsage = usage;
}

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
