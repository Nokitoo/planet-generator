#pragma once

#include <cstdint> // uint32_t
#include <memory> // unique_ptr
#include <vector> // std::vector

#include <Graphics/Camera.hpp> // Graphics::Camera
#include <System/Vector.hpp> // System::Vector

#include <glm/vec3.hpp> // glm::vec3

namespace Core {

class QuadTree {
    friend class SphereQuadTree;

public:
    using LevelsTable = std::vector<float>;

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
        const glm::vec3& normal,
        const LevelsTable& levelsTable,
        uint32_t level);
    QuadTree() = delete;
    ~QuadTree() = default;

    QuadTree(const QuadTree& quadTree) = delete;
    QuadTree(QuadTree&& quadTree) = delete;

    QuadTree& operator=(const QuadTree& quadTree) = delete;
    QuadTree&& operator=(QuadTree&& quadTree) = delete;

    void update(System::Vector<Vertex>& vertices, System::Vector<uint32_t>& indices, const Graphics::Camera& camera);

private:
    bool needSplit(const Graphics::Camera& camera);
    void split();
    bool needMerge(const Graphics::Camera& camera);
    void merge();

    bool isInsideFrustrum(const Graphics::Camera& camera) const;

private:
    Children _children;
    float _size;
    glm::vec3 _pos;
    glm::vec3 _widthDir;
    glm::vec3 _heightDir;
    glm::vec3 _normal;

    Vertex _corners[4];
    glm::vec3 _center;

    const LevelsTable& _levelsTable;
    uint32_t _level = 0;

    bool _split = false;
};

} // Namespace Core

