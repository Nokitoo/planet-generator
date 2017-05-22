#include <iostream> // std::cerr

#include <System/Vector.hpp> // System::Vector

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

void SphereQuadTree::update(const Graphics::Camera& camera, uint32_t level) {
    System::Vector<QuadTree::Vertex> vertices(500);
    System::Vector<uint32_t> indices(500);

    if (isFacingCamera(_leftQuadTree.get(), camera)) {
        _leftQuadTree->update(vertices, indices, level);
    }
    if (isFacingCamera(_rightQuadTree.get(), camera)) {
        _rightQuadTree->update(vertices, indices, level);
    }
    if (isFacingCamera(_frontQuadTree.get(), camera)) {
        _frontQuadTree->update(vertices, indices, level);
    }
    if (isFacingCamera(_backQuadTree.get(), camera)) {
        _backQuadTree->update(vertices, indices, level);
    }
    if (isFacingCamera(_topQuadTree.get(), camera)) {
        _topQuadTree->update(vertices, indices, level);
    }
    if (isFacingCamera(_bottomQuadTree.get(), camera)) {
        _bottomQuadTree->update(vertices, indices, level);
    }

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

bool SphereQuadTree::isFacingCamera(const QuadTree* quadTree, const Graphics::Camera& camera) {
    glm::vec3 cameraDir = glm::normalize(quadTree->_center - camera.getPos());
    return glm::dot(-cameraDir, quadTree->_normal) > -0.7f;
}

} // Namespace Core

