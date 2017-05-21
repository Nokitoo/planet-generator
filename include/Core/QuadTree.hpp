#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr
#include <vector> // std::vector

#include <glm/vec3.hpp> // glm::vec3

namespace Core {

class QuadTree {
    friend class SphereQuadTree;

private:
    // TODO: Move elsewhere
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
    };

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

    void update(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t level);

private:
    void split();
    void merge();

private:
    Children _children;

    float _size;
    glm::vec3 _pos;
    glm::vec3 _widthDir;
    glm::vec3 _heightDir;
    glm::vec3 _normal;

    Vertex _corners[4];

    bool _split = false;
};

} // Core

