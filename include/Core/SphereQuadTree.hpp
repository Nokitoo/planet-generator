#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr

#include <Core/QuadTree.hpp> // Core::QuadTree
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer
#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer

namespace Core {

class SphereQuadTree {
public:
    SphereQuadTree(float size);
    ~SphereQuadTree() = default;

    SphereQuadTree(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree(SphereQuadTree&& quadTree);

    SphereQuadTree& operator=(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree& operator=(SphereQuadTree&& quadTree);

    void update(const Graphics::Camera& camera, uint32_t level);

    const Graphics::API::Buffer& getBuffer() const;
    float getSize() const;

private:
    void initBufferBuilder();
    bool isFacingCamera(const QuadTree* quadTree, const Graphics::Camera& camera);

private:
    std::unique_ptr<QuadTree> _leftQuadTree = nullptr;
    std::unique_ptr<QuadTree> _rightQuadTree = nullptr;
    std::unique_ptr<QuadTree> _frontQuadTree = nullptr;
    std::unique_ptr<QuadTree> _backQuadTree = nullptr;
    std::unique_ptr<QuadTree> _topQuadTree = nullptr;
    std::unique_ptr<QuadTree> _bottomQuadTree = nullptr;

    Graphics::API::Builder::Buffer _bufferBuilder;
    Graphics::API::Buffer _buffer;
    float _size = 0;
};

} // Namespace Core

