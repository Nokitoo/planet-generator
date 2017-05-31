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
    // widthDir and heightDir are used to calculate normal in vertex shader
    struct Vertex {
        glm::vec3 cubePos;
        glm::vec3 spherePos;
        glm::vec3 widthDir;
        glm::vec3 heightDir;
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

    struct Corners {
        Vertex topLeft;
        Vertex topRight;
        Vertex bottomLeft;
        Vertex bottomRight;
    };

    // AABB box used for frustum culling
    struct AABB {
        // Correspond to QuadTree::Corners
        // padded on left/right and top/bottom sides
        // (Because the sides are rounded)
        struct {
            glm::vec3 topLeft;
            glm::vec3 topRight;
            glm::vec3 bottomLeft;
            glm::vec3 bottomRight;
        } corners;

        // Is the same as AABB::corners
        // except that the rounded shape bend is added
        // to fill all the quadtree shape
        //  |\          \ |
        //  | \          \|
        //  |  |    =>    |
        //  | /          /|
        //  |/          / |
        struct {
            glm::vec3 topLeft;
            glm::vec3 topRight;
            glm::vec3 bottomLeft;
            glm::vec3 bottomRight;
        } cornersUp;
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
        float planetSize);
    QuadTree() = delete;
    ~QuadTree() = default;

    QuadTree(const QuadTree& quadTree) = delete;
    QuadTree(QuadTree&& quadTree) = delete;

    QuadTree& operator=(const QuadTree& quadTree) = delete;
    QuadTree&& operator=(QuadTree&& quadTree) = delete;

    void update(Graphics::Camera& camera);
    void updateNeighBors();
    void setNeighBors(QuadTree* top, QuadTree* left, QuadTree* right, QuadTree* bottom);

    uint8_t getOrientationRotationFromFace(Face fromFace);
    QuadTree* getChild(Face fromFace, ChildOrientation childOrientation);
    QuadTree* getNeighbor(Face fromFace, NeighborOrientation neighborOrientation);

    void setNeighbor(Face fromFace, NeighborOrientation neighborOrientation, QuadTree* neighbor);

private:
    void addChildrenVertices(System::Vector<Vertex>& vertices, System::Vector<uint32_t>& indices);

    glm::vec3 calculateSpherePos(const glm::vec3& cubePos);
    void calculateShapeAABB();

    bool needSplit(const Graphics::Camera& camera);
    void split();
    bool needMerge(const Graphics::Camera& camera);
    void merge();

    bool isInsideFrustum(Graphics::Camera& camera) const;
    bool isOccludedByHorizon(const Graphics::Camera& camera) const;

private:
    Children _children;
    Neighbors _neighbors;

    float _size;
    glm::vec3 _pos;
    glm::vec3 _widthDir;
    glm::vec3 _heightDir;
    glm::vec3 _normal;

    Corners _corners;
    glm::vec3 _center;
    AABB _shapeBox;

    Face _face = Face::FRONT;

    const LevelsTable& _levelsTable;
    uint32_t _level = 0;

    bool _split = false;
    float _planetSize = 0;
};

inline QuadTree::ChildOrientation operator+(QuadTree::ChildOrientation a, uint8_t b) {
    return static_cast<QuadTree::ChildOrientation>((static_cast<uint8_t>(a) + b) % 4);
}

inline QuadTree::NeighborOrientation operator+(QuadTree::NeighborOrientation a, uint8_t b) {
    return static_cast<QuadTree::NeighborOrientation>((static_cast<uint8_t>(a) + b) % 4);
}


} // Namespace Core

