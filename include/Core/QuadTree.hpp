#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr
#include <vector> // std::vector

#include <glm/vec3.hpp> // glm::vec3

#include <Graphics/API/Buffer.hpp> // Graphics::API::Buffer

namespace Core {

class QuadTree {
    friend class SphereQuadTree;

private:
    struct Children {
        std::unique_ptr<QuadTree> topLeft = nullptr;
        std::unique_ptr<QuadTree> topRight = nullptr;
        std::unique_ptr<QuadTree> bottomLeft = nullptr;
        std::unique_ptr<QuadTree> bottomRight = nullptr;
    };

public:
    QuadTree(float size,
        const glm::vec3& pos,
        const glm::vec3& widthDir,
        const glm::vec3& heightDir,
        const glm::vec3& normal);
    QuadTree() = delete;
    ~QuadTree() = default;

    QuadTree(const QuadTree& quadTree) = delete;
    QuadTree(QuadTree&& quadTree) = delete;

    QuadTree& operator=(const QuadTree& quadTree) = delete;
    QuadTree&& operator=(QuadTree&& quadTree) = delete;

    void update(std::vector<const Graphics::API::Buffer*>& buffers);

private:
    void split();

private:
    bool buildBuffer(float size,
        const glm::vec3& pos,
        const glm::vec3& widthDir,
        const glm::vec3& heightDir,
        const glm::vec3& normal);

private:
    Children _children;

    float _size;
    glm::vec3 _pos;
    glm::vec3 _widthDir;
    glm::vec3 _heightDir;
    glm::vec3 _normal;

    Graphics::API::Buffer _buffer;
};

} // Core

