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
}

void SphereQuadTree::update(std::vector<const Graphics::API::Buffer*>& buffers) {
    _leftQuadTree->update(buffers);
    _rightQuadTree->update(buffers);
    _frontQuadTree->update(buffers);
    _backQuadTree->update(buffers);
    _topQuadTree->update(buffers);
    _bottomQuadTree->update(buffers);
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

