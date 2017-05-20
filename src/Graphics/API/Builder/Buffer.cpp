#include <iostream>

#include <Graphics/API/Builder/Buffer.hpp>

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
    glGenVertexArrays(1, &buffer._VAO);
    glGenBuffers(1, &buffer._VBO);
    glGenBuffers(1, &buffer._EBO);

    buffer.bind();

    // Update vertices buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer._VBO);
    glBufferData(GL_ARRAY_BUFFER, _verticesSize, _verticesData, GL_STATIC_DRAW);

    // Update indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesSize, _indicesData, GL_STATIC_DRAW);

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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

void Buffer::addAttribute(const Attribute& attribute) {
    _attributes.push_back(attribute);
}

void Buffer::setVertices(const char* data, uint32_t size) {
    _verticesSize = size;
    _verticesData = new char[_verticesSize];

    std::memcpy(_verticesData, data, _verticesSize);
}

void Buffer::setIndices(const char* data, uint32_t size) {
    _indicesSize = size;
    _indicesData = new char[_indicesSize];

    std::memcpy(_indicesData, data, _indicesSize);
}


} // Builder
} // API
} // Graphics
