#include <iostream> // std::cerr

#include <Core/SphereQuadTree.hpp> // Graphics::Core::SphereQuadTree

namespace Core {

SphereQuadTree::SphereQuadTree(float size) {
    _leftQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, 0.0f, -size), // Position
        glm::vec3(0.0f, 0.0f, 1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(-1.0f, 0.0f, 0.0f) // Normal
        );
    _rightQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(size, 0.0f, 0.0f), // Position
        glm::vec3(0.0f, 0.0f, -1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(1.0f, 0.0f, 0.0f) // Normal
        );
    _frontQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, 1.0f) // Normal
        );
    _backQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(size, 0.0f, -size), // Position
        glm::vec3(-1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, -1.0f) // Normal
        );
    _topQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, size, 0.0f), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Height direction
        glm::vec3(0.0f, 1.0f, 0.0f) // Normal
        );
    _bottomQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, 0.0f, -size), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Height direction
        glm::vec3(0.0f, -1.0f, 0.0f) // Normal
        );

    initQuadTree(_leftQuadTree.get(), 4);
    initQuadTree(_rightQuadTree.get(), 4);
    initQuadTree(_frontQuadTree.get(), 4);
    initQuadTree(_backQuadTree.get(), 4);
    initQuadTree(_topQuadTree.get(), 4);
    initQuadTree(_bottomQuadTree.get(), 4);

    initBufferBuilder();
}

SphereQuadTree::SphereQuadTree(SphereQuadTree&& quadTree) {
    if (quadTree._leftQuadTree != nullptr) {
        _leftQuadTree = std::move(quadTree._leftQuadTree);
    }
    if (quadTree._rightQuadTree != nullptr) {
        _rightQuadTree = std::move(quadTree._rightQuadTree);
    }
    if (quadTree._frontQuadTree != nullptr) {
        _frontQuadTree = std::move(quadTree._frontQuadTree);
    }
    if (quadTree._backQuadTree != nullptr) {
        _backQuadTree = std::move(quadTree._backQuadTree);
    }
    if (quadTree._topQuadTree != nullptr) {
        _topQuadTree = std::move(quadTree._topQuadTree);
    }
    if (quadTree._bottomQuadTree != nullptr) {
        _bottomQuadTree = std::move(quadTree._bottomQuadTree);
    }

    initBufferBuilder();
    _buffer = std::move(quadTree._buffer);
}

SphereQuadTree& SphereQuadTree::operator=(SphereQuadTree&& quadTree) {
    if (quadTree._leftQuadTree != nullptr) {
        _leftQuadTree = std::move(quadTree._leftQuadTree);
    }
    if (quadTree._rightQuadTree != nullptr) {
        _rightQuadTree = std::move(quadTree._rightQuadTree);
    }
    if (quadTree._frontQuadTree != nullptr) {
        _frontQuadTree = std::move(quadTree._frontQuadTree);
    }
    if (quadTree._backQuadTree != nullptr) {
        _backQuadTree = std::move(quadTree._backQuadTree);
    }
    if (quadTree._topQuadTree != nullptr) {
        _topQuadTree = std::move(quadTree._topQuadTree);
    }
    if (quadTree._bottomQuadTree != nullptr) {
        _bottomQuadTree = std::move(quadTree._bottomQuadTree);
    }

    initBufferBuilder();
    _buffer = std::move(quadTree._buffer);

    return *this;
}

void SphereQuadTree::update() {
    std::vector<QuadTree::Vertex> vertices;
    std::vector<uint32_t> indices;

    _leftQuadTree->update(vertices, indices);
    _rightQuadTree->update(vertices, indices);
    _frontQuadTree->update(vertices, indices);
    _backQuadTree->update(vertices, indices);
    _topQuadTree->update(vertices, indices);
    _bottomQuadTree->update(vertices, indices);

    _bufferBuilder.setVertices(
        (char*)vertices.data(),
        static_cast<uint32_t>(vertices.size()) * sizeof(QuadTree::Vertex)
        );
    _bufferBuilder.setIndices(
        (char*)indices.data(),
        static_cast<uint32_t>(indices.size()) * sizeof(uint32_t)
        );

    Graphics::API::Buffer buffer;
    if (!_bufferBuilder.build(buffer)) {
        // TODO: replace this with logger
        std::cerr << "QuadTree::buildBuffer::init: failed to create renderer" << std::endl;
        return;
    }

    _buffer = std::move(buffer);
}

const Graphics::API::Buffer& SphereQuadTree::getBuffer() const {
    return _buffer;
}

void SphereQuadTree::initBufferBuilder() {
    // Position attribute
    _bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, pos)
    });
    // Color attribute
    _bufferBuilder.addAttribute({
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, color)
    });
    // Normal attribute
    _bufferBuilder.addAttribute({
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, normal)
    });
}

void SphereQuadTree::initQuadTree(QuadTree* quadTree, uint32_t maxRecurse) {
    if (!maxRecurse) {
        return;
    }

    quadTree->split();
    initQuadTree(quadTree->_children.topLeft.get(), maxRecurse - 1);
    initQuadTree(quadTree->_children.topRight.get(), maxRecurse - 1);
    initQuadTree(quadTree->_children.bottomLeft.get(), maxRecurse - 1);
    initQuadTree(quadTree->_children.bottomRight.get(), maxRecurse - 1);
}

} // Core

