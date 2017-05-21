#include <iostream> // std::cerr

#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer

#include <Core/QuadTree.hpp> // Graphics::Core::QuadTree

namespace Core {

QuadTree::QuadTree(float size,
                    const glm::vec3& pos,
                    const glm::vec3& widthDir,
                    const glm::vec3& heightDir,
                    const glm::vec3& normal,
                    uint32_t maxRecurse) {
    if (!maxRecurse) {
        buildBuffer(size, pos, widthDir, heightDir, normal);
        return;
    }

    float childrenSize = size / 2;
    _children._topLeft = std::make_unique<QuadTree>(
        childrenSize,
        pos + (heightDir * childrenSize),
        widthDir,
        heightDir,
        normal,
        maxRecurse - 1
        );
    _children._topRight = std::make_unique<QuadTree>(
        childrenSize,
        pos + (widthDir * childrenSize) + (heightDir * childrenSize),
        widthDir,
        heightDir,
        normal,
        maxRecurse - 1
        );
    _children._bottomLeft = std::make_unique<QuadTree>(
        childrenSize,
        pos,
        widthDir,
        heightDir,
        normal,
        maxRecurse - 1
        );
    _children._bottomRight = std::make_unique<QuadTree>(
        childrenSize,
        pos + (widthDir * childrenSize),
        widthDir,
        heightDir,
        normal,
        maxRecurse - 1
        );
}

void QuadTree::update(std::vector<const Graphics::API::Buffer*>& buffers) {
    buffers.push_back(&_buffer);

    if (_children._topLeft) {
        _children._topLeft->update(buffers);
    }
    if (_children._topRight) {
        _children._topRight->update(buffers);
    }
    if (_children._bottomLeft) {
        _children._bottomLeft->update(buffers);
    }
    if (_children._bottomRight) {
        _children._bottomRight->update(buffers);
    }
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

