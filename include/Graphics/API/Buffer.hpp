#pragma once

#include <cstdint> // uint32_t

#include <GL/glew.h> // GLuint

namespace Graphics {
namespace API {

namespace Builder {
    class Buffer;
}

class Buffer {
    friend Builder::Buffer;

public:
    Buffer() = default;
    ~Buffer();

    Buffer(const Buffer& buffer) = delete;
    Buffer(Buffer&& buffer);

    Buffer& operator=(const Buffer& buffer) = delete;
    Buffer& operator=(Buffer&& buffer);

    void bind() const;

    uint32_t getVerticesNb() const;
    uint32_t getIndicesNb() const;

    void updateVertices(char* data, uint32_t size, uint32_t verticesNb, GLenum usage);
    void updateIndices(char* data, uint32_t size, uint32_t indicesNb, GLenum usage);

    void destroy();

private:
    Buffer(
        GLuint VAO,
        GLuint VBO,
        GLuint EBO,
        uint32_t verticesSize,
        uint32_t verticesNb,
        uint32_t indicesSize,
        uint32_t indicesNb
    );

private:
    // Vertex array buffer
    GLuint _VAO = 0;

    // Vertex buffer
    GLuint _VBO = 0;

    // Index buffer
    GLuint _EBO = 0;

    uint32_t _verticesSize = 0;
    uint32_t _verticesNb = 0;

    uint32_t _indicesSize = 0;
    uint32_t _indicesNb = 0;
};

} // Namespace API
} // Namespace Graphics
