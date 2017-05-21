#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr

#include <Core/QuadTree.hpp> // Core::QuadTree
#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Core {

class SphereQuadTree {
public:
    SphereQuadTree(float radius, uint32_t maxRecurse = 4);
    ~SphereQuadTree() = default;

    SphereQuadTree(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree(SphereQuadTree&& quadTree) = delete;

    SphereQuadTree& operator=(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree&& operator=(SphereQuadTree&& quadTree) = delete;

    void update(std::vector<const Graphics::API::Buffer*>& buffers);

private:
    std::unique_ptr<QuadTree> _leftQuadTree = nullptr;
    std::unique_ptr<QuadTree> _rightQuadTree = nullptr;
    std::unique_ptr<QuadTree> _frontQuadTree = nullptr;
    std::unique_ptr<QuadTree> _backQuadTree = nullptr;
    std::unique_ptr<QuadTree> _topQuadTree = nullptr;
    std::unique_ptr<QuadTree> _bottomQuadTree = nullptr;
};

} // Core

