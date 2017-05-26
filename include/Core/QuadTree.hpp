#pragma once

#include <cstdint> // uint32_t, uint8_t
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

    // TODO: Move elsewhere
    struct Vertex {
        glm::vec3 pos;
        float quadTreeLevel;
    };

    // Enum order is important !
    // It's used to rotate orientation:
    // TL | TR          BL | TL
    // -------  + 1 =>  -------
    // BL | BR          BR | TR
    enum class ChildOrientation: uint8_t {
        TOP_LEFT = 0,
        TOP_RIGHT = 1,
        BOTTOM_RIGHT = 2,
        BOTTOM_LEFT = 3
    };

    // Enum order is important, same as ChildOrientation
    enum class NeighborOrientation: uint8_t {
        TOP = 0,
        RIGHT = 1,
        BOTTOM = 2,
        LEFT = 3
    };

    enum class Face: uint8_t {
        LEFT = 0,
        RIGHT = 1,
        FRONT = 2,
        BACK = 3,
        TOP = 4,
        BOTTOM = 5
    };

private:
    struct Children {
        std::unique_ptr<QuadTree> topLeft = nullptr;
        std::unique_ptr<QuadTree> topRight = nullptr;
        std::unique_ptr<QuadTree> bottomLeft = nullptr;
        std::unique_ptr<QuadTree> bottomRight = nullptr;
    };

    struct Neighbors {
        QuadTree* top = nullptr;
        QuadTree* left = nullptr;
        QuadTree* right = nullptr;
        QuadTree* bottom = nullptr;
    };

public:
    QuadTree(float size,
        const glm::vec3& pos,
        const glm::vec3& widthDir,
        const glm::vec3& heightDir,
        const glm::vec3& normal,
        Face face,
        const LevelsTable& levelsTable,
        uint32_t level,
        QuadTree* parent = nullptr);
    QuadTree() = delete;
    ~QuadTree() = default;

    QuadTree(const QuadTree& quadTree) = delete;
    QuadTree(QuadTree&& quadTree) = delete;

    QuadTree& operator=(const QuadTree& quadTree) = delete;
    QuadTree&& operator=(QuadTree&& quadTree) = delete;

    void update(const Graphics::Camera& camera);
    void updateNeighBors();
    void setNeighBors(QuadTree* top, QuadTree* left, QuadTree* right, QuadTree* bottom);

    uint8_t getOrientationRotationFromFace(Face fromFace);
    QuadTree* getChild(Face fromFace, ChildOrientation childOrientation);
    QuadTree* getNeighbor(Face fromFace, NeighborOrientation neighborOrientation);

    void setNeighbor(Face fromFace, NeighborOrientation neighborOrientation, QuadTree* neighbor);

private:
    void addChildrenVertices(System::Vector<Vertex>& vertices, System::Vector<uint32_t>& indices);

    bool needSplit(const Graphics::Camera& camera);
    void split();
    bool needMerge(const Graphics::Camera& camera);
    void merge();

    bool isInsideFrustrum(const Graphics::Camera& camera) const;

private:
    Children _children;
    Neighbors _neighbors;

    float _size;
    glm::vec3 _pos;
    glm::vec3 _widthDir;
    glm::vec3 _heightDir;
    glm::vec3 _normal;

    Vertex _corners[4];
    glm::vec3 _center;
    Face _face = Face::FRONT;

    const LevelsTable& _levelsTable;
    uint32_t _level = 0;

    bool _split = false;
    QuadTree* _parent = nullptr;
};

inline QuadTree::ChildOrientation operator+(QuadTree::ChildOrientation a, uint8_t b) {
    return static_cast<QuadTree::ChildOrientation>((static_cast<uint8_t>(a) + b) % 4);
}

inline QuadTree::NeighborOrientation operator+(QuadTree::NeighborOrientation a, uint8_t b) {
    return static_cast<QuadTree::NeighborOrientation>((static_cast<uint8_t>(a) + b) % 4);
}


} // Namespace Core

