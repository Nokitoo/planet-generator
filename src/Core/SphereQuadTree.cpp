#include <Core/SphereQuadTree.hpp> // Graphics::Core::SphereQuadTree

namespace Core {

SphereQuadTree::SphereQuadTree(float size, uint32_t maxRecurse) {
    _leftQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, 0.0f, -size), // Position
        glm::vec3(0.0f, 0.0f, 1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(-1.0f, 0.0f, 0.0f), // Normal
        maxRecurse // recurse
        );
    _rightQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(size, 0.0f, 0.0f), // Position
        glm::vec3(0.0f, 0.0f, -1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(1.0f, 0.0f, 0.0f), // Normal
        maxRecurse // recurse
        );
    _frontQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Normal
        maxRecurse // recurse
        );
    _backQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(size, 0.0f, -size), // Position
        glm::vec3(-1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Normal
        maxRecurse // recurse
        );
    _topQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, size, 0.0f), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Height direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Normal
        maxRecurse // recurse
        );
    _bottomQuadTree = std::make_unique<Core::QuadTree>(
        size, // Width
        glm::vec3(0.0f, 0.0f, -size), // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Height direction
        glm::vec3(0.0f, -1.0f, 0.0f), // Normal
        maxRecurse // recurse
        );
}

void SphereQuadTree::update(std::vector<const Graphics::API::Buffer*>& buffers) {
    _leftQuadTree->update(buffers);
    _rightQuadTree->update(buffers);
    _frontQuadTree->update(buffers);
    _backQuadTree->update(buffers);
    _topQuadTree->update(buffers);
    _bottomQuadTree->update(buffers);
}

} // Core

