#pragma once

#include <cstdint> // uint32_t
#include <memory> // std::unique_ptr
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

#include <Core/QuadTree.hpp> // Core::QuadTree
#include <Graphics/Camera.hpp> // Graphics::Camera
#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer
#include <Graphics/API/Texture.hpp> // Graphics::API::Texture

namespace Graphics {
    class Renderer;
} // Namespace Graphics

namespace Core {

class SphereQuadTree {
public:
    ~SphereQuadTree() = default;

    SphereQuadTree(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree(SphereQuadTree&& quadTree);

    SphereQuadTree& operator=(const SphereQuadTree& quadTree) = delete;
    SphereQuadTree& operator=(SphereQuadTree&& quadTree);

    static std::unique_ptr<SphereQuadTree> create(const Graphics::Renderer* renderer, float size, float maxHeight);

    void update(Graphics::Camera& camera);

    float getSize() const;
    float getMaxHeight() const;
    const Graphics::API::Buffer& getBuffer() const;
    const Graphics::API::Buffer& getDebugBuffer() const;
    const QuadTree::LevelsTable& getLevelsTable() const;
    const Graphics::API::Texture& getHeightMap() const;
    const Graphics::API::Texture& getNormalMap() const;

    void setMaxHeight(float maxHeight);
    void setSize(float size);

private:
    // Only the SphereQuadTree::create can create the quadtree
    SphereQuadTree(float size, float maxHeight);

    bool init(const Graphics::Renderer* renderer);

    void initChildren();
    bool initHeightMap();
    bool initNormalMap(const Graphics::Renderer* renderer);
    void initLevelsDistance();
    bool initBuffer();
    bool initDebugBuffer();

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
    // Buffer storing aabb boxes
    Graphics::API::Buffer _debugBuffer;

    // Store distance needed for each level
    QuadTree::LevelsTable _levelsTable;

    Graphics::API::Texture _heightMap;
    Graphics::API::Texture _normalMap;
};

} // Namespace Core

