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

bool Buffer::build(API::Buffer& buffer) {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Update vertices buffer
    glBufferData(GL_ARRAY_BUFFER, _verticesSize, _verticesData, _verticesUsage);

    // Update indices buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesSize, _indicesData, _indicesUsage);

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

    buffer = API::Buffer(
        VAO,
        VBO,
        EBO,
        _verticesSize,
        _verticesNb,
        _indicesSize,
        _indicesNb
    );

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
