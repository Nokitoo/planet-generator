#include <iostream> // std::cerr

#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer

#include <Core/QuadTree.hpp> // Graphics::Core::QuadTree

namespace Core {

QuadTree::QuadTree(float size,
                    const glm::vec3& pos,
                    const glm::vec3& widthDir,
                    const glm::vec3& heightDir,
                    const glm::vec3& normal): _size(size), _pos(pos), _widthDir(widthDir), _heightDir(heightDir), _normal(normal) {
    buildBuffer(size, pos, widthDir, heightDir, normal);
}

void QuadTree::update(std::vector<const Graphics::API::Buffer*>& buffers) {
    buffers.push_back(&_buffer);

    if (_children.topLeft) {
        _children.topLeft->update(buffers);
    }
    if (_children.topRight) {
        _children.topRight->update(buffers);
    }
    if (_children.bottomLeft) {
        _children.bottomLeft->update(buffers);
    }
    if (_children.bottomRight) {
        _children.bottomRight->update(buffers);
    }
}

void QuadTree::split() {
    float childrenSize = _size / 2;
    _children.topLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );
    _children.topRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize) + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );
    _children.bottomLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos,
        _widthDir,
        _heightDir,
        _normal
        );
    _children.bottomRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );
}

bool QuadTree::buildBuffer(float size,
    const glm::vec3& pos,
    const glm::vec3& widthDir,
    const glm::vec3& heightDir,
    const glm::vec3& normal) {
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
    };

    glm::vec3 topLeft = pos + (heightDir * size);
    glm::vec3 topRight = pos + (widthDir * size) + (heightDir * size);
    glm::vec3 bottomLeft = pos;
    glm::vec3 bottomRight = pos + (widthDir * size);

    Vertex vertices[] = {
        { topLeft, glm::vec3(1.0f, 0.0f, 0.0f), normal },
        { topRight, glm::vec3(0.0f, 1.0f, 0.0f), normal },
        { bottomLeft, glm::vec3(0.0f, 0.0f, 1.0f), normal },
        { bottomRight, glm::vec3(1.0f, 1.0f, 0.0f), normal }
    };

    uint32_t indices[] = {
        0, 2, 3,
        3, 1, 0,
    };

    Graphics::API::Builder::Buffer bufferBuilder;
    // Position attribute
    bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, pos)
    });
    // Color attribute
    bufferBuilder.addAttribute({
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, color)
    });
    // Normal attribute
    bufferBuilder.addAttribute({
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        offsetof(Vertex, normal)
    });
    bufferBuilder.setVertices((char*)vertices, sizeof(vertices));
    bufferBuilder.setIndices((char*)indices, sizeof(indices));

    if (!bufferBuilder.build(_buffer)) {
        // TODO: replace this with logger
        std::cerr << "QuadTree::buildBuffer::init: failed to create renderer" << std::endl;
        return false;
    }

    return true;
}

} // Core

