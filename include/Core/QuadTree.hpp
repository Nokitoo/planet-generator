#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr
#include <vector> // std::vector

#include <glm/vec3.hpp> // glm::vec3

#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Core {

class QuadTree {
private:
    struct Children {
        std::unique_ptr<QuadTree> _topLeft = nullptr;
        std::unique_ptr<QuadTree> _topRight = nullptr;
        std::unique_ptr<QuadTree> _bottomLeft = nullptr;
        std::unique_ptr<QuadTree> _bottomRight = nullptr;
    };

public:
    QuadTree(float size,
        const glm::vec3& pos,
        const glm::vec3& widthDir,
        const glm::vec3& heightDir,
        const glm::vec3& color,
        uint32_t maxRecurse = 4);
    QuadTree() = delete;
    ~QuadTree() = default;

    QuadTree(const QuadTree& quadTree) = delete;
    QuadTree(QuadTree&& quadTree) = delete;

    QuadTree& operator=(const QuadTree& quadTree) = delete;
    QuadTree&& operator=(QuadTree&& quadTree) = delete;

    void update(std::vector<const Graphics::API::Buffer*>& buffers);

private:
    bool buildBuffer(float size,
        const glm::vec3& pos,
        const glm::vec3& widthDir,
        const glm::vec3& heightDir,
        const glm::vec3& color);

private:
    Children _children;

    Graphics::API::Buffer _buffer;
};

} // Core

