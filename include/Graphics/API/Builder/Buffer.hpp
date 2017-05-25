#pragma once

#include <vector> // std::vector
#include <cstdint> // uint32_t

#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Graphics {
namespace API {
namespace Builder {

class Buffer {
private:
    struct Attribute {
        GLuint location;
        GLint componentsNb;
        GLenum componentType;
        GLboolean normalized;
        GLsizei stride;
        uint32_t offset;
    };

public:
    Buffer() = default;
    ~Buffer();

    Buffer(const Buffer& buffer) = delete;
    Buffer(Buffer&& buffer) = delete;

    Buffer& operator=(const Buffer& buffer) = delete;
    Buffer& operator=(Buffer&& buffer) = delete;

    bool build(API::Buffer& returnBuffer);

    void addAttribute(const Attribute& attribute);
    void setVertices(const char* data, uint32_t size, uint32_t verticesNb);
    void setIndices(const char* data, uint32_t size, uint32_t indicesNb);
    void setVerticesUsage(GLenum usage);
    void setIndicesUsage(GLenum usage);

private:
    std::vector<Attribute> _attributes;

    uint32_t _verticesSize = 0;
    uint32_t _verticesNb = 0;
    char* _verticesData = nullptr;

    uint32_t _indicesSize = 0;
    uint32_t _indicesNb = 0;
    char* _indicesData = nullptr;

    GLenum _verticesUsage = GL_STATIC_DRAW;
    GLenum _indicesUsage = GL_STATIC_DRAW;
};

} // Namespace Builder
} // Namespace API
} // Namespace Graphics
