#pragma once

#include <vector> // std::vector
#include <cstdint> // uint32_t

#include <Graphics/API/Buffer.hpp>

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

    bool build(API::Buffer& buffer);

    void addAttribute(const Attribute& attribute);
    void setVertices(const char* data, uint32_t size);
    void setIndices(const char* data, uint32_t size);

private:
    std::vector<Attribute> _attributes;

    char* _verticesData = nullptr;
    uint32_t _verticesSize = 0;

    char* _indicesData = nullptr;
    uint32_t _indicesSize = 0;
};

} // Builder
} // API
} // Graphics
