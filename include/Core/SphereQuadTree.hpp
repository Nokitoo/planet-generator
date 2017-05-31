#pragma once

#include <cstdint> // uint32_t
#include <memory> // std::unique_ptr
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

#include <Core/QuadTree.hpp> // Core::QuadTree
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer
#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Core {

class SphereQuadTree {
public:
    SphereQuadTree(float size, float maxHeight);
    ~SphereQuadTree() = default;

    SphereQuadTree(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree(SphereQuadTree&& quadTree);

    SphereQuadTree& operator=(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree& operator=(SphereQuadTree&& quadTree);

    void update(Graphics::Camera& camera);

    float getSize() const;
    float getMaxHeight() const;
    const Graphics::API::Buffer& getBuffer() const;
    const QuadTree::LevelsTable& getLevelsTable() const;
    const Graphics::API::Texture& getHeightMap() const;

    void setMaxHeight(float maxHeight);

private:
    bool initHeightMap();
    void initLevelsDistance();
    void initBufferBuilder();

private:
    float _size = 0.0f;
    float _maxHeight = 0.0f;

    std::unique_ptr<QuadTree> _leftQuadTree = nullptr;
    std::unique_ptr<QuadTree> _rightQuadTree = nullptr;
    std::unique_ptr<QuadTree> _frontQuadTree = nullptr;
    std::unique_ptr<QuadTree> _backQuadTree = nullptr;
    std::unique_ptr<QuadTree> _topQuadTree = nullptr;
    std::unique_ptr<QuadTree> _bottomQuadTree = nullptr;

    // Buffer storing vertices and indices
    Graphics::API::Buffer _buffer;

    // Store distance needed for each level
    QuadTree::LevelsTable _levelsTable;

    Graphics::API::Texture _heightMap;
};

} // Namespace Core

