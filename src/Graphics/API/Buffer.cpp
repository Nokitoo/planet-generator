#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Graphics {
namespace API {

Buffer::~Buffer() {
    destroy();
}

Buffer::Buffer(Buffer&& buffer) {
    _VAO = buffer._VAO;
    _VBO = buffer._VBO;
    _EBO = buffer._EBO;
    _verticesSize = buffer._verticesSize;
    _verticesNb = buffer._verticesNb;
    _indicesSize = buffer._indicesSize;
    _indicesNb = buffer._indicesNb;

    buffer._VAO = 0;
    buffer._VBO = 0;
    buffer._EBO = 0;
    buffer._verticesSize = 0;
    buffer._verticesNb = 0;
    buffer._indicesSize = 0;
    buffer._indicesNb = 0;
}


Buffer& Buffer::operator=(Buffer&& buffer) {
    destroy();

    _VAO = buffer._VAO;
    _VBO = buffer._VBO;
    _EBO = buffer._EBO;
    _verticesSize = buffer._verticesSize;
    _verticesNb = buffer._verticesNb;
    _indicesSize = buffer._indicesSize;
    _indicesNb = buffer._indicesNb;

    buffer._VAO = 0;
    buffer._VBO = 0;
    buffer._EBO = 0;
    buffer._verticesSize = 0;
    buffer._verticesNb = 0;
    buffer._indicesSize = 0;
    buffer._indicesNb = 0;

    return *this;
}

void Buffer::bind() const {
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
}

uint32_t Buffer::getVerticesNb() const {
    return _verticesNb;
}

uint32_t Buffer::getIndicesNb() const {
    return _indicesNb;
}

void Buffer::updateVertices(char* data, uint32_t size, uint32_t verticesNb, GLenum usage) {
    bind();

    if (size > _verticesSize) {
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    _verticesSize = size;
    _verticesNb = verticesNb;
}

void Buffer::updateIndices(char* data, uint32_t size, uint32_t indicesNb, GLenum usage) {
    bind();

    if (size > _indicesSize) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    }
    else {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }

    _indicesSize = size;
    _indicesNb = indicesNb;
}

void Buffer::destroy() {
    if (_VAO) {
        glDeleteVertexArrays(1, &_VAO);
    }

    if (_VBO) {
        glDeleteBuffers(1, &_VBO);
    }

    if (_EBO) {
        glDeleteBuffers(1, &_EBO);
    }
}

} // Namespace API
} // Namespace Graphics
