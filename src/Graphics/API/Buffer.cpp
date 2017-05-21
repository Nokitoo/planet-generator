#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Graphics {
namespace API {

Buffer::~Buffer() {
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

Buffer::Buffer(Buffer&& buffer) {
    _VAO = buffer._VAO;
    _VBO = buffer._VBO;
    _EBO = buffer._EBO;
    _verticesNb = buffer._verticesNb;
    _indicesNb = buffer._indicesNb;

    buffer._VAO = 0;
    buffer._VBO = 0;
    buffer._EBO = 0;
    buffer._verticesNb = 0;
    buffer._indicesNb = 0;
}


Buffer& Buffer::operator=(Buffer&& buffer) {
    _VAO = buffer._VAO;
    _VBO = buffer._VBO;
    _EBO = buffer._EBO;
    _verticesNb = buffer._verticesNb;
    _indicesNb = buffer._indicesNb;

    buffer._VAO = 0;
    buffer._VBO = 0;
    buffer._EBO = 0;
    buffer._verticesNb = 0;
    buffer._indicesNb = 0;

    return *this;
}

void Buffer::bind() const {
    glBindVertexArray(_VAO);
}

uint32_t Buffer::getVerticesNb() const {
    return _verticesNb;
}

uint32_t Buffer::getIndicesNb() const {
    return _indicesNb;
}

} // Namespace API
} // Namespace Graphics
