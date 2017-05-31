#pragma once

#include <cstdint> // uint32_t
#include <memory> // std::unique_ptr
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

#include <Core/QuadTree.hpp> // Core::QuadTree
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer
#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer
#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Core {

class SphereQuadTree {
public:
    SphereQuadTree(float size);
    ~SphereQuadTree() = default;

    SphereQuadTree(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree(SphereQuadTree&& quadTree);

    SphereQuadTree& operator=(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree& operator=(SphereQuadTree&& quadTree);

    void update(Graphics::Camera& camera);

    const Graphics::API::Buffer& getBuffer() const;
    float getSize() const;
    const QuadTree::LevelsTable& getLevelsTable() const;
    const Graphics::API::Texture& getHeightMap() const;

private:
    bool initHeightMap();
    void initLevelsDistance();
    void initBufferBuilder();

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

    // Store distance needed for each level
    QuadTree::LevelsTable _levelsTable;

    Graphics::API::Texture _heightMap;
};

} // Namespace Core

