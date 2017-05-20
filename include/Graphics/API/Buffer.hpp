#pragma once

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

private:
    // Vertex array buffer
    GLuint                                  _VAO = 0;

    // Vertex buffer
    GLuint                                  _VBO = 0;

    // Index buffer
    GLuint                                  _EBO = 0;
};

} // Namespace API
} // Namespace Graphics
